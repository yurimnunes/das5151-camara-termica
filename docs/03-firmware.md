# Firmware

## Preparar o Linux (uma vez)

Testado no Pop!_OS 24.04.

1. **arduino-cli** (compila e grava pelo terminal, sem IDE):
   ```sh
   curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
   arduino-cli core update-index
   arduino-cli core install arduino:avr
   arduino-cli lib install OneWire DallasTemperature
   ```
2. **Permissão na porta serial:** `sudo usermod -aG dialout $USER`. Sem isso, gravar dá `Permission denied` em `/dev/ttyACM0`. O Linux só carrega os grupos no login, então a permissão vale depois de encerrar a sessão do computador (menu do canto superior direito → Encerrar sessão) e entrar de novo, ou reiniciar. Para usar antes disso, rodar cada comando com `sg dialout -c '<comando>'`.
3. **Monitor em Python:** na raiz do repo, `python3 -m venv .venv && .venv/bin/pip install -r requirements.txt`.
4. **Só se o Uno for clone com chip CH340** e não aparecer `/dev/ttyUSB0` ao conectar: `sudo apt remove brltty` (esse pacote "rouba" a porta). O Uno original aparece como `/dev/ttyACM0` e não tem esse problema.

Quem preferir interface gráfica pode usar a Arduino IDE 2 (AppImage ou Flatpak); os passos 2 e 4 continuam valendo.

## Compilar, gravar e monitorar

```sh
arduino-cli board list                                    # descobrir a porta (ex.: /dev/ttyACM0)
arduino-cli compile firmware/teste_camara                 # placa já definida em sketch.yaml
arduino-cli upload -p /dev/ttyACM0 firmware/teste_camara
.venv/bin/python scripts/monitor_serial.py -n degrau-30   # mostra, grava e aceita comandos
```

O monitor (`scripts/monitor_serial.py`):

* mostra cada linha do Arduino e grava em `dados/<data>_<nome>.tsv`, com o horário do PC na primeira coluna;
* aceita comandos digitados no terminal ou escritos no FIFO: `echo 30 > build/serial_cmd`. É assim que outro terminal ou um agente de IA ajusta o PWM com o monitor rodando;
* registra cada comando no log (`# comando (...)`), para saber quando cada degrau começou;
* ao sair (Ctrl+C ou kill), manda `0` e desliga o aquecedor.

Só um programa usa a porta por vez: fechar o monitor antes de gravar (`upload`), e não abrir o Serial Monitor da IDE junto. Abrir a porta reinicia o Uno, então todo log começa pelo cabeçalho do `setup()`.

## `firmware/teste_camara/teste_camara.ino`

Código de teste dos módulos, ainda sem controle em malha fechada. Serve para validar sensores e acionamento na bancada e fazer o ensaio de degrau manualmente.

### Dependências

* OneWire (Paul Stoffregen)
* DallasTemperature (Miles Burton)

### O que faz

* **DS18B20 (D2):** resolução de 12 bits, conversão não bloqueante. A cada segundo lê o resultado anterior e dispara a próxima conversão.
* **Pt100 (A0):** média de 16 leituras, converte para tensão (referência fixa de 5,0 V) e depois para temperatura pela reta da simulação:
  `T = 25 + (V − 1,736) / 0,0687`
* **PWM (D3):** valor em % digitado no Serial Monitor, limitado a 80 % (`PWM_MAX_PCT`).
* **Proteção:** desliga o PWM se a temperatura do Pt100 passar de 60 °C ou se A0 passar de 4,3 V (INA saturado).
* **Saída serial:** 1 linha por segundo, separada por tabulação.

### Uso pela IDE (alternativa ao monitor em Python)

1. Serial Monitor em 9600 baud, final de linha "Nova linha".
2. Conferir no cabeçalho quantos DS18B20 foram encontrados (esperado: 1).
3. Digitar um número de 0 a 100 e Enter para ajustar o PWM.
4. Copiar o log para planilha ou script. Colunas:

```
tempo_s  PWM_%  A0_V  T_pt100_C  T_amb_C
```

### Constantes a ajustar

| Constante | Valor atual | Observação |
|---|---|---|
| `SENS_V_POR_C` | 0,0687 | Vem da simulação. Trocar pelo valor da calibração em 2 pontos |
| `V_REF_25C` | 1,736 | Idem |
| `PWM_MAX_PCT` | 80 | Limite de segurança nos testes |
| `T_MAX_CAMARA` | 60,0 | Corte por sobretemperatura |

### Observações

* Texto não numérico no Serial vira 0 em `parseInt`, então desliga o aquecedor (falha segura).
* A proteção zera o PWM mas não trava: se o usuário mandar outro valor, o aquecedor liga de novo até a próxima checagem (1 s depois).
* A referência do A/D vem do mesmo 5 V que alimenta a ponte e o INA, então variações do 5 V do USB se cancelam em parte. O restante é absorvido pela calibração.

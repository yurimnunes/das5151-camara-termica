# Firmware

## `teste_camara.ino`

Código de teste dos módulos, ainda sem controle em malha fechada. Serve para validar sensores e acionamento na bancada e fazer o ensaio de degrau manualmente.

### Dependências

Instalar pelo gerenciador de bibliotecas da IDE do Arduino:

* OneWire (Paul Stoffregen)
* DallasTemperature (Miles Burton)

### O que faz

* **DS18B20 (D2):** resolução de 12 bits, conversão não bloqueante. A cada segundo lê o resultado anterior e dispara a próxima conversão.
* **Pt100 (A0):** média de 16 leituras, converte para tensão (referência fixa de 5,0 V) e depois para temperatura pela reta da simulação:
  `T = 25 + (V − 1,736) / 0,0687`
* **PWM (D3):** valor em % digitado no Serial Monitor, limitado a 80 % (`PWM_MAX_PCT`).
* **Proteção:** desliga o PWM se a temperatura do Pt100 passar de 60 °C ou se A0 passar de 4,3 V (INA saturado).
* **Saída serial:** 1 linha por segundo, separada por tabulação.

### Uso

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

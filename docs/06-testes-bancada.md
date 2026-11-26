# Roteiro de testes na bancada

Testar por partes, nesta ordem. Só passar para a próxima parte quando a anterior bater com o esperado.

1. **Sensores** (medição interna com ponte + INA125P e medição ambiente com DS18B20): este documento
2. Acionamento do aquecedor (PC817 + TIP122): a escrever
3. Sistema completo e ensaio de degrau: a escrever

---

## Parte 1: sensores (medição interna e ambiente)

**Montar só:**

* Medição interna: Pt100 A e B, R3 e R5 (100 Ω), Rtop (1,5 kΩ), Rbot (1 kΩ), INA125P com RG (330 Ω) e capacitor de 100 nF, filtro RC (1 kΩ + 100 µF). Esquema: [`esq_pt100_ina125.png`](../relatorio/figuras/esq_pt100_ina125.png).
* Medição ambiente: DS18B20 com pull-up de 3,3 kΩ e capacitor de 100 nF. Esquema: [`esq_ds18b20.png`](../relatorio/figuras/esq_ds18b20.png).

**Não ligar:** acionamento e fonte de 12 V. O firmware `teste_sensores` mantém o D3 em nível baixo de qualquer forma.

As partes 1.1 a 1.3 testam a ponte, a 1.4 testa a ligação do DS18B20 e a 1.5 lê os dois juntos pelo Arduino.

Os valores esperados abaixo supõem alimentação de exatamente 5,00 V. Se o 5 V do Arduino medir diferente (no USB é comum ficar entre 4,8 e 5,1 V), as tensões mudam na mesma proporção.

### 1.1 Componentes, sem alimentação (multímetro em Ω)

| Medição | Esperado |
|---|---|
| Pt100: entre os 2 fios do par | perto de 0 Ω (só os fios) |
| Pt100: fio único até o par | 107,8 Ω a 20 °C, 109,7 Ω a 25 °C, 111,7 Ω a 30 °C |
| Diferença entre Pt100 A e B na mesma temperatura | menor que 0,5 Ω |
| R3 e R5 | 99 a 101 Ω (1 %) |
| Rtop / Rbot / RG | 1,5 kΩ / 1 kΩ / 330 Ω |

Anotar os valores medidos de R3, R5 e dos Pt100. Eles explicam depois o offset da leitura.

### 1.2 Só a ponte, sem o INA (multímetro em V)

Conectar o Arduino pelo USB. A ponte é alimentada pelo 5 V do Arduino. Medir em relação ao GND:

| Ponto | 20 °C | 25 °C | 30 °C |
|---|---|---|---|
| 5 V real | anotar | | |
| Nó T (topo da ponte) | 2,120 V | 2,121 V | 2,122 V |
| Nó F (base da ponte) | 1,920 V | 1,919 V | 1,919 V |
| Tensão sobre a ponte (T − F) | 199,5 mV | 201,3 mV | 203,1 mV |
| Nó a | 2,0237 V | 2,0248 V | 2,0259 V |
| Nó b | 2,0162 V | 2,0155 V | 2,0147 V |
| **Vab** (ponta vermelha em a, preta em b) | **7,48 mV** | **9,34 mV** | **11,20 mV** |

**Teste de sensibilidade:** segurar um Pt100 de cada vez com os dedos. Vab tem que **subir** com qualquer um dos dois, cerca de 0,19 mV por °C em cada sensor. Se um deles fizer Vab **cair**, esse sensor está no braço errado da ponte.

Um erro de alguns décimos de mV em Vab é normal: com resistores de 1 %, cada 1 Ω de diferença em R3 ou R5 desloca a leitura em cerca de 1 a 2 °C. A calibração corrige isso depois.

### 1.3 Com o INA125P, antes do filtro

Medir no pino 10 (VO) em relação ao GND:

| T | Vab | VO esperado |
|---|---|---|
| 20 °C | 7,48 mV | 1,390 V |
| 25 °C | 9,34 mV | 1,736 V |
| 30 °C | 11,20 mV | 2,081 V |

Regra rápida: VO ≈ 186 × Vab. Segurando um Pt100, VO sobe cerca de 35 mV por °C.

### 1.4 DS18B20: ligação

**Antes de energizar,** descobrir qual fio é o terra. A sonda tem fios vermelho, azul e preto, e inverter alimentação e terra queima o sensor. Com o multímetro em modo diodo, o terra é o fio que mostra queda de diodo (cerca de 0,5 a 0,7 V) com a ponta **vermelha** nele e a preta em cada um dos outros dois.

| Ligação | Pino do Arduino |
|---|---|
| Vermelho (VDD) | 5 V |
| Dados (DQ) | D2, com 3,3 kΩ entre D2 e 5 V |
| Terra (confirmado com multímetro) | GND |
| 100 nF entre VDD e GND, perto da sonda | |

Com o Arduino ligado, a linha de dados (D2) em repouso mede perto de 5 V.

### 1.5 Com o Arduino lendo os dois sensores

Ligar a saída do filtro no A0, gravar o firmware e abrir o monitor:

```sh
arduino-cli board list
arduino-cli upload -p /dev/ttyACM0 firmware/teste_sensores
.venv/bin/python scripts/monitor_serial.py -n sensores
```

O cabeçalho deve mostrar `DS18B20 encontrados: 1`.

Colunas: `tempo_s  A0_cont  ruido_cont  A0_V  Vab_mV  T_pt100_C  T_amb_C`.

| O que olhar | Esperado |
|---|---|
| `A0_cont` a 25 °C | cerca de 355 (284 a 20 °C, 426 a 30 °C) |
| `A0_V` | igual ao VO medido no 1.3, com diferença de poucos mV |
| `ruido_cont` | 0 a 2 contagens. Acima de 5: mau contato, fio longo ou falta do capacitor |
| `T_pt100_C` | perto do ambiente, com offset de alguns °C antes da calibração |
| Segurar um Pt100 por 30 s e soltar | sobe alguns °C de forma suave e depois volta devagar |
| `T_amb_C` | temperatura ambiente, variando no máximo 0,1 a 0,2 °C com tudo parado |
| Segurar a sonda do DS18B20 | `T_amb_C` sobe; `T_pt100_C` não muda (prova que um sensor não interfere no outro) |
| `T_pt100_C` contra `T_amb_C`, com os Pt100 fora da câmara | diferença constante = offset do Pt100, útil para a calibração |

Deixar rodando 5 a 10 minutos parado. Esse log (`dados/*_sensores.tsv`) mostra a estabilidade e o ruído das duas medições.

### Diagnóstico rápido

| Sintoma | Causa provável |
|---|---|
| Vab perto de 0 e não muda ao aquecer | os dois Pt100 no mesmo lado da ponte, ou ponte sem alimentação |
| Vab negativo | a e b trocados no multímetro ou no INA, ou os dois Pt100 trocados de posição com R3 e R5 |
| Vab de dezenas ou centenas de mV | sensor aberto ou resistor de valor errado (conferir 1.1) |
| VO preso perto de 0 V | Vab negativo (VIN+ e VIN− trocados) ou INA sem alimentação (pinos 1 e 2) |
| VO preso perto de 4 V ou mais | saturação: Vab alto demais, RG errado ou SENSE (pino 11) solto do VO (pino 10) |
| VO certo mas `A0_V` diferente | filtro mal ligado, capacitor invertido (faixa no GND) ou GNDs diferentes |
| `ruido_cont` alto | mau contato na protoboard, falta do 100 nF entre os pinos 1 e 3, cabo do Pt100 perto de fonte de ruído |
| `DS18B20 encontrados: 0` e `T_amb_C` = `erro` | dados fora do D2, falta do pull-up de 3,3 kΩ ou fios trocados |
| `T_amb_C` = 85,00 fixo | sensor sem alimentação estável (é o valor de quando liga) ou VDD mal ligado |
| `T_amb_C` = −127 ou `erro` às vezes | mau contato no fio de dados ou pull-up fraco para o cabo |

### Resultado da Parte 1

Ensaios já feitos: [01 (2026-11-26)](ensaios/01-sensores-2026-11-26.md).

Registrar no [diário](diario.md): valores medidos em 1.1 a 1.3, nome do arquivo de log do 1.4, ruído observado e o que precisou ser corrigido.

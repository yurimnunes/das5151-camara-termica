# Hardware

## Pinagem no Arduino

| Pino | Ligação |
|---|---|
| D2 | DQ do DS18B20 (pull-up de 3,3 kΩ para 5 V) |
| D3 | PWM (490 Hz) → R1 680 Ω → LED do PC817 |
| A0 | Saída do INA125P após filtro RC (1 kΩ + 100 µF) |
| 5 V | Ponte, INA125P, DS18B20 |
| GND | Terra do lado Arduino (isolado do terra da fonte) |

## Medição da temperatura interna (Pt100 + INA125P)

Esquema: [`esq_pt100_ina125.png`](../relatorio/figuras/esq_pt100_ina125.png)

* Ponte: Pt100 A e Pt100 B em braços opostos, completada por 2 resistores de 100 Ω (equilíbrio em 0 °C). Sensores opostos dobram a sensibilidade.
* Rtop 1,5 kΩ e Rbot 1 kΩ em série com a ponte: corrente total de 1,92 mA (0,96 mA por Pt100) e modo comum perto de 2 V.
* Tensão sobre a ponte: 192,3 mV a 0 °C (1,923 mA × 100 Ω). Sobe com a temperatura, porque a resistência dos Pt100 aumenta, e fica perto de 200 mV na faixa de operação. A simulação já considera isso, e a calibração em 2 pontos absorve qualquer diferença de ganho.
* Sensibilidade da ponte: cerca de 0,37 mV/°C.
* INA125P com RG = 330 Ω: G = 4 + 60 kΩ / RG ≈ 186.
* Saída: cerca de 68,7 mV/°C, resolução de 0,071 °C por LSB.
* Faixa linear da saída: 0,8 a 4,2 V, que dá 12 a 61 °C.
* Pinos do INA125P: SLEEP em V+, SENSE em VO, V−, IAREF e VREFCOM no GND. Pinos 4, 13 a 16 sem ligação. 100 nF entre os pinos 1 e 3.
* 3 fios: o fio único de cada sensor fica em série com o próprio sensor, um fio do par fica em série com o resistor vizinho (resistências de fio se cancelam), o outro fio do par leva a tensão ao INA sem corrente.
* Filtro RC: τ = 0,1 s, fc ≈ 1,6 Hz.
* Conversão linear (erro de não linearidade ≤ 0,11 °C entre 15 e 60 °C) com calibração em 2 pontos para tirar o offset dos resistores de 1 %.

## Temperatura ambiente (DS18B20)

Esquema: [`esq_ds18b20.png`](../relatorio/figuras/esq_ds18b20.png)

* Sonda inox à prova d'água, cabo de 90 cm, barramento 1-Wire.
* Fios: vermelho em 5 V, dados em D2, terra no GND. A sonda tem fios vermelho, azul e preto: **confirmar o terra com multímetro em modo diodo antes de energizar**, inversão queima o sensor.
* 12 bits (0,0625 °C), conversão de até 750 ms, leitura não bloqueante a cada 1 s.
* Exatidão de ±0,5 °C entre −10 e 85 °C.
* Posição: suspensa no ar a cerca de 20 cm da câmara, longe da fonte e das passagens de cabos.

## Acionamento do aquecedor (PC817 + TIP122)

Esquema: [`esq_acionamento.png`](../relatorio/figuras/esq_acionamento.png)

* LED do PC817: R1 = 680 Ω, IF ≈ 5,6 mA.
* Fototransistor: R2 = 3,3 kΩ do 12 V para o coletor, emissor na base do TIP122. Com CTR mínimo de 50 %, entrega cerca de 2,7 mA à base.
* TIP122: hFE mínimo 1000, precisa de pelo menos 2,2 mA de base. Folga pequena: **medir VCE na montagem; se passar de 2 V, trocar R1 por 330 Ω**.
* Pull-down de 22 kΩ na base mantém o TIP122 cortado durante o boot do Arduino.
* Dissipação no TIP122: cerca de 2,5 W em 100 %. Dissipador TO-220 com pasta é obrigatório. A aba é o coletor: não encostar em outro metal.
* Diodo 1N4148 em antiparalelo com o aquecedor (roda livre).
* Caminho de 2,2 A (fonte, placa, TIP122, retorno) fora da protoboard, em barra de terminais com fio de pelo menos 0,5 mm².
* Opcional: KSD301 70 °C NF em série com o aquecedor como proteção independente do software.

## Estrutura mecânica

Esboço: [`esboco_mecanico.png`](../relatorio/figuras/esboco_mecanico.png)

* Placa de resistores suspensa por suporte metálico preso à tampa, sem tocar o EPS.
* Ventoinha em pé junto a uma parede, soprando sobre a placa.
* Pt100 entram pela parede oposta, a alguns centímetros da placa, medindo o ar.
* Fora da câmara: DS18B20, Arduino, protoboard, TIP122 com dissipador, fonte.
* Passagens de cabos vedadas com cola quente.
* Local previsto para uma portinhola, para gerar perturbações nas próximas etapas.

## Lista de componentes

Tabela completa em [`relatorio/secoes/03-componentes.tex`](../relatorio/secoes/03-componentes.tex).

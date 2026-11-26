# Simulações (LTspice)

> **Medição interna com LM324 (2026-11-26):** o INA125P queimou e foi trocado por um amplificador de 3 amp-ops com LM324. A simulação nova (ngspice, G ≈ 119, 44,4 mV/°C) está em [`referencias/lm324.md`](referencias/lm324.md#simulação), netlist [`referencias/lm324_ponte.cir`](referencias/lm324_ponte.cir). A seção "Medição interna" abaixo é do projeto original com o INA125P e continua valendo para o relatório até ele ser atualizado.

Modelos usados: INA125P pela topologia interna de 3 amp-ops, TIP122 como par Darlington com os resistores internos, PC817 como LED + fototransistor com CTR de 50 % (pior caso).

## Medição interna (Pt100)

Pt100 modelados por Callendar–Van Dusen, resistência de fio RL = 0,1 Ω, varredura de 0 a 100 °C.

| T (°C) | Vab (mV) | Vout (V) | Contagens A/D |
|---|---|---|---|
| 15 | 5,62 | 1,044 | 214 |
| 20 | 7,48 | 1,390 | 284 |
| 25 | 9,34 | 1,736 | 355 |
| 30 | 11,20 | 2,081 | 426 |
| 40 | 14,90 | 2,768 | 566 |
| 50 | 18,58 | 3,452 | 706 |
| 60 | 22,25 | 4,134 | 846 |

* Sensibilidade de 68,7 mV/°C, resolução de 0,071 °C/LSB.
* Faixa útil de 12 a 61 °C.
* Não linearidade ≤ 0,11 °C entre 15 e 60 °C.
* Com RL exagerado de 1 Ω: erro de −0,02 °C a 50 °C com 3 fios contra +5,2 °C com 2 fios.

## Acionamento

PWM de 490 Hz, D = 50 %, indutância parasita de 2 µH.

Resultados citados no texto (componentes finais):

* 2,17 A no aquecedor em condução, VCE ≈ 1,13 V.
* 5,6 mA no LED, cerca de 2,7 mA na base.
* 12,2 W no aquecedor e 1,3 W no TIP122 em 50 % (cerca de 24 W e 2,6 W em 100 %).
* Pico de 12,1 V no coletor no desligamento.

## Status das figuras

As figuras de simulação no relatório **ainda são da versão antiga** e não batem com o texto:

| Figura | Está na imagem | Texto/esquema final |
|---|---|---|
| `lt_esq_acion.png` | 24 V, 2 × 25 Ω, R1 750 Ω, R2 4,7 kΩ, Rpd 10 kΩ, 1N4007 | 12 V, 3 × 15 Ω, R1 680 Ω, R2 3,3 kΩ, Rpd 22 kΩ, 1N4148 |
| `lt_formas_acion.png` | cerca de 1,8 A, VC em 24 V, 5 mA no LED | 2,17 A, 12,1 V, 5,6 mA |
| `lt_esq_pt100.png` | filtro Rf 10 kΩ, Cf 10 µF | Rf 1 kΩ, Cf 100 µF (mesmo τ = 0,1 s) |

O relatório já tem um aviso em vermelho (fim da Seção 7) pedindo para refazer essas simulações com os arquivos de `simulacoes_ltspice_final.zip`. Esse zip e os `.asc` não estão nesta pasta.

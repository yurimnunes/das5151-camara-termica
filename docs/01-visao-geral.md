# Visão geral

## Objetivo

Controlar a temperatura de uma câmara térmica pequena e confinada:

* **Planta:** caixa de EPS de 4 a 6 L (cerca de 20 × 15 × 15 cm), paredes de pelo menos 20 mm.
* **Atuador:** placa com 3 resistores de 15 Ω / 20 W em paralelo (5 Ω), alimentada em 12 V e chaveada por PWM.
* **Sensor principal:** 2 Pt100 a 3 fios em braços opostos de uma ponte de Wheatstone, amplificados por um amplificador de instrumentação montado com LM324 (o INA125P previsto queimou).
* **Perturbação medida:** temperatura ambiente, lida por um DS18B20 fora da câmara, para compensação antecipativa (feedforward).
* **Homogeneização:** ventoinha de 120 mm em rotação constante.
* **Controlador:** Arduino Uno R3.

## Premissas

| Item | Valor |
|---|---|
| Setpoint | do ambiente até cerca de 50 °C |
| Limite do EPS | amolece perto de 80 °C |
| Corte por software | 60 °C |
| Faixa de medição interna | cerca de 3 a 76 °C (com o LM324) |
| A/D | 10 bits, referência de 5 V |
| Fonte de potência | bancada 2 canais, 12 V cada (canal 1 aquecedor com limite de 2,6 A, canal 2 ventoinha) |

## Mudanças em relação à proposta inicial

* Medição da temperatura ambiente incluída (sugestão da avaliação), com DS18B20 fornecido pelo professor.
* Componentes trocados pelo que o LMM tem: INA125P (alimentação única de 5 V), PC817 + TIP122 no acionamento, placa de resistores do laboratório.
* Ligação a 3 fios do Pt100 na configuração orientada pelo professor (um fio do par leva o sinal ao amplificador).
* **INA125P trocado por LM324 (2026-11-26).** O INA125P queimou nos testes de bancada e não havia outro no laboratório. No lugar dele entrou um amplificador de instrumentação de 3 amp-ops com LM324, ganho 119. Detalhes em [`referencias/lm324.md`](referencias/lm324.md).
* Medição de corrente por shunt retirada: o optoacoplador isola os terras e medir corrente exigiria juntá-los de novo. Pode voltar em etapas futuras.

## Balanço térmico estimado

* Potência máxima: cerca de 24 W (2,2 A com 10,9 V no aquecedor, descontada a queda no TIP122).
* Condutância térmica global estimada: 0,3 a 0,6 W/K.
* Elevação possível em regime: 40 a 80 K acima do ambiente. Só 25 K são necessários (50 °C com ambiente de 25 °C), então o sinal de controle será limitado por software.
* Valor real da condutância: sai do ensaio de degrau em malha aberta (próxima etapa).

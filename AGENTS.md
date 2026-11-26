# AGENTS.md

Contexto para agentes de IA que trabalham neste repositório.

## Projeto em 5 linhas

Projeto final de DAS5151 Instrumentação em Controle (UFSC). Controle de temperatura de uma câmara de EPS (4 a 6 L) aquecida por 3 resistores de 15 Ω em paralelo, chaveados por PWM (Arduino D3 → PC817 → TIP122, 12 V, cerca de 24 W). Temperatura interna: 2 Pt100 a 3 fios em braços opostos de ponte de Wheatstone + INA125P (G ≈ 186, 68,7 mV/°C, faixa 12 a 61 °C) no A0. Temperatura ambiente: DS18B20 no D2, para feedforward. Setpoint até 50 °C, corte por software em 60 °C.

## Por onde começar

1. `README.md`: estrutura e índice
2. `docs/01-visao-geral.md` a `docs/05-pendencias.md`: resumos por tema
3. `docs/diario.md`: o que mudou e quando

## Fonte da verdade

* `Primeira_Entrega_Projeto_Final___Instrumentação/main.tex` manda em valores de componentes e cálculos. Se os docs divergirem dele, o `.tex` vence e os docs devem ser corrigidos.
* `teste_camara.ino` é o firmware de teste (sem malha fechada ainda).

## Armadilhas conhecidas

* A pasta do relatório tem 116 PNGs, mas o `main.tex` só usa 10. O resto (`fig*`, `ex*-m=*`, `bif*`, `lr_*`, `pi_*`, `controlep*`, `esq_lm35.png` etc.) é sobra de outros trabalhos. Não usar como referência do projeto.
* `lt_esq_acion.png`, `lt_formas_acion.png` e `lt_esq_pt100.png` são de uma versão antiga do circuito (24 V, 2 × 25 Ω, filtro 10 kΩ/10 µF). O texto do `.tex` já usa os valores finais. Detalhes em `docs/04-simulacoes.md`.
* Os arquivos `.asc` do LTspice (`simulacoes_ltspice_final.zip`) não estão no repositório.
* Tensão sobre a ponte: 192,3 mV a 0 °C (1,923 mA × 100 Ω), sobe para perto de 200 mV na faixa de operação. Não é um valor fixo.

## Comandos

Compilar o relatório sem sujar a pasta de fontes:

```sh
cd Primeira_Entrega_Projeto_Final___Instrumentação
latexmk -pdf -outdir=../build main.tex
```

Firmware: IDE do Arduino com as bibliotecas OneWire e DallasTemperature. Serial em 9600 baud.

## Convenções

* Docs e relatório em português. Decimais com vírgula no texto (`68,7 mV/°C`).
* Não usar travessão como pontuação em texto corrido.
* Commits com gitmoji (`:memo:` docs, `:sparkles:` novidade, `:bug:` correção). Sem linha de coautoria de IA.
* Ao concluir algo relevante: adicionar entrada datada no topo de `docs/diario.md` e marcar o item em `docs/05-pendencias.md`.
* Imagens usadas pelo `.tex` ficam versionadas. Não colocar PNG no `.gitignore`.

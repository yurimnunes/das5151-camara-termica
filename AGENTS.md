# AGENTS.md

Contexto para agentes de IA que trabalham neste repositório.

## Projeto em 5 linhas

Projeto final de DAS5151 Instrumentação em Controle (UFSC). Controle de temperatura de uma câmara de EPS (4 a 6 L) aquecida por 3 resistores de 15 Ω em paralelo, chaveados por PWM (Arduino D3 → PC817 → TIP122, 12 V, cerca de 24 W). Temperatura interna: 2 Pt100 a 3 fios em braços opostos de ponte de Wheatstone + amplificador de instrumentação de 3 amp-ops com LM324 (G ≈ 119, 44,4 mV/°C, faixa cerca de 3 a 76 °C) no A0. O INA125P do projeto original queimou e foi substituído em 2026-11-26. Temperatura ambiente: DS18B20 no D2, para feedforward. Setpoint até 50 °C, corte por software em 60 °C.

## Por onde começar

1. `README.md`: estrutura e índice
2. `docs/01-visao-geral.md` a `docs/05-pendencias.md`: resumos por tema
3. `docs/diario.md`: o que mudou e quando
4. `relatorio/README.md`: mapa do relatório (qual arquivo tem cada seção, figuras e labels) e regras para editar. **Leia antes de mexer no LaTeX** e abra só o arquivo da seção que precisa mudar.

## Fonte da verdade

* O relatório em `relatorio/` (`preambulo.tex` + `secoes/*.tex`) manda em valores de componentes e cálculos. Se os docs divergirem dele, o `.tex` vence e os docs devem ser corrigidos. **Exceção atual:** medição interna com LM324 (ver Armadilhas).
* Ao mudar a estrutura do relatório (seção, figura, label), atualizar a tabela do `relatorio/README.md` no mesmo commit.
* `firmware/teste_camara/teste_camara.ino` é o firmware de teste (sem malha fechada ainda).
* `firmware/teste_sensores/teste_sensores.ino` testa só os sensores (Pt100 e DS18B20), com o aquecedor desligado. Os testes de bancada são feitos por partes, seguindo `docs/06-testes-bancada.md`. Não pular partes sem o usuário pedir.
* Cada ensaio feito na bancada vira um arquivo em `docs/ensaios/NN-nome-AAAA-MM-DD.md` (configuração, log usado, resultados contra o esperado, interpretação, próximos passos), com o log `.tsv` de `dados/` commitado junto. Todo log novo entra no índice `dados/README.md` com o que cada canal media (os nomes das colunas do `teste_nos` são fixos e não dizem onde estavam os jumpers). Resumo rápido: `python3 scripts/resumo_log.py dados/<arquivo>.tsv`.

## Armadilhas conhecidas

* **Medição interna com LM324, relatório ainda com INA125P.** O INA125P queimou e foi trocado por um LM324 (detalhes em `docs/referencias/lm324.md`). Checklist da atualização e das medidas pendentes em `docs/plano-lm324.md`. Até o relatório ser atualizado, o `.tex` (seção 05, componentes e `esq_pt100_ina125.png`) ainda descreve o INA125P; nesse ponto os docs e o firmware estão à frente do relatório e valem eles.
* `lt_esq_acion.png`, `lt_formas_acion.png` e `lt_esq_pt100.png` são de uma versão antiga do circuito (24 V, 2 × 25 Ω, filtro 10 kΩ/10 µF). O texto do `.tex` já usa os valores finais. Detalhes em `docs/04-simulacoes.md`.
* Os arquivos `.asc` do LTspice (`simulacoes_ltspice_final.zip`) não estão no repositório.
* Tensão sobre a ponte: 192,3 mV a 0 °C (1,923 mA × 100 Ω), sobe para perto de 200 mV na faixa de operação. Não é um valor fixo.

## Comandos

Compilar o relatório sem sujar a pasta de fontes:

```sh
cd relatorio && latexmk -pdf -outdir=../build main.tex
```

Zip para o Overleaf (conteúdo do último commit; avisa se `relatorio/` tiver mudanças não commitadas): `sh scripts/overleaf-zip.sh` → `build/relatorio-overleaf.zip`. **Só rodar quando o usuário pedir explicitamente.** Nunca gerar o zip por conta própria depois de mexer no relatório.

Firmware e bancada (detalhes e instalação em `docs/03-firmware.md`):

```sh
arduino-cli compile firmware/teste_camara
arduino-cli upload -p /dev/ttyACM0 firmware/teste_camara
.venv/bin/python scripts/monitor_serial.py -n <nome-do-ensaio>   # rodar em segundo plano
echo 30 > build/serial_cmd                                       # PWM em %, com o monitor rodando
tail -n 20 dados/<arquivo>.tsv                                   # ler as últimas medidas
```

Regras de bancada para agentes:

* **Hardware real ligado em 12 V e 2,2 A.** Só mudar PWM, gravar firmware ou iniciar ensaio quando o usuário pedir.
* Antes de `upload`, parar o monitor (só um programa usa a porta).
* Parar o monitor com kill normal (SIGTERM), nunca `kill -9`: ele manda PWM 0 ao sair.
* Se o log mostrar `!! Protecao` ou temperatura subindo além do esperado, mandar `echo 0 > build/serial_cmd` e avisar o usuário.

## Convenções

* Docs e relatório em português. Decimais com vírgula no texto (`68,7 mV/°C`).
* Não usar travessão como pontuação em texto corrido.
* Commits com gitmoji (`:memo:` docs, `:sparkles:` novidade, `:bug:` correção). Sem linha de coautoria de IA.
* Ao concluir algo relevante: adicionar entrada datada no topo de `docs/diario.md` e marcar o item em `docs/05-pendencias.md`.
* Imagens usadas pelo `.tex` ficam versionadas em `relatorio/figuras/`. Não colocar PNG no `.gitignore`.
* Relatório sem caminhos `../` em `\input`/`\includegraphics`, para continuar compatível com o Overleaf.

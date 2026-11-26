# Câmara térmica com controle de temperatura

Projeto final de **DAS5151 Instrumentação em Controle** (UFSC, CTC, EAS).
Prof. Rodolfo Cesar Costa Flesch.

Equipe: Yuri Melo Nunes, Giulia Cenci dos Santos, Ana Luíza Luft.

Controle da temperatura de uma câmara de EPS aquecida por resistores via PWM. Temperatura interna medida por 2 Pt100 em ponte de Wheatstone com INA125P; temperatura ambiente medida por DS18B20 para compensação feedforward. Arduino Uno faz aquisição e gera o PWM.

## Estrutura

| Caminho | Conteúdo |
|---|---|
| `relatorio/` | Relatório em LaTeX, um arquivo por seção. Mapa em [`relatorio/README.md`](relatorio/README.md) |
| `scripts/overleaf-zip.sh` | Gera o zip do relatório para subir no Overleaf |
| `firmware/teste_camara/` | Firmware de teste dos módulos (sensores e PWM) |
| `firmware/teste_nos/` | Arduino como voltímetro: tensões dos nós da ponte e do INA nos pinos A0 a A5 |
| `firmware/teste_sensores/` | Firmware dos sensores (Pt100 + DS18B20), aquecedor desligado |
| `scripts/monitor_serial.py` | Monitor serial: mostra, grava em `dados/` e envia comandos |
| `dados/` | Logs dos ensaios (`.tsv`), com índice em [`dados/README.md`](dados/README.md) |
| `docs/` | Resumos e anotações do projeto |
| `AGENTS.md` / `CLAUDE.md` | Contexto e regras para agentes de IA |

## Documentação

1. [Visão geral](docs/01-visao-geral.md): objetivo, premissas, mudanças em relação à proposta
2. [Hardware](docs/02-hardware.md): componentes, circuitos, pinagem
3. [Firmware](docs/03-firmware.md): preparar o Linux, gravar no Uno, monitorar e o que o `teste_camara.ino` faz
4. [Simulações](docs/04-simulacoes.md): resultados do LTspice e status das figuras
5. [Pendências](docs/05-pendencias.md): o que falta e próximos passos
6. [Testes de bancada](docs/06-testes-bancada.md): roteiro por partes com valores esperados
   * Ensaios realizados: [`docs/ensaios/`](docs/ensaios/), um arquivo por ensaio com resultados e checklist
7. [Diário](docs/diario.md): registro datado do andamento

## Compilar o relatório

```sh
cd relatorio && latexmk -pdf -outdir=../build main.tex
```

PDF em `build/main.pdf`. Para o Overleaf, ver [`relatorio/README.md`](relatorio/README.md#levar-para-o-overleaf).

# Câmara térmica com controle de temperatura

Projeto final de **DAS5151 Instrumentação em Controle** (UFSC, CTC, EAS).
Prof. Rodolfo Cesar Costa Flesch.

Equipe: Yuri Melo Nunes, Giulia Cenci dos Santos, Ana Luíza Luft.

Controle da temperatura de uma câmara de EPS aquecida por resistores via PWM. Temperatura interna medida por 2 Pt100 em ponte de Wheatstone com INA125P; temperatura ambiente medida por DS18B20 para compensação feedforward. Arduino Uno faz aquisição e gera o PWM.

## Estrutura

| Caminho | Conteúdo |
|---|---|
| `Primeira_Entrega_Projeto_Final___Instrumentação/` | Relatório da primeira entrega em LaTeX (`main.tex`) e figuras |
| `teste_camara.ino` | Firmware de teste dos módulos (sensores e PWM) |
| `docs/` | Resumos e anotações do projeto |
| `AGENTS.md` / `CLAUDE.md` | Contexto e regras para agentes de IA |

## Documentação

1. [Visão geral](docs/01-visao-geral.md): objetivo, premissas, mudanças em relação à proposta
2. [Hardware](docs/02-hardware.md): componentes, circuitos, pinagem
3. [Firmware](docs/03-firmware.md): o que o `teste_camara.ino` faz e como usar
4. [Simulações](docs/04-simulacoes.md): resultados do LTspice e status das figuras
5. [Pendências](docs/05-pendencias.md): o que falta e próximos passos
6. [Diário](docs/diario.md): registro datado do andamento

## Compilar o relatório

```sh
cd Primeira_Entrega_Projeto_Final___Instrumentação
latexmk -pdf main.tex
```

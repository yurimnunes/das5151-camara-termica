# Diário do projeto

Entradas mais recentes no topo.

## 2026-11-25

* Ambiente Linux preparado: `arduino-cli` 1.6.0, core `arduino:avr` 1.8.8, OneWire 2.3.8, DallasTemperature 4.0.6. Sketch movido para `firmware/teste_camara/` e compilando (25 % da flash, 12 % da RAM).
* Criado `scripts/monitor_serial.py`: grava ensaios em `dados/` e aceita comandos pelo terminal ou por `build/serial_cmd`. Testado com Arduino simulado.
* Relatório reorganizado: pasta renomeada para `relatorio/`, preâmbulo e uma seção por arquivo em `secoes/`, imagens em `figuras/`. Removidas as 106 imagens de outros trabalhos (recuperáveis no commit `0a1c345`). PDF gerado idêntico ao anterior. Mapa das partes em `relatorio/README.md`.
* Criado `scripts/overleaf-zip.sh` para levar o relatório ao Overleaf.
* Conferida a tensão sobre a ponte: 192,3 mV a 0 °C, não 200 mV. Sem impacto: relatório e simulação já usam 1,92 mA, e a calibração absorve o ganho.
* Criados `AGENTS.md` e `CLAUDE.md` para dar contexto automático a agentes de IA.
* Repositório criado no GitHub (privado) com o relatório da primeira entrega e o firmware de teste.
* Relatório compila sem erros com `latexmk -pdf`.
* Encontrado: figuras de simulação do LTspice ainda são da versão antiga (24 V, 2 × 25 Ω, filtro 10 kΩ/10 µF). Texto já usa os valores finais.
* Documentação inicial escrita em `docs/`.

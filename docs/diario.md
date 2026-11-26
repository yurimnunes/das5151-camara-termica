# Diário do projeto

Entradas mais recentes no topo.

## 2026-11-26

* Diagnóstico da medição interna com o Arduino como voltímetro (`firmware/teste_nos`), detalhes no [ensaio 01](ensaios/01-sensores-2026-11-26.md):
  * Componente solto deixava a ponte sem 5 V.
  * Pt100 A ligado pelos dois fios do par (mesma ponta do sensor) aparecia como curto.
  * Fio do VIN+ estava no ponto do F.
  * Depois de corrigir: **ponte certa a 2 e a 3 fios, Vab cerca de +10 mV** (esperado +9,7 mV).
  * Em aberto: saída do INA125P parada em cerca de 0,06 V.
* Primeiro teste com o Uno R3 (`/dev/ttyACM0`), firmware `teste_sensores`, log `dados/2026-11-26_001655_sensores.tsv` (314 leituras, cerca de 5 min):
  * DS18B20 ok: detectado, 25,75 a 25,87 °C estável.
  * Pt100 sem sinal: A0 preso em 4,8 a 5,0 contagens (0,024 V), esperado cerca de 355 (1,74 V). Suspeitas: INA sem alimentação, VIN+/VIN− trocados (Vab negativo satura em 0 V), filtro mal ligado ou A0 desconectado. Circuito em revisão; sem multímetro no momento.
  * Esquemático conferido: polaridade correta (nó a no VIN+, Vab = +9,34 mV a 25 °C). Relatório e checklist em [`ensaios/01-sensores-2026-11-26.md`](ensaios/01-sensores-2026-11-26.md).
* Decidido testar por partes, começando pela medição interna. Criados `firmware/teste_ponte/` (só A0, D3 forçado em LOW) e o roteiro `docs/06-testes-bancada.md` com valores esperados de cada nó da ponte.
* Parte 1 passou a testar também o DS18B20 junto com a ponte: firmware renomeado para `firmware/teste_sensores/` (A0 + D2, D3 em LOW).

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

# Diário do projeto

Entradas mais recentes no topo.

## 2026-11-26

* **Medição interna com LM324 aprovada** ([ensaio 02](ensaios/02-lm324-2026-11-26.md), log `dados/2026-11-26_102800_lm324-1.tsv`, Serial Monitor da IDE em outro computador): A0 em 1,285 V parado (0,14 V acima do teórico, cerca de 3 °C de offset, dentro do típico), ruído de no máximo 1 contagem, e os dois Pt100 fazem a saída subir ao serem segurados (até 1,505 e 1,549 V). DS18B20 ok. **Parte 1 concluída.** Próximo: montar o acionamento (Parte 2) e calibrar o Pt100.
* **INA125P trocado por LM324.** O INA125P foi considerado queimado (alimentação, SLEEP, entradas, RG, IAREF e VREFCOM certos, mas sem resposta no pino 8 e VREFBG em 0,33 V; ver [ensaio 01](ensaios/01-sensores-2026-11-26.md)). Não havia outro no laboratório, só um LM324. Montado no lugar um amplificador de instrumentação de 3 amp-ops:
  * U1A e U1B com Rf = 2,2 kΩ e Rg = 1 kΩ (G1 = 5,4), U1C como subtrator com Rd1 = 1 kΩ e Rd2 = 22 kΩ (G2 = 22), U1D sem uso. Ganho total ≈ 119. O primeiro projeto usava 10 kΩ / 1,8 kΩ / 100 kΩ (G ≈ 121), mas não havia 10 kΩ no laboratório; escolhida a combinação com 1 kΩ, 2,2 kΩ e 22 kΩ.
  * Ganho reduzido de 186 para 119 porque a saída do LM324 em 5 V só vai até cerca de 3,5 V. Sensibilidade passa de 68,7 para 44,4 mV/°C; faixa de cerca de 3 a 76 °C.
  * Documentação em [`referencias/lm324.md`](referencias/lm324.md), esquema [`esq_pt100_lm324.png`](referencias/esq_pt100_lm324.png) e montagem pino a pino [`lm324-montagem.png`](referencias/lm324-montagem.png).
  * Firmwares `teste_sensores` e `teste_camara` atualizados para a reta nova (`SENS_V_POR_C` = 0,0444, `V_REF_25C` = 1,110). No `teste_camara`, limite de saturação de 4,3 para 3,4 V e nova proteção que desliga o aquecedor se o A0 ficar abaixo de 0,15 V (o sintoma do INA queimado, que a proteção por temperatura não pegava).
  * Relatório ainda descreve o INA125P: atualização registrada nas [pendências](05-pendencias.md).
* Diagnóstico da medição interna com o Arduino como voltímetro (`firmware/teste_nos`), detalhes no [ensaio 01](ensaios/01-sensores-2026-11-26.md):
  * Componente solto deixava a ponte sem 5 V.
  * Pt100 A ligado pelos dois fios do par (mesma ponta do sensor) aparecia como curto.
  * Fio do VIN+ estava no ponto do F.
  * Depois de corrigir: **ponte certa a 2 e a 3 fios, Vab cerca de +10 mV** (esperado +9,7 mV).
  * Em aberto: saída do INA125P parada em cerca de 0,06 V.
  * INA125P medido pino a pino: alimentação, SLEEP, entradas, RG, Sense, IAREF e VREFCOM certos, mas o pino 8 não segue o VIN+ e a referência interna (pino 13) fica em 0,33 V. **Decisão: trocar o INA125P.**
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

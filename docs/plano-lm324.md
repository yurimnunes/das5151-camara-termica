# Plano: troca do INA125P pelo LM324 (medidas e relatório)

Checklist para aplicar aos poucos. Marcar `[x]` ao concluir cada item e registrar a data no [diário](diario.md). Contexto da troca: [ensaio 01](ensaios/01-sensores-2026-11-26.md) (INA125P queimado), [ensaio 02](ensaios/02-lm324-2026-11-26.md) (LM324 aprovado) e [referencias/lm324.md](referencias/lm324.md) (circuito, pinagem e simulação).

## Impactos da troca (resumo)

| Aspecto | INA125P | LM324 | Efeito |
|---|---|---|---|
| Saída máxima | cerca de 4,2 V | cerca de 3,5 V (Vcc − 1,5 V) | ganho reduzido de 186 para 119 |
| Sensibilidade | 68,7 mV/°C | 44,4 mV/°C | resolução de 0,07 para 0,11 °C por contagem; suficiente para o controle (DS18B20 tem 0,06 °C) |
| Faixa | 12 a 61 °C | cerca de 3 a 76 °C | mais larga |
| Offset | baixo | até 7 mV por amp-op, multiplicado por 119 | **maior risco**. Medido +0,14 V (cerca de 3 °C); tirado pela calibração em 2 pontos |
| Rejeição de modo comum | interna, casada no CI | depende do casamento dos resistores de 1 % | erro fixo de algumas dezenas de mV, tirado pela calibração |
| Deriva térmica | baixa | maior | circuito fica fora da câmara; medir (item M5) |

Não mudam: ponte, ligação a 3 fios (a entrada + do amp-op não puxa corrente), filtro RC, autoaquecimento e o cancelamento das variações do 5 V (A/D e ponte usam o mesmo 5 V).

## Parte A: medidas na bancada

Firmware `teste_nos` = Arduino como voltímetro (A0 a A5); `teste_sensores` = A0 + DS18B20. Todo log entra no índice [`dados/README.md`](../dados/README.md) com o que cada canal media. Valores esperados para 5,00 V e 25 °C em [referencias/lm324.md](referencias/lm324.md#valores-esperados-alimentação-de-500-v).

- [ ] **M1. Ganho por estágio** (sem multímetro). `teste_nos` com A1 no pino 5 (nó a), A2 no pino 3 (nó b), A3 no pino 7, A4 no pino 1, A5 no pino 8. A coluna `a_b_mV` passa a ser pino 7 − pino 1.
  - Esperado a 25 °C: pino 7 ≈ 2,045 V, pino 1 ≈ 1,995 V (diferença ≈ 50 mV), pino 8 ≈ 1,110 V mais o offset.
  - (pino 7 − pino 1) / (pino 5 − pino 3) ≈ 5,4 e pino 8 / (pino 7 − pino 1) ≈ 22. Confirma Rf, Rg, Rd1 e Rd2.
  - Lembrete: pelo Arduino, Vab (cerca de 10 mV) tem resolução ruim; a razão do 1º estágio é só uma checagem grosseira.
- [ ] **M2. Offset só do amplificador** (sem multímetro). Tirar o fio do nó b do pino 3 e ligar o pino 3 ao pino 5 (as duas entradas no nó a). **Não ligar o nó a ao nó b na ponte.** Rodar `teste_sensores` e anotar o A0. Depois religar o pino 3 no nó b.
  - A0 em curto = offset do LM324 + erro de modo comum.
  - (A0 normal − A0 em curto) / 119 = Vab real. Comparar com o Vab teórico na temperatura do DS18B20 dá o desbalanço da ponte em °C.
  - Se o A0 em curto der cerca de 0 V, o offset é negativo e a saída travou embaixo. Registrar assim mesmo.
- [ ] **M3. Vab com multímetro** (quando houver multímetro). Escala de mV direto entre os nós a e b. Confere o M2 de forma independente.
- [ ] **M4. 5 V real.** Mudar um jumper do `teste_nos` (por exemplo A1) para o pino 3,3 V do Arduino. Vcc ≈ 3,3 × 5 / leitura em V. Serve para comparar as medidas com a teoria em volts.
- [ ] **M5. Log longo parado** (15 a 30 min, `teste_sensores`, câmara aberta, nada encostando nos sensores). Ruído e deriva do A0 depois de aquecer, comparados com o DS18B20.
- [ ] **M6. Calibração em 2 pontos.** Ponto baixo (água com gelo ou ambiente) e ponto alto (água morna perto de 50 °C), com termômetro de referência e os dois Pt100 juntos no mesmo banho.
  - Sensibilidade real contra os 44,4 mV/°C teóricos.
  - Confirmar que a saída não satura perto de 60 °C (A0 abaixo de 3,4 V).
  - Atualizar `SENS_V_POR_C` e `V_REF_25C` em `firmware/teste_sensores` e `firmware/teste_camara`.
- [ ] **M7. Tempo de resposta do Pt100.** Degrau em água morna, log com `teste_sensores`. Constante de tempo do sensor, útil para o modelo da planta.
- [ ] **M8. Ruído com o aquecedor (Parte 2).** Comparar o `ruido_cont` e o A0 com PWM em 0 % e em 50 a 80 %. O chaveamento de 2,2 A pode induzir ruído na medição.

## Parte B: relatório

### Antes de começar

- [ ] Saber se a primeira entrega já foi entregue ao professor. Se já foi, criar uma tag no git com a versão entregue antes de editar.
- [ ] Decidir a figura: usar [`esq_completo_lm324.png`](referencias/esq_completo_lm324.png) (bom, mas desenha cada Pt100 com 2 terminais e some a ligação a 3 fios) e explicar os 3 fios no texto, **ou** ajustar a figura para mostrar os 3 fios.

### Mudanças por seção

Regras de edição em [`relatorio/README.md`](../relatorio/README.md): figura nova em `relatorio/figuras/` com prefixo `esq_` ou `graf_`, e atualizar a tabela do mapa no mesmo commit.

- [ ] **Seção 1, descrição** (`secoes/01-descricao.tex`): novo item nas mudanças em relação à proposta. Troca do INA125P pelo LM324: o INA125P apresentou defeito no teste de bancada (referência interna em 0,33 V em vez de 1,24 V, saída sem resposta), não havia outro no LMM e foi montado um amplificador de 3 amp-ops com LM324. Tom objetivo. Ajustar o item atual que diz que o amplificador passou a ser o INA125P.
- [ ] **Seção 2, premissas** (`secoes/02-premissas.tex`): faixa de medição de cerca de 3 a 76 °C; o 5 V alimenta o LM324.
- [ ] **Seção 3, componentes** (`secoes/03-componentes.tex`): linha 6 passa a ser LM324 + Rf 2,2 kΩ (×2), Rg 1 kΩ, Rd1 1 kΩ (×2), Rd2 22 kΩ (×2), G ≈ 119; linha 17 com o desacoplamento do LM324 (100 nF entre os pinos 4 e 11).
- [ ] **Seção 5, medição interna** (`secoes/05-medicao-interna.tex`): reescrever a parte do amplificador.
  - Topologia de 3 amp-ops: U1A e U1B com G1 = 1 + 2 Rf / Rg = 5,4, U1C subtrator com G2 = Rd2 / Rd1 = 22, U1D sem uso.
  - Limite de saída de cerca de 3,5 V, que levou ao ganho 119; sensibilidade 44,4 mV/°C; resolução 0,11 °C por LSB.
  - Faixa de cerca de 3 a 76 °C.
  - Offset do LM324 e casamento dos resistores resolvidos pela calibração em 2 pontos.
  - Modo comum perto de 2 V, dentro da faixa de entrada do LM324 (0 a Vcc − 1,5 V).
  - Trocar a figura `esq_pt100_ina125.png` pela nova e tirar a descrição dos pinos do INA.
- [ ] **Seção 8, simulação** (`secoes/08-simulacao.tex`): trocar a simulação do INA (LTspice) pela do LM324 (ngspice, [`sim_lm324.png`](referencias/sim_lm324.png), netlist [`lm324_ponte.cir`](referencias/lm324_ponte.cir)). Refazer a tabela de Vab, Vout e contagens. As figuras `lt_esq_pt100`, `lt_vab_pt100` e `lt_vadc_pt100` deixam de ser usadas.
- [ ] **Opcional: validação em bancada.** Parágrafo curto com o ensaio 02: A0 de 1,285 V parado (offset de cerca de 3 °C), ruído de no máximo 1 contagem e os dois sensores respondendo no sentido certo.
- [ ] Compilar (`cd relatorio && latexmk -pdf -outdir=../build main.tex`) e conferir que não sobrou "INA125P" fora do histórico de mudanças: `grep -rn INA relatorio/secoes/`.
- [ ] Ao terminar: tirar a armadilha "Medição interna com LM324, relatório ainda com INA125P" do `AGENTS.md`, o aviso do topo de `docs/04-simulacoes.md` e o item correspondente em [05-pendencias.md](05-pendencias.md).

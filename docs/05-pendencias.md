# Pendências e próximos passos

## Primeira entrega

- [ ] Refazer simulações no LTspice com os componentes finais e trocar `lt_esq_acion.png`, `lt_formas_acion.png` e `lt_esq_pt100.png` (ver [Simulações](04-simulacoes.md#status-das-figuras))
- [ ] Remover o aviso em vermelho "[A FAZER ...]" do fim de `relatorio/secoes/07-acionamento.tex`
- [ ] Trazer para o repositório os arquivos `.asc` do LTspice (`simulacoes_ltspice_final.zip`)
- [ ] Atualizar o relatório para o LM324: seção 05 (medição interna), lista de componentes, novo esquemático no lugar de `esq_pt100_ina125.png` (rascunho em `docs/referencias/esq_pt100_lm324.png`) e simulação do circuito novo. Avisar o professor da troca

## Montagem e testes

Roteiro detalhado em [06-testes-bancada.md](06-testes-bancada.md).

- [x] Parte 1: sensores (ponte + amplificador com LM324 e DS18B20) com `firmware/teste_sensores` ([ensaio 02](ensaios/02-lm324-2026-11-26.md))
  - [x] DS18B20
  - [x] Ponte a 3 fios (Vab cerca de +10 mV)
  - [x] INA125P diagnosticado como queimado ([ensaio 01](ensaios/01-sensores-2026-11-26.md)). Sem outro no laboratório: substituído por amplificador de instrumentação com LM324 ([`referencias/lm324.md`](referencias/lm324.md))
  - [x] LM324 montado e testado: A0 = 1,285 V parado (offset de cerca de 3 °C), ruído ≤ 1 contagem, os dois Pt100 fazem a saída subir ([ensaio 02](ensaios/02-lm324-2026-11-26.md))
- [ ] Parte 2: acionamento
- [ ] Parte 3: sistema completo
- [ ] Confirmar o fio de terra da sonda DS18B20 com multímetro antes de energizar
- [ ] Medir VCE do TIP122 saturado; se passar de 2 V, trocar R1 para 330 Ω
- [ ] Calibração em 2 pontos do Pt100 com termômetro de referência e atualizar `SENS_V_POR_C` e `V_REF_25C`
- [ ] Ensaio de degrau em malha aberta para obter a condutância térmica real e o modelo da planta

## Próximas etapas do projeto

- [ ] Projeto do controlador (malha fechada) com compensação feedforward da temperatura ambiente
- [ ] Ensaios de perturbação usando a portinhola e a segunda ventoinha
- [ ] Reavaliar a medição de corrente do aquecedor

## Organização do repositório

- [x] Remover as 106 imagens que não eram do projeto e separar o relatório em um arquivo por seção (`relatorio/`)

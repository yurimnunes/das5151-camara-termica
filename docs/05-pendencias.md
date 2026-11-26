# Pendências e próximos passos

## Primeira entrega

- [ ] Refazer simulações no LTspice com os componentes finais e trocar `lt_esq_acion.png`, `lt_formas_acion.png` e `lt_esq_pt100.png` (ver [Simulações](04-simulacoes.md#status-das-figuras))
- [ ] Remover o aviso em vermelho "[A FAZER ...]" do fim de `relatorio/secoes/07-acionamento.tex`
- [ ] Trazer para o repositório os arquivos `.asc` do LTspice (`simulacoes_ltspice_final.zip`)

## Montagem e testes

Roteiro detalhado em [06-testes-bancada.md](06-testes-bancada.md).

- [ ] Parte 1: sensores (ponte + INA125P e DS18B20) com `firmware/teste_sensores`
  - [x] DS18B20
  - [x] Ponte a 3 fios (Vab cerca de +10 mV)
  - [ ] **Trocar o INA125P** (o atual não opera; ver [ensaio 01](ensaios/01-sensores-2026-11-26.md)) e medir de novo com `firmware/teste_nos`
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

# Pendências e próximos passos

## Primeira entrega

- [ ] Refazer simulações no LTspice com os componentes finais e trocar `lt_esq_acion.png`, `lt_formas_acion.png` e `lt_esq_pt100.png` (ver [Simulações](04-simulacoes.md#status-das-figuras))
- [ ] Remover o aviso em vermelho "[A FAZER ...]" do fim da Seção 7 do `main.tex`
- [ ] Trazer para o repositório os arquivos `.asc` do LTspice (`simulacoes_ltspice_final.zip`)

## Montagem e testes

- [ ] Confirmar o fio de terra da sonda DS18B20 com multímetro antes de energizar
- [ ] Medir VCE do TIP122 saturado; se passar de 2 V, trocar R1 para 330 Ω
- [ ] Calibração em 2 pontos do Pt100 com termômetro de referência e atualizar `SENS_V_POR_C` e `V_REF_25C`
- [ ] Ensaio de degrau em malha aberta para obter a condutância térmica real e o modelo da planta

## Próximas etapas do projeto

- [ ] Projeto do controlador (malha fechada) com compensação feedforward da temperatura ambiente
- [ ] Ensaios de perturbação usando a portinhola e a segunda ventoinha
- [ ] Reavaliar a medição de corrente do aquecedor

## Organização do repositório

- A pasta do relatório tem 116 PNGs mas o `main.tex` usa só 10. As outras 106 (`fig*`, `ex*-m=*`, `bif*`, `lr_*`, `pi_*`, `controlep*` etc.) parecem sobras de relatórios de outras disciplinas. Avaliar mover para fora do projeto.

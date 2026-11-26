# Relatório (LaTeX)

Relatório do projeto, dividido em um arquivo por seção. Para mudar uma seção, edite só o arquivo dela.

## Estrutura

```
relatorio/
  main.tex        documento raiz: classe, preâmbulo e ordem das seções
  preambulo.tex   pacotes, margens, cabeçalho, \graphicspath, espaçamento 1,5
  secoes/         um arquivo por seção (tabela abaixo)
  figuras/        todas as imagens usadas no relatório
```

## Mapa das seções

| Seção | Arquivo | Conteúdo | Figuras | Labels definidos |
|---|---|---|---|---|
| Capa | `secoes/00-capa.tex` | Capa (UFSC, autores, professor) e sumário | `logoUFSCsimples` | |
| 1 | `secoes/01-descricao.tex` | O que é o projeto e mudanças em relação à proposta | | |
| 2 | `secoes/02-premissas.tex` | Faixa de setpoint, faixa de medição, câmara, Arduino, fonte | | |
| 3 | `secoes/03-componentes.tex` | Tabela com os 19 componentes | | `tab:componentes` |
| 4 | `secoes/04-aquecedor.tex` | Resistência equivalente, corrente, potência, balanço térmico | | |
| 5 | `secoes/05-medicao-interna.tex` | Ponte com 2 Pt100, INA125P, ganho, faixa, 3 fios, calibração, filtro RC | `esq_pt100_ina125` | `sec:pt100`, `fig:esq_pt100` |
| 6 | `secoes/06-temperatura-ambiente.tex` | DS18B20: ligação, características, posicionamento | `esq_ds18b20` | `fig:esq_ds18b20` |
| 7 | `secoes/07-acionamento.tex` | PC817, TIP122, dissipação, montagem de potência, proteções. Tem o aviso "[A FAZER]" em vermelho | `esq_acionamento` | `sec:acionamento`, `fig:esq_acion` |
| 8 | `secoes/08-simulacao.tex` | Simulações LTspice: medição interna (tabela) e acionamento | `lt_esq_pt100`, `lt_vab_pt100`, `lt_vadc_pt100`, `lt_esq_acion`, `lt_formas_acion` | `sec:simulacao`, `fig:lt_esq_pt100`, `fig:lt_sim_pt100`, `tab:sim_pt100`, `fig:lt_esq_acion`, `fig:lt_formas_acion` |
| 9 | `secoes/09-mecanica.tex` | Esboço mecânico: câmara, aquecedor, ventoinha, sensores, portinhola | `esboco_mecanico` | `fig:mecanica` |

Referências entre arquivos: a Seção 1 cita `sec:pt100` (Seção 5), a 4 cita `sec:acionamento` (Seção 7) e a 5 cita `sec:simulacao` (Seção 8). Ao renomear um label, procurar em todos os arquivos: `grep -rn 'nome-do-label' secoes/`.

## Regras

* **Mapa sempre atualizado:** ao criar, renomear ou apagar uma seção, figura ou label, atualizar a tabela acima no mesmo commit.
* **Nova seção:** criar `secoes/NN-nome.tex` (número na ordem, nome sem acento), começar com `% Seção N: Título` e adicionar `\input{secoes/NN-nome}` no `main.tex`. Para inserir no meio, renumerar os arquivos seguintes com `git mv` no mesmo commit.
* **Nova figura:** salvar em `figuras/` sem acento e sem espaço, com prefixo pelo tipo: `esq_` (esquemático), `lt_` (LTspice), `graf_` (gráfico de dados), `foto_` (foto da montagem). Incluir só pelo nome, sem pasta: `\includegraphics{esq_exemplo.png}`.
* **Labels:** `sec:`, `fig:`, `tab:`, `eq:` + nome curto.
* **Pacotes:** só no `preambulo.tex`.
* Sem caminhos com `../`, para o projeto funcionar no Overleaf.

## Compilar localmente

Da raiz do repositório:

```sh
cd relatorio && latexmk -pdf -outdir=../build main.tex
```

O PDF sai em `build/main.pdf`, fora da pasta do relatório.

## Levar para o Overleaf

O GitHub é a fonte oficial. O Overleaf serve para quem prefere editar ou visualizar por lá. O zip é gerado só quando alguém pede, não a cada mudança no relatório.

1. Na raiz do repositório, rodar `sh scripts/overleaf-zip.sh`. Ele gera `build/relatorio-overleaf.zip` com o `relatorio/` do **último commit**. Mudanças ainda não commitadas ficam de fora, e o script avisa quando houver alguma. Sem o script, dá no mesmo: `git archive --format=zip -o relatorio-overleaf.zip HEAD:relatorio`.
2. No Overleaf: *New Project → Upload Project* e escolher o zip. O Overleaf detecta o `main.tex` e compila direto.

**Trazer edições do Overleaf de volta:**

1. No Overleaf, *Menu → Download → Source*.
2. Apagar o conteúdo de `relatorio/` (o git guarda tudo) e descompactar o zip ali. Assim, arquivos apagados ou renomeados no Overleaf também somem aqui.
3. Conferir com `git status` (arquivos novos ou apagados) e `git diff` (mudanças), depois `git add relatorio` e commit.

O Overleaf não sincroniza sozinho com o GitHub (só no plano pago). Por isso, combinem quem está editando onde, para não sobrescrever o trabalho de outra pessoa.

# Reorganizar relatório LaTeX Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Status:** concluído (commits `bbeb516` a `4d6e736` e correções da revisão final). A pasta antiga citada abaixo já foi renomeada para `relatorio/`.


**Goal:** Reorganizar o relatório em `relatorio/` com preâmbulo, uma seção por arquivo e figuras em `figuras/`, documentar cada parte em um `.md`, sem mudar nada no PDF gerado.

**Architecture:** `relatorio/main.tex` fica só com `\documentclass`, `\input{preambulo}` e a lista de `\input{secoes/...}`. Cada `\section` do `main.tex` atual vira um arquivo em `relatorio/secoes/`. As 10 imagens usadas vão para `relatorio/figuras/`; as 106 não usadas saem do repo (recuperáveis no commit `0a1c345`). Estrutura compatível com Overleaf: `main.tex` na raiz da pasta, caminhos relativos a ela.

**Tech Stack:** LaTeX (pdflatex, latexmk), git, pdftotext/pdfinfo para verificação.

**Spec:** pedido do usuário nesta conversa + respostas: remover imagens não usadas, manter compatível com Overleaf, renomear pasta para `relatorio/`.

## Global Constraints

- Conteúdo do PDF idêntico ao atual: mesmo texto extraído por `pdftotext` (exceto a data de `\today`) e mesmo número de páginas.
- `relatorio/main.tex` continua sendo o documento raiz (Overleaf usa `main.tex` como padrão).
- Caminhos de `\input` e `\includegraphics` relativos a `relatorio/`, sem caminho absoluto.
- Nomes de arquivos sem acento e sem espaço.
- Docs em português, sem travessão como pontuação, commits com gitmoji e sem coautoria de IA.
- `build/` continua no `.gitignore`; nunca compilar dentro de `relatorio/`.

## Review Focus

- Label usado em uma seção e definido em outra (`sec:pt100`, `sec:acionamento`, `sec:simulacao`): referências devem continuar resolvidas, sem `??` no PDF.
- Overleaf: upload da pasta `relatorio/` inteira deve compilar sem ajuste (sem `\input` com `../`).
- Linha do aviso vermelho "[A FAZER ...]" do fim da Seção 7 deve continuar no arquivo da Seção 7.
- `\tcbset` e `\clearpage`/`\tableofcontents` que ficam entre a capa e a Seção 1 não podem se perder no corte.
- Links nos docs (`docs/*.md`, `AGENTS.md`, `README.md`) para a pasta antiga devem ser todos atualizados.

---

## Estrutura alvo

```
relatorio/
  main.tex              documento raiz: classe, preâmbulo, ordem das seções
  preambulo.tex         pacotes e configurações (linhas 2 a 54 do main.tex atual)
  README.md             mapa: seção → arquivo → conteúdo → figuras → labels
  secoes/
    00-capa.tex         capa, sumário, \tcbset (linhas 57 a 90)
    01-descricao.tex    linhas 92 a 105
    02-premissas.tex    linhas 106 a 116
    03-componentes.tex  linhas 117 a 155
    04-aquecedor.tex    linhas 156 a 172
    05-medicao-interna.tex     linhas 173 a 215
    06-temperatura-ambiente.tex linhas 216 a 233
    07-acionamento.tex  linhas 234 a 266
    08-simulacao.tex    linhas 267 a 355
    09-mecanica.tex     linhas 356 a 376
  figuras/              as 10 imagens usadas
```

---

### Task 1: Gerar PDF de referência

**Files:** nenhum no repo; saída em `build/ref/`.

- [ ] **Step 1:** `cd "Primeira_Entrega_Projeto_Final___Instrumentação" && latexmk -pdf -interaction=nonstopmode -outdir=../build/ref main.tex`
- [ ] **Step 2:** `pdftotext -layout build/ref/main.pdf build/ref/main.txt && pdfinfo build/ref/main.pdf | grep Pages`. Guardar o número de páginas.

### Task 2: Mover pasta, limpar imagens, separar figuras

**Files:**
- Rename: `Primeira_Entrega_Projeto_Final___Instrumentação/` → `relatorio/`
- Move: as 10 imagens usadas → `relatorio/figuras/` (`logoUFSCsimples`, `esq_pt100_ina125`, `esq_ds18b20`, `esq_acionamento`, `lt_esq_pt100`, `lt_vab_pt100`, `lt_vadc_pt100`, `lt_esq_acion`, `lt_formas_acion`, `esboco_mecanico`, todas `.png`)
- Delete: os 106 PNGs restantes com `git rm`

- [ ] **Step 1:** `git mv` da pasta, `git mv` das 10 imagens para `figuras/`, `git rm` do resto.
- [ ] **Step 2:** `ls relatorio/*.png` não retorna nada; `ls relatorio/figuras | wc -l` = 10.
- [ ] **Step 3:** Compilar (`latexmk -pdf -outdir=../build relatorio/main.tex` a partir de `relatorio/`): sem erro, pois `\graphicspath{{figuras/}{./}}` já existe.
- [ ] **Step 4:** Commit `:truck: Move report to relatorio/ and figures to figuras/` + `:fire:` no corpo citando remoção das 106 imagens não usadas.

### Task 3: Separar o documento em preâmbulo e seções

**Files:**
- Create: `relatorio/preambulo.tex`, `relatorio/secoes/00-capa.tex` a `09-mecanica.tex` (cortes da tabela acima)
- Modify: `relatorio/main.tex` passa a ter só:

```latex
\documentclass[a4paper,11pt]{article}
\input{preambulo}

\begin{document}
\input{secoes/00-capa}
\input{secoes/01-descricao}
...
\input{secoes/09-mecanica}
\end{document}
```

- [ ] **Step 1:** Cortar o texto exatamente nas linhas indicadas (por script, sem reescrever conteúdo). Cada arquivo de seção começa com uma linha de comentário `% Seção N: <título>`; remover os separadores `% ===== N =====`.
- [ ] **Step 2:** Compilar em `build/` e verificar: `grep -c '^! ' build/main.log` = 0, `grep 'undefined' build/main.log` vazio.
- [ ] **Step 3:** `pdftotext -layout build/main.pdf build/main.txt && diff <(grep -v Florianópolis build/ref/main.txt) <(grep -v Florianópolis build/main.txt)` sem diferenças; páginas iguais ao Task 1.
- [ ] **Step 4:** Commit `:recycle: Split report into preamble and one file per section`.

### Task 4: Documentar as partes e atualizar referências

**Files:**
- Create: `relatorio/README.md` com: como compilar (local e Overleaf), árvore de arquivos, tabela `Seção | Arquivo | Conteúdo | Figuras | Labels definidos/usados`, regras (uma seção por arquivo, figuras novas em `figuras/` com prefixo `esq_`, `lt_`, `foto_`, `graf_`; nova seção = novo arquivo numerado + `\input` no `main.tex`).
- Modify: `AGENTS.md` (caminhos, comando de build, apontar para `relatorio/README.md`, remover armadilha das 106 imagens), `README.md` (tabela de estrutura, comando de build), `docs/02-hardware.md` e `docs/04-simulacoes.md` (links de imagens para `../relatorio/figuras/`), `docs/05-pendencias.md` (marcar item de organização como feito), `docs/diario.md` (entrada nova), `.gitignore` (sem mudança esperada; conferir).

- Create: `scripts/overleaf-zip.sh`: gera `build/relatorio-overleaf.zip` com o conteúdo de `relatorio/` na raiz do zip (sem a pasta `relatorio/` em volta), pronto para *New Project → Upload Project*. Documentar o fluxo GitHub → Overleaf e o caminho de volta (Download Source) no `relatorio/README.md`.

- [ ] **Step 1:** Escrever os arquivos.
- [ ] **Step 1b:** Rodar o script, extrair o zip em pasta temporária e compilar lá com `latexmk`: sem erro.
- [ ] **Step 2:** `grep -rn 'Primeira_Entrega' --include=*.md .` não retorna nada fora de `docs/superpowers/` e do `diario.md` histórico.
- [ ] **Step 3:** Todo link relativo nos `.md` aponta para arquivo existente (script simples que extrai `](...)` e testa com `test -e`).
- [ ] **Step 4:** Commit `:memo: Document report structure for humans and agents`.

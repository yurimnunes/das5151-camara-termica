# Decisão pendente: montagem mecânica da câmara

**Status:** em aberto. Por enquanto vale o que está no relatório (`relatorio/secoes/09-mecanica.tex`) e em [02-hardware.md](02-hardware.md#estrutura-mecânica). Decidir antes de montar a câmara (Parte 3).

Um resumo antigo de uma colega (18/11) descreve uma montagem diferente da que está no relatório:

| Item | Relatório (atual) | Resumo de 18/11 |
|---|---|---|
| Ventoinha | em pé junto a uma parede lateral, soprando sobre a placa | deitada no fundo, logo abaixo da placa, soprando para cima |
| Caixa | 4 a 6 L (cerca de 20 × 15 × 15 cm) | cerca de 25 × 20 × 20 cm internos (cerca de 10 L), a ajustar à caixa real |
| Placa de resistores | suspensa por suporte metálico preso à tampa, sem tocar o EPS | suspensa pela tampa, a pelo menos 5 cm dela e longe das paredes |
| Pt100 | entram pela parede oposta à ventoinha, a alguns centímetros da placa | entram pela parede lateral a meia altura, fora da coluna de ar quente |

Iguais nas duas versões: tampa removível, passagens de cabos vedadas com cola quente, local para a portinhola, e DS18B20, Arduino, protoboard, TIP122 e fonte fora da caixa.

## O que considerar

* **Caixa real:** o resumo cita a caixa de isopor do LIN (uso a confirmar). As dimensões dela decidem o volume. Uma caixa maior muda o balanço térmico da seção 4 do relatório: mais área de parede, mais perda e aquecimento mais lento com os mesmos cerca de 24 W.
* **Ventoinha:** soprando direto na placa tira o calor dos resistores mais rápido. Deitada no fundo cria uma coluna de ar quente subindo, e aí os Pt100 precisam ficar fora dela.
* **Pt100:** precisam medir o ar médio da câmara, não o jato de ar quente que sai da placa.

## Ao decidir

* Atualizar `relatorio/secoes/09-mecanica.tex` e a figura `esboco_mecanico.png`.
* Se o volume mudar, refazer o balanço térmico em `relatorio/secoes/04-aquecedor.tex`.
* Atualizar [02-hardware.md](02-hardware.md#estrutura-mecânica) e [01-visao-geral.md](01-visao-geral.md).
* Marcar o item em [05-pendencias.md](05-pendencias.md) e apagar este arquivo, ou trocar o status para "decidido".

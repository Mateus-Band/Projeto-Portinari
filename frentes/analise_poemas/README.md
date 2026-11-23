# Pipeline de análise lexical e morfológica de poemas

Etiqueta cada palavra de um corpus de poemas com um modelo de PLN (spaCy, Stanza, UDPipe 2, Trankit; na fase 2, um BERTimbau ajustado) em um nível de contexto (palavra isolada, verso ou poema inteiro), salva o resultado em CSV e gera, **sem rodar modelo nenhum**, visualizações lexicais (quais palavras dominam) e morfológicas (quais classes e formas gramaticais dominam).

```
projeto/
├── dados/
│   ├── bruto/        CSV original (somente leitura). Coloque aqui poemas.csv
│   ├── analise/      saídas do notebook 01
│   └── anotado/      (fase 2) amostra anotada manualmente
├── src/
│   ├── carga.py            leitura e limpeza do CSV; leitura dos CSVs de tokens
│   ├── preprocessamento.py separação de versos, normalização de maiúsculas, tokens comuns
│   ├── alinhamento.py      alinhamento dos tokens de cada modelo aos tokens comuns
│   ├── tokenizacao.py      orquestra o notebook 01 (contextos, CSVs, registro, testes de consistência)
│   ├── modelos/            um módulo por modelo, com a mesma interface
│   ├── stopwords.py        lista única de stopwords (congelada em stopwords_pt.txt)
│   ├── metricas.py         métricas, frequências, contagens morfológicas
│   └── graficos.py         funções de figura
├── notebooks/  01_tokenizacao · 02_lexical · 03_morfologica
├── saida/{lexical|morfologica}/{modelo}_{contexto}/
├── testes/     47 testes automáticos (pytest)
└── requirements*.txt
```

Nenhuma função de análise é definida em notebook: eles só definem parâmetros, chamam `src/` e exibem resultados.

## Como usar

1. `pip install -r requirements.txt` e, para o modelo escolhido, o `requirements-<modelo>.txt` (spaCy: depois `python -m spacy download pt_core_news_lg`).
2. Copie o CSV para `dados/bruto/poemas.csv` (colunas usadas: `estrofes`, com os versos separados por `/`, e `id`; as demais são ignoradas). Para experimentar, use `dados/bruto/poemas_exemplo.csv` (um poema real de exemplo e quatro sintéticos) mudando `ARQUIVO_CSV`.
3. **Notebook 01**: defina `MODELO` e `CONTEXTO` e rode. Repita para cada modelo e contexto desejados.
4. **Notebooks 02 e 03**: defina os mesmos `MODELO` e `CONTEXTO` e rode. Se o CSV do notebook 01 não existir, param com uma mensagem pedindo para rodá-lo com esses valores.

Os notebooks funcionam rodando de `projeto/` ou de `projeto/notebooks/`.

## Hiperparâmetros

| Parâmetro | Valores | Notebook |
|---|---|---|
| `MODELO` | `spacy`, `stanza`, `udpipe2`, `trankit`, `bertimbau_ft` (fase 2) | 1, 2, 3 |
| `CONTEXTO` | `token`, `verso`, `poema` | 1, 2, 3 |
| `NORMALIZAR_MAIUSCULAS` | `True` (padrão) / `False` | 1 |
| `LEMAS` | `True` / `False` | 2 |
| `SEM_STOPWORDS` | `True` / `False` | 2 |
| `TOP` | inteiro (padrão 50) | 2, 3 |
| `MIN_OCORRENCIAS` | inteiro (padrão 10) | 3 |
| `SEMENTE` | inteiro (padrão 42) | 2 |

Parâmetros auxiliares: `ARQUIVO_CSV`, `CONFIG_MODELOS` (pacote do spaCy, modelo e acesso do UDPipe 2 etc.), `RODAR_TESTE_NORMALIZACAO` (notebook 1); `ARQUIVO_CSV` (só para caracteres e sentenças) e `FONTE_NUVEM` (notebook 2).

`SEM_STOPWORDS` (padrão `True`) vale para os rankings (top-`TOP` e top 10 por classe); as tabelas de frequência são sempre gravadas com e sem stopwords, e a nuvem usa sempre palavras de conteúdo.

## Arquivos gerados

**Notebook 01**, em `dados/analise/`:

- `tokens_{contexto}_{modelo}.csv`: uma linha por token, colunas `poema_id, verso_idx, token_idx, forma, lema, upos, morph, modelo, contexto`. O que o modelo não fornece fica vazio. Não há marcação de stopword.
- `lemas_{contexto}_{modelo}.csv`: `forma, upos, lema, n` (auditoria; só para modelos com lema).
- `registro.csv`: uma linha por execução (data, modelo, versão exata, acesso local ou online, contexto, normalização, nº de poemas, tokens, reagrupamentos e divergências de alinhamento). É um histórico: cresce a cada execução.
- `divergencias_{contexto}_{modelo}.csv`: tokens com divergência de alinhamento (só existe se houver alguma).

**Notebook 02**, em `saida/lexical/{modelo}_{contexto}/`: `metricas_por_poema.csv`, `frequencias_{todas|conteudo}_{lemas|formas}.csv`, top-`TOP` (barras com `freq` e `df`), nuvem de palavras, top 10 por classe (NOUN, VERB, ADJ, ADV) e painel, curva de Zipf, proporção de hapax e comparação formas × lemas. Os arquivos que dependem da unidade levam o sufixo `_lemas` ou `_formas`.

**Notebook 03**, em `saida/morfologica/{modelo}_{contexto}/`: distribuição de UPOS, tabela classe × traço × valor, gráficos de traços por classe (VERB, AUX, NOUN, ADJ, PRON, DET), figura de verbos (VerbForm, Tense, Mood) e top traços agregados.

## Decisões de projeto

- **Token comum**: sequência de letras, com hífen ou apóstrofo internos ("bem-estar", "d'água"). O modelo recebe a pontuação (ela ajuda a delimitar frases); pontuação e números em dígitos são descartados depois da classificação.
- **Contexto**: `poema` entrega o poema inteiro com os versos unidos por **um espaço** (nunca quebra de linha); `verso`, cada verso; `token`, cada palavra isolada (cada forma distinta é analisada uma vez). O `/` é removido antes do modelo; o nº do verso de cada token vai em `verso_idx`.
- **Normalização de maiúsculas**: o primeiro verso e os versos que abrem uma frase ficam como estão (o verso anterior termina em `.`, `!`, `?` ou `…`, aceitando aspas e parênteses depois); nos demais, a maiúscula inicial é rebaixada, exceto para o **conjunto protegido** (palavras que aparecem com maiúscula no meio de algum verso do corpus, como "Emily"). Maiúsculas no meio do verso nunca mudam. Vale para os três contextos.
- **Alinhamento** pela posição dos caracteres. Várias palavras do modelo cobrindo exatamente um token são **reagrupadas** (contrações como "do" = "de" + "o"; clíticos; palavras compostas partidas pelo tokenizador, sem o hífen): lema = componentes unidos por espaço, classe e traços = os da primeira componente. Limites que não coincidem são **divergências**, contadas no registro (usa-se a palavra de maior sobreposição).
- **Lema**: sempre em minúsculas (inclusive nomes próprios). Contrações ficam como um token ("do" → lema "de o"); na análise lexical com lemas, as componentes são separadas antes de contar, e no top por classe só a componente principal conta.
- **Stopwords**: lista única (própria ∪ spaCy), congelada em `src/stopwords_pt.txt` (446 palavras), aplicada à unidade contada, nunca antes do modelo. Regenerar: `python -m src.stopwords`.
- **Morfologia**: sem filtro de stopwords; a unidade é a ocorrência do token, não o lema. Proporções por classe são calculadas antes de descartar combinações com menos de `MIN_OCORRENCIAS`.
- **Validação de classes**: etiquetas fora das 17 UPOS viram `X`.

## Estado da implementação e dos testes

| Modelo | Implementado | Testado com o modelo real |
|---|---|---|
| `spacy` (`pt_core_news_lg`/`md`/`sm`) | sim | **sim** (3 contextos, notebooks 01 a 03) |
| `stanza` | sim | **não** |
| `udpipe2` (serviço REST) | sim; acesso local **não implementado** | **não** |
| `trankit` | sim | **não** |
| `bertimbau_ft` | não (fase 2): falha com mensagem explicativa | não se aplica |

Stanza, UDPipe 2 e Trankit foram escritos a partir da documentação, mas o ambiente de desenvolvimento não alcança os servidores de modelos (Hugging Face, LINDAT). Foram testados **a lógica de alinhamento e reagrupamento de contrações, o analisador de CoNLL-U do UDPipe 2 e o pipeline completo com um modelo simulado** que separa contrações como esses modelos. Primeiros pontos a conferir ao rodá-los de verdade:

- **Stanza**: se `bulk_process` aceita a lista de textos na sua versão (há um recuo para chamadas individuais); se as posições (`start_char`/`end_char`) coincidem com o texto enviado.
- **UDPipe 2**: o nome do modelo (`MODELO_PADRAO`; use `listar_modelos()` em `src/modelos/udpipe2.py`); se a opção `tokenizer=ranges` devolve `TokenRange` no campo MISC (sem ela, o analisador procura cada forma em sequência no texto); e, no modo `lote="bloco"`, se a linha em branco isola mesmo os textos (se não, use `lote="individual"`).
- **Trankit**: se instala, se `dspan` é (início, fim) exclusivo (o código confere e corrige) e se o projeto ainda é mantido.

### Critérios de aceitação

| # | Critério | Situação |
|---|---|---|
| 1 | Os três contextos geram CSVs distintos com o mesmo conjunto de tokens | Verificado (spaCy real e modelo simulado; `conferir_consistencia`) |
| 2 | Mesmo nº de tokens por poema em todos os modelos; divergências contadas | Verificado para spaCy e para o modelo simulado com contrações separadas. **Pendente** para Stanza, UDPipe 2 e Trankit reais |
| 3 | Notebooks 2 e 3 sem importar spaCy, Stanza, UDPipe, Trankit ou transformers | Verificado (teste automático e execução dos notebooks) |
| 4 | `LEMAS = True` sem lema e notebook 3 sem traços não quebram | Verificado com um CSV sem lema e sem traços (simula `bertimbau_ft`): avisam e pulam |
| 5 | Duas execuções com os mesmos parâmetros geram arquivos idênticos | Verificado: 95 arquivos (CSVs e PNGs) idênticos; `registro.csv` é um histórico e cresce de propósito. Com UDPipe 2 online vale só enquanto o modelo no servidor não mudar (o registro guarda a versão) |
| 6 | Teste da normalização | Executado em amostra pequena (ver abaixo); **precisa ser repetido no corpus completo** |
| 7 | Contagens do contexto `poema` com spaCy reproduzíveis a partir do CSV | Verificado: os notebooks 2 e 3 só leem o CSV |

### Teste da normalização (critério 6): resultado provisório

Rodado com spaCy `pt_core_news_lg`, em `dados/bruto/poemas_exemplo.csv` (218 palavras: 1 poema real e 3 sintéticos), proporção de PROPN e palavras cuja classe mudou:

| Contexto | PROPN com normalização | PROPN sem | Palavras que mudaram de classe |
|---|---|---|---|
| `poema` | 2,3% | 6,4% | 10 (4,6%) |
| `verso` | 2,3% | 4,6% | 6 (2,8%) |
| `token` | 2,8% | 4,6% | 5 (2,3%) |

Com o `pt_core_news_sm` o padrão é o mesmo (poema 3,7% × 7,3%; verso 3,7% × 5,5%; token 5,0% × 6,9%). As palavras que mudam são, em quase todos os casos, palavras comuns que ficavam como PROPN apenas por abrirem um verso ("transitavam", "cafeeira", "companhia", "futuro", "procissões"). O efeito é maior no contexto `poema`, mas aparece também em `verso` e `token`, em que a palavra isolada ainda chega com a maiúscula.

**Padrão provisório: `NORMALIZAR_MAIUSCULAS = True` nos três contextos.** Antes de fixá-lo, repita o teste no corpus completo (`RODAR_TESTE_NORMALIZACAO = True` no notebook 01) e inclua a conferência manual de uma amostra, principalmente das palavras que **deixam** de ser PROPN: o teste não mede quantas delas eram nomes próprios de verdade.

## Pontos em aberto

- **Gabarito**: haverá amostra anotada à mão (`dados/anotado/`)? Sem ela, a comparação de modelos (fase seguinte) mede só concordância, não acurácia.
- **Contrações**: a regra "classe e traços da primeira componente" é uma proposta a confirmar com Stanza, UDPipe 2 e Trankit reais (ex.: "dela" = "de" + "ela" fica ADP).
- **UDPipe 2**: serviço online (simples, mas envia os textos para fora e depende da versão do servidor) ou instalação local (não implementada; exige o repositório do UDPipe 2 e GPU).
- **Trankit**: confirmar se instala no ambiente e se ainda é mantido antes de investir nele.
- **Padrão de `NORMALIZAR_MAIUSCULAS` por contexto**: definir após o teste no corpus completo.

## Fases seguintes (fora deste escopo)

- Notebook 4: comparação de modelos (concordância por token, matriz de confusão de UPOS, divergência de lemas, sobreposição dos rankings) e, havendo gabarito, acurácia por classe. Os CSVs já guardam tudo o que ele precisa; `conferir_consistencia` garante a mesma base de tokens.
- Fine-tuning do BERTimbau (`bertimbau_ft`) em corpus UD do português, avaliado na amostra anotada.

## Desvios e acréscimos em relação ao enunciado

- Módulo extra `src/tokenizacao.py` (orquestração do notebook 01), porque nenhuma função de análise pode ficar em notebook.
- Arquivo extra `divergencias_{contexto}_{modelo}.csv` e pasta `testes/` (47 testes).
- Não implementados: UDPipe 2 local; análises "opcionais" do notebook 02 (mapa de calor lema × poema, lemas distintivos por grupo de metadados, coocorrência).
- O notebook 02 lê também o CSV bruto, só para número de caracteres e de sentenças por poema.

## Ambiente em que foi testado

Python 3.12; pandas 3.0, numpy 2.4, matplotlib 3.10, wordcloud 1.9, spaCy 3.8 com `pt_core_news_{sm,md,lg}` 3.8.0. Rodar os testes: `pip install -r requirements-dev.txt` e `pytest` na raiz.

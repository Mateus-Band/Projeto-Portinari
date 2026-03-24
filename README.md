# Projeto Portinari

Projeto de TED (Tema de Estudo Dirigido) da graduação com foco na análise computacional do acervo pictórico de Cândido Portinari. O projeto é dividido em frentes independentes que se complementam: coleta automatizada de dados, identificação de paletas de cores, análise e visualização dos metadados, e análise de correspondências — com outras frentes previstas.

---

## Estrutura do repositório

```
.
├── Data/                        # Base de dados local (não versionada — ver abaixo)
│   ├── imagens/                 # Imagens das obras baixadas
│   ├── obras.csv                # Metadados em CSV
│   └── obras_limpo.json         # Metadados em JSON processado
│
├── Id_Cores/                    # Identificação de paletas de cores das pinturas
│   └── detectar_cores2.py
│
└── portinari-scrapper/          # Coleta automatizada de dados do site do Projeto Portinari
    ├── Scrapper.js              # Scraper principal (Node.js + Puppeteer)
    ├── analise_portinari.ipynb  # Notebook de limpeza e exploração dos dados
    ├── visualizacoes_portinari.ipynb  # Notebook de visualizações interativas
    └── package.json
```

---

## Frentes do projeto

### Coleta de dados (`portinari-scrapper/`)

Scraper desenvolvido em Node.js com Puppeteer que navega pelo site do [Projeto Portinari](https://www.portinari.org.br) e extrai metadados e imagens das obras do acervo. Suporta paginação completa (1826 obras em 37 páginas), retomada de execuções interrompidas e modo de teste com limite configurável.

Campos coletados por obra: título, código FCO, número CR, ano, técnica, suporte, dimensões, descrição curatorial, local de produção, autoria, temas, assinatura e URL da imagem em alta resolução.

**Dependências:** Node.js, Puppeteer, Axios, fs-extra

```bash
cd portinari-scrapper
npm install
node Scrapper.js
```

### Identificação de cores (`Id_Cores/`)

Módulo Python que analisa as imagens das pinturas e identifica as cores predominantes usando o espaço de cor HSV. Implementa amostragem aleatória de pixels e classifica cada amostra em categorias de cores nomeadas (vermelho, laranja, amarelo, verde, azul, roxo, etc.) com distinção entre tons claros e escuros.

**Dependências:** Python 3, OpenCV (`cv2`), NumPy

```bash
cd Id_Cores
python detectar_cores2.py
```

### Análise e visualizações (`portinari-scrapper/`)

Dois notebooks Jupyter para exploração e comunicação dos dados:

- `analise_portinari.ipynb` — carregamento do JSON, limpeza, normalização, filtros e exportação para CSV/Excel.
- `visualizacoes_portinari.ipynb` — visualizações interativas inspiradas no [Vikus Viewer](https://vikusviewer.fh-potsdam.de/): linha do tempo das obras, painel de estatísticas, dispersão por dimensões físicas, nuvem de palavras das descrições e grade visual filtrada por técnica e ano.

**Dependências:** Python 3, pandas, plotly, ipywidgets, wordcloud, matplotlib, Pillow, kaleido

```bash
pip install pandas plotly ipywidgets wordcloud matplotlib Pillow kaleido openpyxl
jupyter notebook
```

---

## Branches

| Branch | Responsabilidade |
|---|---|
| `main` | Código estável, integração das frentes |
| `scraper` | Coleta de dados e notebooks de análise |
| `identificador-cores` | Módulo de identificação de cores |

Novas branches serão criadas conforme novas frentes do projeto forem iniciadas.

---

## Base de dados

A base de dados (imagens e metadados) **não está disponível publicamente** neste repositório. O acesso é restrito aos colaboradores do projeto. O link de acesso será comunicado diretamente aos membros da equipe.

Os arquivos de dados são ignorados pelo `.gitignore` e não devem ser versionados.

---

## Status

O projeto está em desenvolvimento ativo. Todas as frentes funcionam, mas estão sujeitas a refinamentos. Novas frentes de análise (como o estudo de correspondências) estão previstas.

---

## Fonte dos dados

Os metadados e imagens são provenientes do site do [Projeto Portinari](https://www.portinari.org.br), acervo mantido pela família Portinari. Verifique os termos de uso do site antes de qualquer utilização dos dados além do contexto deste projeto.

---

## Dependências

**Runtime/Linguagens:**
- Node.js (para scraping)
- Python 3 (para análise e identificação de cores)

**Node.js packages:**
- Puppeteer — automação de navegador
- Axios — requisições HTTP
- fs-extra — operações de arquivo

**Python packages:**
- OpenCV (`cv2`) — processamento de imagens
- NumPy — operações numéricas
- pandas — manipulação de dados
- plotly — visualizações interativas
- ipywidgets — widgets para Jupyter
- wordcloud — nuvem de palavras
- matplotlib — gráficos
- Pillow — processamento de imagens
- kaleido — exportação de gráficos
- openpyxl — exportação para Excel
- jupyter — notebooks interativos

---

## Resumo das dependências

**Dependências principais do projeto:**

- **Node.js:**
    - puppeteer
    - axios
    - fs-extra

- **Python:**
    - opencv-python (cv2)
    - numpy
    - pandas
    - plotly
    - ipywidgets
    - wordcloud
    - matplotlib
    - pillow
    - kaleido
    - openpyxl
    - jupyter

Consulte as instruções acima para detalhes de instalação em cada frente do projeto.

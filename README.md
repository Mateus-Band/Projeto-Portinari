# Projeto Portinari

Projeto de Tema de Estudo Dirigido (TED) da graduação com foco na análise computacional, estatística e de visão computacional sobre o acervo artístico, literário e documental de Cândido Portinari.

O repositório é organizado de forma modular em frentes de pesquisa integradas por uma biblioteca central compartilhada.

---

## Estrutura do Repositório

```text
projeto-portinari/
├── README.md                   # Apresentação do projeto e guia de uso
├── CONTRIBUTING.md             # Padrões de branches, sincronização e PRs
├── pyproject.toml              # Configuração do projeto e dependências (uv/pip)
├── requirements.txt            # Dependências em formato pip tradicional
├── assets/                     # Identidade visual e imagens do projeto
│
├── src/portinari/              # Pacote Python compartilhado
│   ├── caminhos.py             # Caminhos padronizados relativos à raiz
│   ├── dados.py                # Funções de carregamento (obras, poemas, correspondências)
│   └── cores/                  # Módulos centrais de extração e análise cromática
│
├── dados/                      # Bases de dados (ver dados/README.md)
│   ├── brutos/                 # JSON original extraído pelo scraper
│   ├── processados/            # Obras tratadas, metadados com cores e correspondências
│   └── poemas/                 # Poemas catalogados e datasets literários
│
├── frentes/                    # Frentes de trabalho ativas
│   ├── coleta/                 # Scraper Puppeteer (Node.js) e tratamento das obras
│   ├── cores/                  # Identificação de paletas cromáticas, amostras e experimentos
│   ├── poemas/                 # Análise morfológica descritiva e classificador BERTimbau
│   ├── duplicatas/             # Detecção e tratamento de duplicatas
│   ├── correspondencias/       # Análise de correspondências e cartas (em estruturação)
│   └── ocr/                    # Reconhecimento óptico de manuscritos (futuro)
│
├── apresentacoes/              # Notebooks preparados para exposições e bancas
└── arquivo/                    # Frentes e experimentos históricos preservados
```

---

## Frentes de Pesquisa

### 1. Coleta e Tratamento de Obras (`frentes/coleta/`)
* **Scraper:** Desenvolvido em Node.js com Puppeteer para coletar metadados e URLs de imagens do portal do Projeto Portinari.
* **Tratamento:** Notebooks de normalização de dados curatoriais, dimensões físicas e padronização.

### 2. Identificação de Cores (`frentes/cores/`)
* Módulos em Python utilizando OpenCV, NumPy e Scikit-Learn para quantificação de cores predominantes via K-means, Mean Shift e binning no espaço de cores HSV.
* Notebooks de visualização e paletas por obra e por década.

### 3. Poemas de Portinari (`frentes/poemas/`)
* **Descritiva:** Análise morfossintática com spaCy, coocorrência de termos, frequências lexicais e nuvens de palavras.
* **Classificador LLM:** Treinamento e ajuste fino do modelo **BERTimbau** (`neuralmind/bert-base-portuguese-cased`) para classificação e estilometria dos poemas.

### 4. Análise de Duplicatas (`frentes/duplicatas/`)
* Auditoria dos identificadores FCO, número CR e duplicatas de registros no acervo.

---

## Instalação e Ambiente

### Opção 1: Usando uv (Recomendado)
```bash
# Sincroniza o ambiente com as dependências do projeto
uv sync

# Para rodar scripts ou jupyter usando o ambiente:
uv run jupyter notebook
```

### Opção 2: Usando venv e pip tradicional
```bash
# Cria e ativa o ambiente virtual
python3 -m venv .venv
source .venv/bin/activate

# Instala as dependências e o pacote local em modo editável
pip install -r requirements.txt
pip install -e .
```

---

## Como Utilizar o Pacote `portinari`

Em qualquer script ou notebook do repositório, você pode carregar as bases e caminhos diretamente sem se preocupar com diretórios absolutos:

```python
import portinari

# Carregar DataFrames das bases principais
df_obras = portinari.carregar_obras()
df_cores = portinari.carregar_obras_cores()
df_poemas = portinari.carregar_poemas()
df_cartas = portinari.carregar_correspondencias()

# Acessar caminhos universais
print(portinari.caminhos.RAIZ)
print(portinari.caminhos.DADOS_DIR)
```

Consulte [CONTRIBUTING.md](CONTRIBUTING.md) para conhecer as diretrizes de branches e Pull Requests.

# Projeto Portinari

Projeto de Tema de Estudo Dirigido (TED) da graduação com foco na análise computacional, estatística e de visão computacional sobre o acervo artístico, literário e documental de Cândido Portinari.

O repositório é organizado de forma modular em frentes de pesquisa integradas por uma biblioteca central compartilhada (`portinari`).

---

## Política de Dados e Google Drive

> [!IMPORTANT]
> **Nenhum arquivo de dados ou imagem é versionado no GitHub.**
> Todas as bases de metadados, textos de poemas, cartas e imagens de pinturas residem exclusivamente na pasta institucional do **Google Drive** do Projeto Portinari por motivos de privacidade e direitos de acervo.

### Como Obter e Configurar os Dados

Para que os scripts e notebooks funcionem na sua máquina:

1. **Acesso:** Solicite acesso à pasta do Google Drive do projeto via e-mail institucional (`al.mateus.torres@impatech.edu.br`).
2. **Posicionamento:** Baixe a pasta de dados e posicione-a na raiz do projeto com o nome **`data/`** (essa pasta já está no `.gitignore` e não será enviada ao GitHub).
3. **Opção com rclone (Recomendado para manter atualizado):**
   ```bash
   # Configurar remote no rclone (ex: 'portinari_drive') e sincronizar:
   rclone sync portinari_drive:Projeto-Portinari/data data/
   ```

A estrutura esperada dentro de `data/` é:
```text
data/
├── brutos/                 # obras.json
├── processados/            # obras.csv, obras_limpo.json, obras_cores.csv, correspondencias.csv
├── poemas/                 # poemas.csv, poemas_com_obras.csv, portuguese-poems.csv
└── imagens/                # Imagens das obras (quando necessário)
```

---

## Estrutura do Repositório

```text
projeto-portinari/
├── README.md                   # Apresentação do projeto e guia de uso
├── CONTRIBUTING.md             # Padrões de branches, sincronização e PRs
├── pyproject.toml              # Configuração do projeto e dependências (uv/pip)
├── requirements.txt            # Dependências em formato pip tradicional
├── .gitignore                  # Ignora data/, dados/, imagens e PDFs
├── assets/                     # Identidade visual e imagens do projeto
│
├── src/portinari/              # Pacote Python compartilhado
│   ├── caminhos.py             # Caminhos padronizados com detecção automática de data/
│   ├── dados.py                # Funções de carregamento (obras, poemas, correspondências)
│   └── cores/                  # Módulos centrais de extração e análise cromática
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
├── arquivo/                    # Frentes e experimentos históricos preservados
└── data/                       # [NÃO VERSIONADO] Pasta local com os dados do Drive
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

### Usando uv (Ferramenta Padrão do Projeto)
O projeto utiliza o `uv` para o gerenciamento ultrarrápido de dependências, garantindo que todos utilizem a mesma versão através do `pyproject.toml` e `uv.lock`.

```bash
# Sincroniza e instala o ambiente virtual com as dependências do projeto e o pacote portinari localmente:
uv sync

# Para rodar scripts, análises ou o jupyter usando o ambiente configurado:
uv run jupyter notebook
```

---

## Como Utilizar o Pacote `portinari`

Em qualquer script ou notebook do repositório, você pode carregar as bases e caminhos diretamente sem se preocupar com diretórios absolutos locais:

```python
import portinari

# Carregar DataFrames automaticamente da pasta data/ local:
df_obras = portinari.carregar_obras()
df_cores = portinari.carregar_obras_cores()
df_poemas = portinari.carregar_poemas()
df_cartas = portinari.carregar_correspondencias()

# Acessar caminhos universais:
print(portinari.caminhos.RAIZ)
print(portinari.caminhos.DADOS_DIR)    # Aponta automaticamente para a pasta data/
print(portinari.caminhos.IMAGENS_DIR)
```

Consulte [CONTRIBUTING.md](CONTRIBUTING.md) para conhecer as diretrizes de branches e Pull Requests.

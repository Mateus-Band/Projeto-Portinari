# 🎨 Portinari Scraper

Extrai metadados e imagens das obras do [Projeto Portinari](https://www.portinari.org.br) usando **Node.js + Puppeteer**.

---

## 📦 Instalação

```bash
# 1. Entre na pasta do projeto
cd portinari-scraper

# 2. Instale as dependências
npm install
```

> **Nota:** O Puppeteer baixa automaticamente o Chromium (~170 MB). Aguarde na primeira instalação.

---

## 🚀 Como usar

```bash
node scraper.js
```

Os arquivos serão gerados em `./output/`:
```
output/
├── obras.json        ← todos os metadados em JSON
└── imagens/
    ├── 14831_cafe.jpg
    ├── 14687_....jpg
    └── ...
```

---

## ⚙️ Configuração

Edite o objeto `CONFIG` no topo do `scraper.js`:

| Propriedade             | Descrição                                                   |
|-------------------------|-------------------------------------------------------------|
| `listingUrl`            | URL da página de listagem de obras                          |
| `outputDir`             | Pasta de saída                                              |
| `timeout`               | Tempo máximo para carregar uma página (ms)                  |
| `delayBetweenRequests`  | Pausa entre requisições para respeitar o servidor (ms)      |
| `limit`                 | Limita o número de obras (null = todas)                     |

---

## 🔍 Ajustando os seletores CSS

O site do Portinari usa renderização dinâmica. Se o scraper não encontrar dados,
o HTML pode ter mudado. Use o **modo diagnóstico** para inspecionar:

```js
// No scraper.js, descomente estas linhas no bloco try:
await diagnosticarPagina(page, CONFIG.listingUrl);
await diagnosticarPagina(page, "https://www.portinari.org.br/acervo/obras/14831/cafe");
await browser.close(); return;
```

Isso imprime os primeiros 5000 caracteres do HTML no terminal.
Inspecione os nomes das classes CSS e atualize os seletores em `extrairMetadadosDeObra()`.

---

## 🐛 Debug visual (ver o browser abrindo)

Mude `headless: true` para `headless: false` em `puppeteer.launch()`:

```js
const browser = await puppeteer.launch({
  headless: false, // ← abre o Chromium visualmente
  slowMo: 100,     // ← desacelera ações para acompanhar
});
```

---

## 🗂️ Estrutura do JSON de saída (`obras.json`)

```json
[
  {
    "id": "14831",
    "url": "https://www.portinari.org.br/acervo/obras/14831/cafe",
    "titulo": "Café",
    "autor": "Cândido Portinari",
    "ano": "1935",
    "tecnica": "Óleo sobre tela",
    "dimensoes": "130 x 195 cm",
    "localizacao": "Museu Nacional de Belas Artes",
    "descricao": "...",
    "imageUrl": "https://...",
    "imagemLocal": "14831_Caf_.jpg",
    "camposAdicionais": { ... }
  }
]
```

---

## ⚠️ Boas práticas de Web Scraping

- **Respeite o `robots.txt`** do site antes de fazer scraping em produção.
- Mantenha o `delayBetweenRequests` ≥ 1000 ms para não sobrecarregar o servidor.
- Para grandes volumes, rode em horários de menor tráfego.
- Verifique os **termos de uso** do Projeto Portinari quanto ao uso das imagens.
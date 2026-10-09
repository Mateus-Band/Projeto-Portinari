/**
 * Portinari Scraper — versão precisa
 * Seletores extraídos diretamente do HTML real do site.
 *
 * Instalação:
 *   npm install puppeteer axios fs-extra
 *
 * Uso:
 *   node scraper.js
 */

const puppeteer = require("puppeteer");
const axios = require("axios");
const fs = require("fs-extra");
const path = require("path");

// ─── Configurações ────────────────────────────────────────────────────────────

const CONFIG = {
  // URL base da listagem de pinturas
  baseListingUrl: "https://www.portinari.org.br/acervo/obras/@relId/14687",

  // ⬇️  Quantas obras processar no total (null = todas as 1826)
  LIMITE: null,

  // ⬇️  Começar a partir de qual índice global de obra (1-based). Use 788 para retomar do item 788.
  startIndex: 1,

  // Se true: visita cada página de obra para pegar todos os metadados.
  // Se false: só extrai o que está disponível na listagem (mais rápido).
  modoCompleto: true,

  // Pasta de saída
  outputDir: "./output",

  // Delay entre requisições (ms) — respeite o servidor!
  delayMs: 1500,

  // Timeout para carregar cada página (ms)
  timeout: 30000,
};

// ─── Utilitários ──────────────────────────────────────────────────────────────

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

const sanitize = (str) =>
  (str || "sem_titulo").replace(/[^a-z0-9_\-]/gi, "_").substring(0, 80);

async function downloadImagem(url, destPath) {
  try {
    const resp = await axios.get(url, {
      responseType: "arraybuffer",
      timeout: 20000,
      headers: {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        Referer: "https://www.portinari.org.br/",
      },
    });
    await fs.outputFile(destPath, resp.data);
    return true;
  } catch (e) {
    console.error(`  ❌ Falha ao baixar imagem: ${e.message}`);
    return false;
  }
}

// ─── Extração da listagem ─────────────────────────────────────────────────────
// A listagem já expõe: título, FCO, CR, data, descrição resumida e URL da imagem
// em alta resolução (S3). Não precisamos visitar a página individual só pela imagem.

async function extrairCardsDeListagem(page, url) {
  await page.goto(url, { waitUntil: "networkidle2", timeout: CONFIG.timeout });
  await page.waitForSelector(".card", { timeout: CONFIG.timeout });

  return await page.evaluate(() => {
    return Array.from(document.querySelectorAll(".card")).map((card) => {
      // URL da página individual
      const linkEl = card.querySelector("a.card-image");
      const obraUrl = linkEl?.href || null;

      // ID numérico da obra extraído da URL (ex: /obras/14758/retrato... → 14758)
      const idMatch = obraUrl?.match(/\/obras\/(\d+)/);
      const id = idMatch ? idMatch[1] : null;

      // Imagem em alta resolução — está no atributo href do botão de zoom
      // Ex: href="https://acervo-portinari.s3.sa-east-1.amazonaws.com/abc.jpeg"
      const imagemUrl = card.querySelector("a.card-zoom")?.href || null;

      // Título
      const titulo = card.querySelector(".card-text--title")?.innerText.trim() || null;

      // Código FCO e número CR — estão no mesmo elemento separados por " | "
      // Ex: "FCO-6  |  CR-942"
      const idTexto = card.querySelector(".card-text--id")?.innerText.trim() || "";
      const fcoMatch = idTexto.match(/FCO-[\w\d]+/);
      const crMatch  = idTexto.match(/CR-[\d]+/);
      const codigoFCO = fcoMatch ? fcoMatch[0] : null;
      const numeroCR  = crMatch  ? crMatch[0]  : null;

      // Data/ano — aparece entre colchetes ou sem: "[1938]" ou "1957"
      const dataTexto = card.querySelector(".card-text--additional")?.innerText.trim() || null;

      // Descrição resumida (texto truncado com "...")
      const descricaoResumida = card.querySelector(".card-text--description")?.innerText.trim() || null;

      return { id, obraUrl, titulo, codigoFCO, numeroCR, dataTexto, imagemUrl, descricaoResumida };
    });
  });
}

// ─── Extração da página individual ───────────────────────────────────────────
// Usa atributos itemprop para encontrar os campos com precisão.

async function extrairMetadadosDaObra(page, url) {
  await page.goto(url, { waitUntil: "networkidle2", timeout: CONFIG.timeout });
  await page.waitForSelector(".highlights", { timeout: CONFIG.timeout });

  return await page.evaluate(() => {
    // Helper: retorna todos os textos de âncoras dentro de um campo
    const campoLista = (prop) => {
      const label = document.querySelector(`[itemprop="${prop}"]`);
      if (!label) return [];
      const row = label.closest(".property-row");
      return Array.from(row?.querySelectorAll(".property-value a") || [])
        .map((a) => a.innerText.trim())
        .filter(Boolean);
    };

    // Helper: retorna o texto simples de um campo
    const campo = (prop) => {
      const label = document.querySelector(`[itemprop="${prop}"]`);
      if (!label) return null;
      const row = label.closest(".property-row");
      return row?.querySelector(".limited-text")?.innerText.trim()
        || campoLista(prop).join(", ")
        || null;
    };

    // ── Imagem em alta resolução ──
    // <a data-fancybox="gallery" href="https://...s3...jpeg">
    const imagemUrl = document.querySelector('a[data-fancybox="gallery"]')?.href || null;

    // ── Destaques rápidos (div.highlights) ──
    // Estrutura: <div><h3>dimensões</h3>100 x 81 cm</div>
    const highlights = {};
    document.querySelectorAll(".highlights > div").forEach((div) => {
      const h3 = div.querySelector("h3");
      if (!h3) return;
      const chave = h3.innerText.trim().toLowerCase();
      const valor = div.innerText.replace(h3.innerText, "").trim();
      highlights[chave] = valor;
    });

    return {
      imagemUrl,
      titulo:             campo("name"),
      descricaoCompleta:  campo("description"),
      localProducao:      campoLista("locationCreated").join(" > "),
      autoria:            campoLista("author").join(", "),
      tipoObra:           campoLista("artForm").join(", "),
      tecnica:            campoLista("artMedium").join(", "),
      suporte:            campoLista("artworkSurface").join(", "),
      numeroDN:           campo("dnNumber"),
      altura:             campo("height"),
      largura:            campo("width"),
      assinatura:         campo("annotation"),
      inscricaoFamilia:   campo("annotationFam"),
      inscricaoExposicao: campo("annotationExp"),
      dimensoes:          highlights["dimensões"] || null,
      temas:              campoLista("about"),
      pessoasRetratadas:  campoLista("aboutPerson"),
    };
  });
}

// ─── Main ─────────────────────────────────────────────────────────────────────

async function main() {
  await fs.ensureDir(path.join(CONFIG.outputDir, "imagens"));

  const outputJsonPath = path.join(CONFIG.outputDir, "obras.json");
  let todasObras = [];
  if (await fs.pathExists(outputJsonPath)) {
    try {
      todasObras = await fs.readJson(outputJsonPath);
      console.log(`🟢 JSON existente carregado: ${todasObras.length} obras em ${outputJsonPath}`);
    } catch (e) {
      console.warn(`⚠️ Não foi possível ler JSON existente: ${e.message}. Reiniciando do zero.`);
      todasObras = [];
    }
  }

  let totalProcessadas = todasObras.length;
  let currentIndex = 0;
  const existingIds = new Set(todasObras.filter((o) => o.id).map((o) => o.id));

  let resumeIndex = Math.max(CONFIG.startIndex, totalProcessadas + 1);
  if (totalProcessadas > 0) {
    console.log(`▶️  Continuando do índice ${resumeIndex} (já processadas ${totalProcessadas}).`);
  }

  console.log("🚀 Iniciando Puppeteer...");
  const browser = await puppeteer.launch({
    headless: true,
    args: ["--no-sandbox", "--disable-setuid-sandbox"],
  });
  const page = await browser.newPage();
  await page.setUserAgent(
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
  );
  await page.setViewport({ width: 1280, height: 900 });

  let paginaAtual = 1;
  let continuar = true;

  try {
    // ── Percorre páginas da listagem ──────────────────────────────────────────
    while (continuar) {
      const urlPagina =
        paginaAtual === 1
          ? CONFIG.baseListingUrl
          : `${CONFIG.baseListingUrl}/@p/${paginaAtual}`;

      console.log(`\n📄 Página ${paginaAtual}: ${urlPagina}`);
      const cards = await extrairCardsDeListagem(page, urlPagina);
      console.log(`   ${cards.length} obras encontradas nesta página.`);

      if (cards.length === 0) break;

      // ── Processa cada card ────────────────────────────────────────────────
      for (const card of cards) {
        currentIndex++;

        if (currentIndex < resumeIndex) {
          continue; // pular objetos já processados
        }

        if (card.id && existingIds.has(card.id)) {
          console.log(`   ⏭️  Pulando obra já salva (id=${card.id})`);
          continue;
        }

        if (CONFIG.LIMITE !== null && totalProcessadas >= CONFIG.LIMITE) {
          continuar = false;
          break;
        }

        totalProcessadas++;
        console.log(`\n[${totalProcessadas}${CONFIG.LIMITE ? "/" + CONFIG.LIMITE : ""}] ${card.titulo} (global #${currentIndex})`);

        let obra = { ...card };

        // ── Metadados completos (visita a página individual) ────────────────
        if (CONFIG.modoCompleto && card.obraUrl) {
          try {
            await sleep(CONFIG.delayMs);
            const detalhes = await extrairMetadadosDaObra(page, card.obraUrl);
            obra = { ...obra, ...detalhes };
            console.log(`   ✅ Metadados extraídos`);
          } catch (e) {
            console.error(`   ⚠️  Erro nos metadados: ${e.message}`);
          }
        }

        // ── Download da imagem ──────────────────────────────────────────────
        const imgUrl = obra.imagemUrl;
        if (imgUrl) {
          const ext = path.extname(imgUrl.split("?")[0]) || ".jpg";
          const filename = `${obra.id || totalProcessadas}_${sanitize(obra.titulo)}${ext}`;
          const destPath = path.join(CONFIG.outputDir, "imagens", filename);

          const ok = await downloadImagem(imgUrl, destPath);
          obra.imagemLocal = ok ? filename : null;
          if (ok) console.log(`   🖼️  Imagem salva: ${filename}`);
        } else {
          console.warn(`   ⚠️  URL de imagem não encontrada`);
        }

        todasObras.push(obra);
        if (obra.id) existingIds.add(obra.id);

        // Salva JSON parcialmente a cada obra (segurança contra interrupções)
        await fs.writeJson(
          path.join(CONFIG.outputDir, "obras.json"),
          todasObras,
          { spaces: 2 }
        );
      }

      paginaAtual++;
      if (paginaAtual > 37) break; // Site tem 37 páginas (1826 obras / ~50 por página)
      if (continuar) await sleep(CONFIG.delayMs);
    }
  } finally {
    await browser.close();
  }

  // ── Resumo ────────────────────────────────────────────────────────────────
  const comImagem   = todasObras.filter((o) => o.imagemLocal).length;
  const comMetadados = todasObras.filter((o) => o.titulo).length;

  console.log("\n" + "═".repeat(55));
  console.log(`✅ Concluído!`);
  console.log(`   Obras processadas : ${todasObras.length}`);
  console.log(`   Com imagem salva  : ${comImagem}`);
  console.log(`   Com título        : ${comMetadados}`);
  console.log(`   Saída             : ${path.resolve(CONFIG.outputDir)}`);
  console.log("═".repeat(55));
}

main().catch((err) => {
  console.error("💥 Erro fatal:", err);
  process.exit(1);
});
import os
import pandas as pd
import plotly.graph_objects as go

os.makedirs("Localidades/Localidades_graficos", exist_ok=True)

# ─────────────────────────────────────────────
# 🔹 Hierarquia de temas
# ─────────────────────────────────────────────
HIERARQUIA = {
    "Cenas de trabalho": [
        "Cultura de cana",
        "Cultura de fumo",
        "Cultura de erva-mate",
        "Garimpagem",
        "Cultura de borracha",
        "Cultura de arroz",
        "Cultura de milho",
        "Cultura de café",
        "Lavoura",
        "Cultura de cacau",
        "Cultura de algodão",
    ],
    "Trabalhadores": [
        "Lavrador",
        "Garimpeiro",
        "Operário",
    ],
    "Paisagem": [
        "Campo",
        "Cidade",
        "Brodowski",
        "Marinha",
        "Morro",
        "Floresta",
        "Rio de Janeiro",
        "Deserto",
    ],
    "Cenas Históricas": [
        "Descobrimento do Brasil",
    ],
    "Festas populares": [
        "Carnaval",
    ],
}

TODOS_OS_PAIS = set(HIERARQUIA.keys())
FILHO_PARA_PAI = {
    filho: pai
    for pai, filhos in HIERARQUIA.items()
    for filho in filhos
}

# ─────────────────────────────────────────────
# 🔹 Carregar CSV
# ─────────────────────────────────────────────
csv_path = "Localidades/Localidades_Data/filtro_frequencia.csv"
df = pd.read_csv(csv_path)
df["frequencia"] = pd.to_numeric(df["frequencia"], errors="coerce").fillna(0)
freq_map = dict(zip(df["tema"], df["frequencia"]))

# ─────────────────────────────────────────────
# 🔹 Montar listas para o Plotly (branchvalues="remainder")
# ─────────────────────────────────────────────
ROOT = "Portinari"

ids     = []
labels  = []
parents = []
values  = []

def add(id_, label, parent, value):
    ids.append(id_)
    labels.append(label)
    parents.append(parent)
    values.append(value)

add(ROOT, ROOT, "", 0)

for pai in HIERARQUIA:
    add(pai, pai, ROOT, freq_map.get(pai, 0))

for pai, filhos in HIERARQUIA.items():
    for filho in filhos:
        if filho in freq_map:
            add(filho, filho, pai, freq_map[filho])

for tema, freq in freq_map.items():
    if tema not in TODOS_OS_PAIS and tema not in FILHO_PARA_PAI:
        add(tema, tema, ROOT, freq)

# ─────────────────────────────────────────────
# 🔹 Cores
# ─────────────────────────────────────────────
CORES_PAI = {
    "Cenas de trabalho": "#E07B54",
    "Trabalhadores"    : "#5B8DB8",
    "Paisagem"         : "#6AAF6A",
    "Cenas Históricas" : "#C97BBD",
    "Festas populares" : "#E8C44A",
    "Guerra"           : "#E05C5C",
    "Favela"           : "#8C7B6B",
    "Circo"            : "#F4A460",
    "Industrialização" : "#7B7B7B",
}
COR_PADRAO = "#BBBBBB"

def cor_do_no(id_no):
    if id_no == ROOT:
        return "#EEEEEE"
    if id_no in CORES_PAI:
        return CORES_PAI[id_no]
    pai = FILHO_PARA_PAI.get(id_no)
    if pai and pai in CORES_PAI:
        return CORES_PAI[pai]
    return COR_PADRAO

cores = [cor_do_no(i) for i in ids]

HOVER = "<b>%{label}</b><br>Frequência: %{value:.2f}<extra></extra>"

# ─────────────────────────────────────────────
# 🔹 Sunburst
# ─────────────────────────────────────────────
fig_sunburst = go.Figure(
    go.Sunburst(
        ids=ids,
        labels=labels,
        parents=parents,
        values=values,
        marker=dict(colors=cores),
        branchvalues="remainder",
        hovertemplate=HOVER,
        textfont=dict(size=12),
        insidetextorientation="radial",
        maxdepth=3,
    )
)
fig_sunburst.update_layout(
    title=dict(text="Temas das Obras de Portinari — Sunburst", font=dict(size=20), x=0.5),
    margin=dict(t=60, l=10, r=10, b=10),
    width=850,
    height=850,
)
fig_sunburst.show()
fig_sunburst.write_html("Localidades/Localidades_graficos/sunburst_temas.html")
print("✅ Sunburst salvo em Localidades/Localidades_graficos/sunburst_temas.html")

# ─────────────────────────────────────────────
# 🔹 Icicle
# ─────────────────────────────────────────────
fig_icicle = go.Figure(
    go.Icicle(
        ids=ids,
        labels=labels,
        parents=parents,
        values=values,
        marker=dict(colors=cores),
        branchvalues="remainder",
        hovertemplate=HOVER,
        textfont=dict(size=12),
        maxdepth=3,
        tiling=dict(orientation="v"),
    )
)
fig_icicle.update_layout(
    title=dict(text="Temas das Obras de Portinari — Icicle", font=dict(size=20), x=0.5),
    margin=dict(t=60, l=10, r=10, b=10),
    width=1000,
    height=700,
)
fig_icicle.show()
fig_icicle.write_html("Localidades/Localidades_graficos/icicle_temas.html")
print("✅ Icicle salvo em Localidades/Localidades_graficos/icicle_temas.html")

# ─────────────────────────────────────────────
# 🔹 Treemap
# ─────────────────────────────────────────────
fig_treemap = go.Figure(
    go.Treemap(
        ids=ids,
        labels=labels,
        parents=parents,
        values=values,
        marker=dict(colors=cores),
        branchvalues="remainder",
        hovertemplate=HOVER,
        textfont=dict(size=12),
        maxdepth=3,
        tiling=dict(packing="squarify"),
        pathbar=dict(visible=True),
    )
)
fig_treemap.update_layout(
    title=dict(text="Temas das Obras de Portinari — Treemap", font=dict(size=20), x=0.5),
    margin=dict(t=60, l=10, r=10, b=10),
    width=1100,
    height=700,
)
fig_treemap.show()
fig_treemap.write_html("Localidades/Localidades_graficos/treemap_temas.html")
print("✅ Treemap salvo em Localidades/Localidades_graficos/treemap_temas.html")
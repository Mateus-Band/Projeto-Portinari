import pandas as pd

# Caminhos
input_path = "Localidades/Localidades_Data/obras_filtrado.csv"
output_path = "Localidades/Localidades_Data/filtro_frequencia.csv"

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

FILHO_PARA_PAI = {
    filho: pai
    for pai, filhos in HIERARQUIA.items()
    for filho in filhos
}
TODOS_OS_PAIS = set(HIERARQUIA.keys())


def temas_folha(temas: list) -> list:
    """
    Retorna apenas os temas folha da obra:
    - Filhos sempre são mantidos.
    - Um pai só é mantido se a obra NÃO possui nenhum filho seu.
    - Independentes (sem pai na hierarquia) sempre são mantidos.
    """
    pais_cobertos = {FILHO_PARA_PAI[t] for t in temas if t in FILHO_PARA_PAI}

    return [
        t for t in temas
        if not (t in TODOS_OS_PAIS and t in pais_cobertos)
    ]


# ─────────────────────────────────────────────
# 🔹 Pipeline
# ─────────────────────────────────────────────
df = pd.read_csv(input_path)

df["temas_filtrados"] = df["temas_filtrados"].fillna("")
df["temas_lista"] = df["temas_filtrados"].apply(
    lambda x: [t.strip() for t in x.split(",") if t.strip()]
)

# Aplicar filtragem hierárquica
df["temas_lista"] = df["temas_lista"].apply(temas_folha)

# Explodir — uma linha por tema folha
df_explodido = df.explode("temas_lista")
df_explodido = df_explodido[df_explodido["temas_lista"].notna() & (df_explodido["temas_lista"] != "")]

# Contar ocorrências simples (sem peso)
frequencia = (
    df_explodido["temas_lista"]
    .value_counts()
    .reset_index()
)
frequencia.columns = ["tema", "frequencia"]

# Adicionar coluna de tipo
def tipo_tema(tema):
    if tema in TODOS_OS_PAIS:
        return "pai (sem filhos na obra)"
    elif tema in FILHO_PARA_PAI:
        return f"filho de '{FILHO_PARA_PAI[tema]}'"
    else:
        return "independente"

frequencia["tipo"] = frequencia["tema"].apply(tipo_tema)

# Salvar
frequencia.to_csv(output_path, index=False, encoding="utf-8")

print(f"Arquivo de frequência criado em: {output_path}")
print(f"\n📊 Temas encontrados ({len(frequencia)}):\n")
print(frequencia.to_string(index=False))
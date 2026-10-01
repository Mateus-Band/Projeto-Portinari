import csv

ARQUIVO = "Data/obras.csv"
SAIDA = "./analise_duplicatas/obras_sem_imagem.csv"


# ---------------------------------------------------------
# Carrega o CSV
# ---------------------------------------------------------

with open(ARQUIVO, "r", encoding="utf-8-sig", newline="") as f:
    leitor = csv.DictReader(f)
    dados = list(leitor)


# ---------------------------------------------------------
# Identifica obras sem imagemUrl
# ---------------------------------------------------------

sem_imagem = []

for obra in dados:

    imagem_url = obra.get("imagemUrl", "")

    # Considera sem imagem se:
    # - campo não existir
    # - estiver vazio
    # - tiver apenas espaços
    # - for "null" ou "None"

    if (
        imagem_url is None
        or imagem_url.strip() == ""
        or imagem_url.strip().lower() in ["null", "none", "nan"]
    ):
        sem_imagem.append(obra)


# ---------------------------------------------------------
# Salva o resultado
# ---------------------------------------------------------

if sem_imagem:

    with open(SAIDA, "w", encoding="utf-8", newline="") as f:

        campos = list(dados[0].keys())

        escritor = csv.DictWriter(
            f,
            fieldnames=campos
        )

        escritor.writeheader()
        escritor.writerows(sem_imagem)


# ---------------------------------------------------------
# Resultado
# ---------------------------------------------------------

print(f"Total de obras: {len(dados)}")
print(f"Obras sem imagemUrl: {len(sem_imagem)}")

if sem_imagem:
    print(f"Arquivo gerado: {SAIDA}")
else:
    print("Nenhuma obra sem imagemUrl foi encontrada.")

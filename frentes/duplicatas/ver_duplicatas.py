import json
from collections import defaultdict

ARQUIVO = "Data/obras_limpo.json"
ARQUIVO_DUPLICATAS = "./analise_duplicatas/duplicatas_numeroCR.json"


def encontrar_duplicados(dados, campo):
    grupos = defaultdict(list)

    for obra in dados:
        valor = obra.get(campo)

        # Ignora campos vazios ou nulos
        if valor is not None and valor != "":
            grupos[str(valor)].append(obra)

    # Mantém somente valores que aparecem mais de uma vez
    return {
        valor: obras
        for valor, obras in grupos.items()
        if len(obras) > 1
    }


# ---------------------------------------------------------
# Carrega o JSON
# ---------------------------------------------------------

with open(ARQUIVO, "r", encoding="utf-8") as f:
    dados = json.load(f)


print(f"Total de obras no arquivo: {len(dados)}")
print("=" * 70)


# ---------------------------------------------------------
# Procura duplicatas somente em numeroCR
# ---------------------------------------------------------

duplicados = encontrar_duplicados(dados, "numeroCR")


# ---------------------------------------------------------
# Mostra as duplicatas
# ---------------------------------------------------------

print("\n" + "=" * 70)
print("DUPLICADOS POR: numeroCR")
print("=" * 70)

if not duplicados:

    print("Nenhuma duplicação encontrada.")

else:

    print(f"Valores repetidos: {len(duplicados)}")

    for valor, obras in duplicados.items():

        print(f"\nnumeroCR: {valor}")
        print(f"Quantidade: {len(obras)}")

        for obra in obras:
            print(
                f"  - ID: {obra.get('id')} | "
                f"Título: {obra.get('titulo')} | "
                f"CR: {obra.get('numeroCR')} | "
                f"DN: {obra.get('numeroDN')} | "
                f"Imagem: {obra.get('imagemLocal')}"
            )


# ---------------------------------------------------------
# Salva as duplicatas em um novo JSON
# ---------------------------------------------------------

with open(ARQUIVO_DUPLICATAS, "w", encoding="utf-8") as f:
    json.dump(
        duplicados,
        f,
        ensure_ascii=False,
        indent=4
    )


print("\n" + "=" * 70)
print(f"Duplicatas salvas em: {ARQUIVO_DUPLICATAS}")
print("=" * 70)
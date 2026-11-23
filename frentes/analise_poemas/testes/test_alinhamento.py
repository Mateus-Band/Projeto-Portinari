from src.alinhamento import alinhar
from src.modelos.base import PalavraModelo as W
from src.preprocessamento import preparar_poema


def tokens_de(texto):
    return preparar_poema("p", texto, normalizar=False).tokens


def test_uma_palavra_por_token_caso_normal():
    toks = tokens_de("a casa velha")
    palavras = [W(0, 1, "a", "o", "DET", "Gender=Fem"), W(2, 6, "casa", "casa", "NOUN", "Gender=Fem"),
                W(7, 12, "velha", "velho", "ADJ", "Gender=Fem")]
    r = alinhar(toks, palavras)
    assert [l["lema"] for l in r.linhas] == ["o", "casa", "velho"]
    assert r.n_reagrupados == 0 and r.n_divergencias == 0


def test_contracao_separada_e_reagrupada_com_classe_da_primeira_componente():
    toks = tokens_de("o rio do povo")
    # "do" (posições 6:8) em duas palavras com a mesma posição, como Stanza/UDPipe/Trankit
    palavras = [W(0, 1, "o", "o", "DET"), W(2, 5, "rio", "rio", "NOUN"),
                W(6, 8, "de", "de", "ADP", "AdpType=Prep"), W(6, 8, "o", "o", "DET", "Definite=Def"),
                W(9, 13, "povo", "povo", "NOUN")]
    r = alinhar(toks, palavras)
    assert r.linhas[2] == {"lema": "de o", "upos": "ADP", "morph": "AdpType=Prep"}
    assert r.n_reagrupados == 1 and r.n_divergencias == 0
    assert len(r.linhas) == len(toks) == 4


def test_pontuacao_do_modelo_e_descartada():
    toks = tokens_de("rio, mar")
    palavras = [W(0, 3, "rio", "rio", "NOUN"), W(3, 4, ",", ",", "PUNCT"), W(5, 8, "mar", "mar", "NOUN")]
    r = alinhar(toks, palavras)
    assert [l["upos"] for l in r.linhas] == ["NOUN", "NOUN"] and r.n_divergencias == 0


def test_palavra_composta_partida_pelo_modelo_e_reagrupada_sem_o_hifen():
    toks = tokens_de("o bem-estar")
    palavras = [W(0, 1, "o", "o", "DET"), W(2, 5, "bem", "bem", "ADV"), W(5, 6, "-", "-", "PUNCT"),
                W(6, 11, "estar", "estar", "VERB")]
    r = alinhar(toks, palavras)
    assert r.linhas[1] == {"lema": "bem estar", "upos": "ADV", "morph": ""}
    assert r.n_reagrupados == 1 and r.n_divergencias == 0


def test_apostrofo_deixado_fora_pelo_modelo_e_aceito():
    toks = tokens_de("d'água")
    palavras = [W(0, 1, "d", "de", "ADP"), W(2, 6, "água", "água", "NOUN")]     # o apóstrofo (posição 1) não é coberto
    r = alinhar(toks, palavras)
    assert r.linhas[0]["lema"] == "de água" and r.n_divergencias == 0


def test_palavra_do_modelo_que_cobre_dois_tokens_e_divergencia():
    toks = tokens_de("rio mar")
    palavras = [W(0, 7, "rio mar", "rio mar", "NOUN")]
    r = alinhar(toks, palavras)
    assert r.n_divergencias == 2 and all(d["motivo"] == "limites não coincidem" for d in r.divergencias)
    assert r.linhas[0]["upos"] == "NOUN"           # usa a melhor correspondência


def test_token_sem_correspondencia_fica_vazio_e_conta_como_divergencia():
    toks = tokens_de("rio mar")
    r = alinhar(toks, [W(0, 3, "rio", "rio", "NOUN")])
    assert r.linhas[1] == {"lema": "", "upos": "", "morph": ""}
    assert r.divergencias[0]["motivo"] == "sem correspondência" and r.divergencias[0]["token"] == "mar"


def test_entrada_desordenada_e_vazia():
    toks = tokens_de("rio mar")
    palavras = [W(4, 7, "mar", "mar", "NOUN"), W(0, 3, "rio", "rio", "NOUN")]
    assert [l["lema"] for l in alinhar(toks, palavras).linhas] == ["rio", "mar"]
    assert alinhar([], []).linhas == []

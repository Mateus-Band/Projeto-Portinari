from src.preprocessamento import (
    capitalizadas_no_meio, normalizar_versos, preparar_corpus, preparar_poema, separar_versos, texto_do_poema,
)
import pandas as pd


def test_separar_versos_ignora_vazios_e_espacos():
    assert separar_versos("  Um verso  /  / outro   verso / ") == ["Um verso", "outro verso"]
    assert separar_versos("") == []
    assert separar_versos(" / / ") == []


def test_texto_do_poema_une_por_um_espaco_sem_quebra_de_linha():
    assert texto_do_poema("Não / Há rio, / nem pedras.") == "Não Há rio, nem pedras."
    assert "\n" not in texto_do_poema("a / b")


def test_verso_que_continua_a_frase_e_rebaixado():
    assert normalizar_versos(["Uma parada, uma parada", "Para o trem"]) == ["Uma parada, uma parada", "para o trem"]


def test_verso_que_abre_frase_e_preservado():
    versos = ["Companhia. Alegre e promissor", "Futuro…", "As festas, os bailes", "Procissões e o sino"]
    saida = normalizar_versos(versos)
    assert saida[2] == "As festas, os bailes"        # anterior termina em '…'
    assert saida[3] == "procissões e o sino"         # anterior não termina frase
    assert saida[0] == "Companhia. Alegre e promissor"


def test_primeiro_verso_nunca_e_alterado_e_maiuscula_no_meio_tambem_nao():
    saida = normalizar_versos(["Fizeram uma parada", "lá em Brodowski, de manhã"])
    assert saida == ["Fizeram uma parada", "lá em Brodowski, de manhã"]


def test_aspas_e_parenteses_depois_da_pontuacao_final():
    assert normalizar_versos(['ele disse: “Vou.”', "Depois saiu"])[1] == "Depois saiu"
    assert normalizar_versos(["(Foi embora.)", "Depois voltou"])[1] == "Depois voltou"


def test_nome_proprio_que_abre_verso_e_protegido():
    versos = ["A mãe chamava", "Emily cantava"]
    assert normalizar_versos(versos)[1] == "emily cantava"
    assert normalizar_versos(versos, frozenset({"Emily"}))[1] == "Emily cantava"


def test_capitalizadas_no_meio_detecta_nomes_e_ignora_inicio_de_verso_e_de_frase():
    textos = ["A mãe de Emily / Portinari pintou. Depois saiu / O areal"]
    achadas = capitalizadas_no_meio(textos)
    assert "Emily" in achadas
    assert "Portinari" not in achadas       # só aparece abrindo verso
    assert "Depois" not in achadas          # início de frase
    assert "O" not in achadas


def test_preparar_poema_posicoes_e_versos():
    p = preparar_poema("p1", "Fizeram uma parada / Para o trem, bem-estar / d'água", normalizar=True)
    assert p.texto == "Fizeram uma parada para o trem, bem-estar d'água"
    assert p.inicios == [0, 21, 40] or p.inicios[0] == 0
    for t in p.tokens:
        assert p.texto[t.ini:t.fim] == t.texto
    formas = [t.texto for t in p.tokens]
    assert "bem-estar" in formas and "d'água" in formas
    versos_dos_tokens = {t.texto: t.verso_idx for t in p.tokens}
    assert versos_dos_tokens["Fizeram"] == 0 and versos_dos_tokens["para"] == 1 and versos_dos_tokens["d'água"] == 2
    assert [t.idx for t in p.tokens] == list(range(len(p.tokens)))


def test_numeros_e_pontuacao_ficam_fora_dos_tokens():
    p = preparar_poema("p", "Em 1956, ele (bem) saiu!", normalizar=False)
    assert [t.texto for t in p.tokens] == ["Em", "ele", "bem", "saiu"]


def test_normalizar_false_entrega_os_versos_como_estao():
    p = preparar_poema("p", "uma parada / Para o trem", normalizar=False)
    assert p.versos == ["uma parada", "Para o trem"]


def test_poema_vazio():
    p = preparar_poema("p", "", normalizar=True)
    assert p.versos == [] and p.texto == "" and p.tokens == []


def test_preparar_corpus_protege_nomes_do_corpus_inteiro():
    corpus = pd.DataFrame({
        "poema_id": ["a", "b"],
        "texto": ["a casa de Emily / Emily chegou", "lá longe / Emily sorriu"],
    })
    com = preparar_corpus(corpus, normalizar=True)
    assert "Emily chegou" in com[0].texto and "Emily sorriu" in com[1].texto
    sem = preparar_corpus(corpus, normalizar=False)
    assert "Emily chegou" in sem[0].texto

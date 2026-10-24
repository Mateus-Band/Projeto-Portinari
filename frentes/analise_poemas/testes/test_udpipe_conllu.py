from src.modelos.udpipe2 import ModeloUDPipe2, parsear_conllu

TEXTO = "O rio do povo corria."
CONLLU = """# newdoc
# sent_id = 1
# text = O rio do povo corria.
1\tO\to\tDET\t_\tDefinite=Def|Gender=Masc|Number=Sing|PronType=Art\t2\tdet\t_\tTokenRange=0:1
2\trio\trio\tNOUN\t_\tGender=Masc|Number=Sing\t5\tnsubj\t_\tTokenRange=2:5
3-4\tdo\t_\t_\t_\t_\t_\t_\t_\tTokenRange=6:8
3\tde\tde\tADP\t_\t_\t5\tcase\t_\t_
4\to\to\tDET\t_\tDefinite=Def|Gender=Masc|Number=Sing|PronType=Art\t5\tdet\t_\t_
5\tpovo\tpovo\tNOUN\t_\tGender=Masc|Number=Sing\t6\tnsubj\t_\tTokenRange=9:13
6\tcorria\tcorrer\tVERB\t_\tMood=Ind|Number=Sing|Person=3|Tense=Imp|VerbForm=Fin\t0\troot\t_\tSpaceAfter=No|TokenRange=14:20
7\t.\t.\tPUNCT\t_\t_\t6\tpunct\t_\tSpaceAfter=No|TokenRange=20:21

"""


def test_parser_usa_token_range_e_herda_posicao_nas_contracoes():
    p = parsear_conllu(CONLLU, TEXTO)
    assert [(w.texto, w.ini, w.fim) for w in p] == [
        ("O", 0, 1), ("rio", 2, 5), ("de", 6, 8), ("o", 6, 8), ("povo", 9, 13), ("corria", 14, 20), (".", 20, 21)]
    assert p[2].lema == "de" and p[2].upos == "ADP" and p[2].morph == ""         # '_' vira vazio
    assert p[5].morph.startswith("Mood=Ind") and p[5].lema == "correr"


def test_parser_sem_token_range_procura_cada_forma_em_sequencia():
    sem_faixa = "\n".join(l.replace("TokenRange=", "X=") for l in CONLLU.splitlines())
    p = parsear_conllu(sem_faixa, TEXTO)
    assert [(w.texto, w.ini) for w in p if w.texto in ("rio", "povo", "corria")] == [("rio", 2), ("povo", 9), ("corria", 14)]
    assert [(w.ini, w.fim) for w in p if w.texto in ("de", "o")] == [(6, 8), (6, 8)]


def test_lote_em_bloco_devolve_posicoes_relativas_a_cada_texto(monkeypatch):
    import re

    def conllu_falso(texto):
        linhas, n = [], 0
        for m in re.finditer(r"\w+", texto):
            n += 1
            linhas.append(f"{n}\t{m.group()}\t{m.group().lower()}\tNOUN\t_\t_\t0\troot\t_\tTokenRange={m.start()}:{m.end()}")
        return "\n".join(linhas) + "\n\n"

    modelo = ModeloUDPipe2()
    monkeypatch.setattr(modelo, "_processar", conllu_falso)
    saidas = modelo.analisar(["a casa", "", "o rio azul"])
    assert [[(w.texto, w.ini, w.fim) for w in s] for s in saidas] == [
        [("a", 0, 1), ("casa", 2, 6)], [], [("o", 0, 1), ("rio", 2, 5), ("azul", 6, 10)]]


def test_acesso_local_nao_implementado_e_mensagem_clara():
    import pytest
    with pytest.raises(NotImplementedError):
        ModeloUDPipe2(acesso="local")

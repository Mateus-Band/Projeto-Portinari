import numpy as np
import os

'''
O código a seguir seleciona uma paleta para um quadro especifico por meio do Bin method

A idéia é pegar um conjunto de pixels no espaço RGB, e aplicar o algoritimo

O algoritmo consiste em:

Verifica, entre todos os conjuntos de dados existentes, em qual eixo o conjunto tem a maior variação (range) de dados.

Pega o ponto com a mediana observando apenas esse eixo.

Divide o conjunto em dois, o ponto mediana irá para o lado com a menor range

Se com essa divisão a quantidade de cores escolhida for igual a quantidade de conjuntos formados pare.

Caso contrario: 
    repita o processo.


'''

def quantizacao(Data):
    '''Quantiza as cores, basicamente reduz a quantidade de variação existente nas cores'''
    return Data//8


def Binacao(Data_Bin):
    '''
    Data_Bin é uma lista de lista, em que cada elemento é um conjunto.
    '''



    #lista dos ranges
    ranges = []
    #encontra o eixo com maior range para cada conjunto
    for conjunto in Data_Bin:
        RGB = np.array(list(zip(*conjunto)))
        R = RGB[0]
        G = RGB[1]
        B = RGB[2]

        range_list = [max(R)-min(R),max(G)-min(G),max(B)-min(B)]
        
        maior_range = (max(range_list),range_list.index(max(range_list)))
        ranges.append(maior_range)


    #verifica qual conjunto tem o maior range
    def tuple_max(tupla):
        a,b = tupla
        return a
    
    vari,axe = max(ranges,key = tuple_max)
    conjun_idx = ranges.index((vari,axe))

    #aqui temos a informação indice do conjunto que será particionado (conjun_idx), e o eixo (axe)

    #pega a mediana do conjunto 
    setn = np.array(Data_Bin[conjun_idx])


    eixo = setn[:,axe]
    mediana = np.median(eixo)

    #criando os conjuntos
    left_Bin = setn[ eixo > mediana]
    right_Bin = setn[eixo < mediana]

    equals = setn[eixo == mediana]

    #escolhendo qual o conjunto desempate
    left_range,right_range = (mediana - min(setn[axe]), max(setn[axe]) -  mediana)
    if left_range > right_range:
        np.concatenate((left_Bin,equals),axis=0)
    else:
        np.concatenate((right_Bin,equals),axis = 0)

    #deleta os subconjuntos
    Data_Bin.pop(conjun_idx)
    
    #adiciona os novos subconjuntos 
    Data_Bin.append(left_Bin)
    Data_Bin.append(right_Bin)
    

    return Data_Bin
    

def Bin_method(Data,clusters_numb):
    Data_Bined = [Data]
    while(len(Data_Bined) != clusters_numb):
        Data_Bined = Binacao(Data_Bined)


    return Data_Bined


def testezim():
    import random

    a = [[random.randrange(3),random.randrange(3),random.randrange(3)] for i in range(10)]

    print(f'O conjunto de dados inicial: {a}', end='\n\n')
    print(f'Os subconjuntos formados pela função:')
    for idx,conjunto in enumerate(Bin_method(a,3)):
        print(f'Conjunto {idx} : {conjunto}', end='\n\n')


if __name__ == '__main__':
    testezim()

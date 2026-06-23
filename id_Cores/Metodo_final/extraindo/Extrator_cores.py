'''
Esse documento é arquivo final de muitas tentativas de extrair uma paleta de cores de imagens, mais especificamente quadros do artista portinari.

Para isso será utlizado um algoritimo conhecido como mean shift, sobre todos os pixels das imagens.

O arquivo deve iterar sobre as imagens escolhidas e escrever as cores/paleta em uma tabela pandas, adicionando metadata ao conjunto de dados a cerca das imagens.


'''


import os
import cv2
from funcionalidades import cor_predominante
import pandas as pd






#apenas para trocar os rgb de lugar
def rgbaiter(cor):
    b= cor[0]
    g= cor[1]
    r = cor[2]
    return [r,g,b]




#caminho do diretorio onde as imagens estão
img_dir = ''

tabela_dados = '/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/obras.csv'




tabela  = pd.read_csv(tabela_dados)





#itera sobre as imagens 
for idx,img in enumerate(cam_imagens):
    #juntando o nome do arquivo da imagem para o caminho do folder das imagens
    caminho_img = os.path.join(img_dir,img)

    #le as imagens coletando os pixels
    imagem = cv2.imread(caminho_img)

    #deixa todos os pixels em fila
    imagem = imagem.reshape((-1,3))

    #printa o quanto por cento das imagens já foram processadas
    print()

    #usa a funcao para retornar a paleta e a proporcao
    #ATENCAO a cor aqui sai no formato bgr
    cor,qtd = cor_predominante(imagem)
    cor = list(rgbaiter(c) for c in cor)

    #salva as informações na tabela

    cores_nov.append(cor)
    proporcoes_nov.append(qtd)
    nomes_nov.append(img)

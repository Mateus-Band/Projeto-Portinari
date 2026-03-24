import cv2
import random
import numpy as np


def media_colors(caminho_imagem, n_pixels = 30):
    #carregando a imagem
    imagem = cv2.imread(caminho_imagem)

    if imagem is None:
        print("Erro: Não foi possível carregar a imagem.")
        return None
    


    '''
    Irei pegar pontos aleatorios para com eles tentar ter as cores resumidas da imagem
    
    com elas irei, fazer a média das cores para tentar encontrar alguma informação, não estou tão preocupado com essa parte

    Irei também tentar pegar a cor com mais aparições, para isso terei de definir faixas de cores para cada cor o que farei com base no meu ponto de vista
    
    
    '''

    pixels = []


    b = int(0)
    g = int(0)
    r = int(0)

    #cada vez escolher um inteiro aleatório para o eixo x e eixo y 
    for i in range(n_pixels):
        pixel = list(imagem[int(random.uniform(0,imagem.shape[0])),int(random.uniform(0,imagem.shape[1]))])

        #para a media
        b += int(pixel[0])
        g += int(pixel[1])
        r += int(pixel[2])

        #guardando
        pixels.append(pixel)


    media = [r/n_pixels,g/n_pixels,b/n_pixels]

    print(f'A Cor média retirada da imagem é :{media}')

def is_in(n,intervalo):
    if len(intervalo) != 2:
        print('ERRO: o intervalo deve conter apenas dois valores')
        return False

    a = intervalo[0]
    b = intervalo[1]

    if a > b:
        return False

    if n <= b and n >= a:
        return True
    else:
        return False


def how_color(r,g,b):
    '''
    Vermelho - 255,0,0 - 255, 68, 0
    laranja -  255 , 68, 0 -255,180,0 
    amarelo - 255,180,0 - 213,255,0
    verde - 213,255,0 - 0,255,94
    azul claro  - 0, 255, 94  - 0 , 128, 255
    azul escuro - 0 , 128, 255 - 34, 0, 255
    roxo - 205, 0 , 255 - 255 , 0 , 85
    
    '''

    ''' HSV
    Vermelho - [334,50,60] - [13,100,100]
    Laranja  - [13,50,60] - [36,100,100]
    amarelo - [36,...] - [57,...]
    verde claro (grama) - [57,...] -[95,..]
    Verde - [95,...] - [137,...]
    Azul claro - [137,...] - [208,...]
    Azul escuro - [208,...] - [239,...]
    Roxo - [239,...] - [334,100,80]
    rosa - igual o roxo mas acima de 80

    a baixo de 60 no ultimo campo é considerada a cor mas escura
    branco é a baixo do 50 , cinza e branco escuro

    preto é o ultimo campo menor que 10
    '''



    color_hsv = cv2.cvtColor(np.uint8([[[b, g, r]]]), cv2.COLOR_BGR2HSV)[0][0]

    
    h = color_hsv[0]
    s = color_hsv[1]
    v = color_hsv[2]



    if is_in(v, [0, 25]):
        return 'Preto'

    # Branco / Cinza
    if is_in(s, [0, 25]):
        if is_in(v, [0, 153]):
            return 'Cinza'
        else:
            return 'Branco'

    # Vermelho
    if (is_in(h, [0, 6]) or is_in(h, [167, 179])) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Vermelho'
        if v <= 153:
            return 'Vermelho escuro'

    # Laranja
    if is_in(h, [6, 18]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Laranja'
        if v <= 153:
            return 'Laranja escuro'

    # Amarelo
    if is_in(h, [18, 28]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Amarelo'
        if v <= 153:
            return 'Amarelo escuro'

    # Verde claro
    if is_in(h, [28, 47]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Verde Claro'
        if v <= 153:
            return 'Verde Claro escuro'

    # Verde
    if is_in(h, [47, 68]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Verde'
        if v <= 153:
            return 'Verde escuro'

    # Azul claro
    if is_in(h, [68, 104]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Azul Claro'
        if v <= 153:
            return 'Azul Claro escuro'

    # Azul escuro
    if is_in(h, [104, 119]) and is_in(s, [25, 255]):
        if is_in(v, [153, 255]):
            return 'Azul Escuro'
        if v <= 153:
            return 'Azul Escuro escuro'

    # Roxo
    if is_in(h, [119, 167]) and is_in(s, [25, 255]):
        if is_in(v, [153, 204]):
            return 'Roxo'
        if v <= 153:
            return 'Roxo escuro'

    # Rosa
    if is_in(h, [119, 167]) and is_in(s, [25, 255]):
        if is_in(v, [204, 255]):
            return 'Rosa'

    return 'Indefinido'


def most_collor(caminho_imagem,n_pixels = 10**4):

    #carregando a imagem
    imagem = cv2.imread(caminho_imagem)

    if imagem is None:
        print("Erro: Não foi possível carregar a imagem.")
        return None
    

    #contador de quantidade que a cor apareceu
    cores_count = {
        "Preto": 0,
        "Branco": 0,
        "Cinza": 0,
        "Vermelho": 0,
        "Vermelho escuro": 0,
        "Laranja": 0,
        "Laranja escuro": 0,
        "Amarelo": 0,
        "Amarelo escuro": 0,
        "Verde Claro": 0,
        "Verde Claro escuro": 0,
        "Verde": 0,
        "Verde escuro": 0,
        "Azul Claro": 0,
        "Azul Claro escuro": 0,
        "Azul Escuro": 0,
        "Azul Escuro escuro": 0,
        "Roxo": 0,
        "Roxo escuro": 0,
        "Rosa": 0,
        "Rosa escuro": 0,
        "Indefinido": 0
    }


    #loop sobre os pontos
    for i in range(n_pixels):
        pixel = list(imagem[int(random.uniform(0,imagem.shape[0])),int(random.uniform(0,imagem.shape[1]))])

        #para a media
        b = int(pixel[0])
        g = int(pixel[1])
        r = int(pixel[2])

        #função que retorna qual cor com base na faxa de cores que eu defini
        color = how_color(r,g,b)

        #realiza a contagem
        cores_count[color] += 1


    #printa a cor que mais aparece
    print(f'A cor mais predominante na imagem é {max(cores_count,key = cores_count.get)}')





#pegando imagem com caminho começando com 14886
caminho_imagem = 'output/imagens/14886_Colheita_de_Caf_.jpeg'
media_colors(caminho_imagem,50000)
most_collor(caminho_imagem,50000)

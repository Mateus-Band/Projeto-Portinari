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

def how_color2(r,g,b):
    
    color_hsv = cv2.cvtColor(np.uint8([[[b, g, r]]]), cv2.COLOR_BGR2HSV)[0][0]
    
    h = color_hsv[0]
    s = color_hsv[1]
    v = color_hsv[2]


    
    if v < 30: return 'Preto'
    if s < 40:
        if v > 200: return 'Branco'
        return 'Cinza'


    if h < 5 or h >= 165:   color = 'Vermelho'
    elif h < 15:           color = 'Laranja'
    elif h < 25:           color = 'Amarelo'
    elif h < 45:           color = 'Verde Grama'
    elif h < 75:           color = 'Verde'
    elif h < 95:           color = 'Ciano'
    elif h < 125:          color = 'Azul'
    elif h < 145:          color = 'Roxo'
    elif h < 165:          color = 'Rosa'
    else:                  return 'Indefinido'

    if ((color == 'Laranja') and (v < 160)):
        color = 'Marrom' 

    if ((color == 'Amarelo') and (v < 160)):
        color = 'Palha' 


    if v < 100:
        return f'{color} Escuro'
    elif v > 220 and s < 100:
        return f'{color} Claro'
    
    return color

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



    if is_in(v, [0, 8]):
        return 'Preto'

    # Branco / Cinza
    if is_in(s, [0, 10.2]):
        if is_in(v, [8, 153]):
            return 'Cinza'
        else:
            return 'Branco'

    # Vermelho
    if (is_in(h, [0, 6]) or is_in(h, [167, 179])) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Vermelho'
        if v <= 102:
            return 'Vermelho escuro'

    # Laranja
    if is_in(h, [6, 18]) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Laranja'
        if v <= 102:
            return 'Laranja escuro'

    # Amarelo
    if is_in(h, [18, 28]) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Amarelo'
        if v <= 102:
            return 'Amarelo escuro'

    # Verde claro
    if is_in(h, [28, 47]) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Verde Grama'
        if v <= 102:
            return 'Verde Grama escuro'

    # Verde
    if is_in(h, [47, 68]) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Verde'
        if v <= 102:
            return 'Verde escuro'

    # Azul claro
    if is_in(h, [68, 104]) and is_in(s, [60, 255]):
        if is_in(v, [102, 255]):
            return 'Azul Claro'
        if v <= 102:
            return 'Azul Claro escuro'

    # Azul escuro
    if is_in(h, [104, 119]) and is_in(s, [60, 255]):
        if is_in(v, [153, 255]):
            return 'Azul Escuro'
        if v <= 153:
            return 'Azul Escuro escuro'

    # Roxo
    if is_in(h, [119, 167]) and is_in(s, [60, 255]):
        if is_in(v, [153, 204]):
            return 'Roxo'
        if is_in(v, [204, 255]):
            return 'Rosa'

        if v <= 153:
            return 'Roxo escuro'
        

    return 'Indefinido'


def most_collor2(caminho_imagem,n_pixels = 10**4):

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
        "Laranja": 0,
        "Amarelo": 0,
        "Verde Grama": 0,
        "Verde": 0,
        "Ciano": 0,
        "Azul": 0,
        "Roxo": 0,
        "Rosa": 0,

        "Marrom":0,
        "Palha":0,

        "Vermelho Escuro": 0,
        "Laranja Escuro": 0,
        "Amarelo Escuro": 0,
        "Verde Grama Escuro": 0,
        "Verde Escuro": 0,
        "Ciano Escuro": 0,
        "Azul Escuro": 0,
        "Roxo Escuro": 0,
        "Rosa Escuro": 0,
        "Marrom Escuro":0,
        "Palha Escuro":0,

        "Vermelho Claro": 0,
        "Laranja Claro": 0,
        "Amarelo Claro": 0,
        "Verde Grama Claro": 0,
        "Verde Claro": 0,
        "Ciano Claro": 0,
        "Azul Claro": 0,
        "Roxo Claro": 0,
        "Rosa Claro": 0,

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
        color = how_color2(r,g,b)

        #realiza a contagem
        if color in cores_count:
            cores_count[color] += 1
        else:
            cores_count['Indefinido'] += 1

    #printa a cor que mais aparece
    return max(cores_count,key = cores_count.get)


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
        "Verde Grama": 0,
        "Verde Grama escuro": 0,
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
    return max(cores_count,key = cores_count.get)







if __name__ == '__main__':
    media_colors('quadro_branco.png',10000)
    print(most_collor('quadro_azul.png',10000))
    print(most_collor2('quadro_azul.png',10000))

library(grip)

#le o arquivo csv
dados <- read.csv("/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/frentes/poemas/descritiva/grafo/GraphLike.csv") # Confirme se este é o nome exato do seu arquivo

# extrai a lista de palavras unicas do grafo
nos_unicos <- unique(c(as.character(dados$no_A), as.character(dados$no_B)))
n <- length(nos_unicos) # Este é o 'n' que o pacote estava reclamando que faltava

# converte as palavras para índices numéricos
edges <- cbind(
  match(dados$no_A, nos_unicos),
  match(dados$no_B, nos_unicos)
)

#aplica o grip
cat("Calculando layout...\n")
coords <- grip(edges = edges, n = n)

#reescrevendo o resultado
resultado <- data.frame(
    Palavra = nos_unicos, 
    X = coords[,1], 
    Y = coords[,2]
)

# salva
write.csv(resultado, "coordenadas_finais.csv", row.names=FALSE)
cat("Pronto! Arquivo coordenadas_finais.csv gerado com sucesso.\n")
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    int id;
    int quantidade;
    float preco;
} ItemDaVenda;

FILE *produtos = fopen("produtos.txt", "a+");

float encontrarPreco(int id) {
    struct Produto produto;
    while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
        if (id == produto.id) {
            return produto.preco;
        }
    }
    return 0;
}

void motorVendas() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);

    setlocale(LC_ALL, "Portuguese");
    printf("\nTem certeza de que quer iniciar o motor de vendas? (S/N): ");
    char resposta;
    scanf(" %c", &resposta);
    resposta = toupper(resposta);
    if (resposta != 'S') {
        return;
    }
    printf("\nData atual: %s\nIniciando motor de vendas: %s\n", buffer);
    FILE *relatoriovendas = fopen(strcat(buffer, ".txt"), "a+");
    char saida = 'n';
    do {
        printf("\nDeseja iniciar uma nova venda?");
        scanf(" %c", &saida);
        if (toupper(saida) != 'N') {
            int contadorvetor = 0;
            struct ItemDaVenda venda[30];
            char continuarvenda = 'S';
            do {
                int idproduto, quantidade;
                float precovenda;
                printf("\nInsira o produto a ser vendido: ");
                scanf("%d", &idproduto);
                struct Produto produto;
                int encontrado = 0;
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (idproduto == produto.id) {
                        encontrado == 1;
                    }
                }
                if (!encontrado) {
                    printf("\nProduto inválido!");
                    continue;
                } else {
                    printf("\nQuantos deste produto serão vendidos? ");
                    scanf("%d", &quantidade);
                    precovenda = quantidade * encontrarPreco(idproduto);
                    venda[contadorvetor].id = idproduto;
                    venda[contadorvetor].quantidade = quantidade;
                    venda[contadorvetor].preco = precovenda;
                    contadorvetor++;
                }
                printf("\nDeseja continuar a venda? (S/N) ");
                scanf(" %c", &continuarvenda);
            } while (toupper(continuarvenda) != 'N');
        }
    } while (toupper(saida) != 'N');
}
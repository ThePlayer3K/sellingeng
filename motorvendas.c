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

char metpagamento[5] = {'$', 'C', 'D', 'P', 'A'};

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
    strftime(buffer, sizeof(buffer), "%d%m%Y", timeinfo);
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
                int erroproduto = 0;
                do {
                    printf("\nInsira o produto a ser vendido: ");
                    erroproduto = scanf("%d", &idproduto);
                    if (erroproduto != 1) {
                        printf("\nErro ao registar ID de produto. Tente novamente");
                    } else {
                        char nomeproduto[76];
                        while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                            if (idproduto == produto.id) {
                                produto.nome = nomeproduto; 
                            }
                        }
                        printf("\nO produto %s está correto? (S/N)", nomeproduto);
                        char verificarproduto;
                        erroproduto = scanf( "%c", &verificarproduto);
                        if (toupper(verificarproduto) != 'S') {
                            printf("\nProduto cancelado");
                            erroproduto = 0;
                        } else if (erroproduto != 1) printf("\nErro ao obter confirmação");
                    }
                } while (erroproduto != 1);
                struct Produto produto;
                int encontrado = 0;
                while (fscanf(produtos, "%d|%76[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
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
            } while (toupper(continuarvenda) != 'N' || contadorvetor <= 30);
            if (contadorvetor > 30) printf("\nLimite de itens por venda excedido!");
            char metododavenda;
            int erropagamento = 0;
            do {
                printf("\nSelecione um meio de pagamento usando um caractere: ");
                printf("\n($) Dinheiro");
                printf("\n(C) Crédito");
                printf("\n(D) Débito");
                printf("\n(A) Vale-Alimentação");
                scanf(" %c", &metododavenda);
                metododavenda = toupper(metododavenda);
                for (int i = 0; i < 5; i++) {
                    if (metododavenda == metpagamento[i]) erropagamento == 1;
                }
                if (erropagamento != 1) {
                    printf("\nErro ao obter método de pagamento, tente novamente!");
                    continue;
                } else {
                    printf("\nVocê tem certeza do método de pagamento? (S/N)");
                    char confirmacaometodo;
                    scanf(" %c", &confirmacaometodo);
                    if (toupper(confirmacaometodo) != 'S') erropagamento = 0;
                }
            } while (erropagamento != 1);
            for (int i = 0; i < contadorvetor; i++) {
                fprintf(relatoriovendas, "%d|%d|%f|%c\n", ItemDaVenda[i].id, ItemDaVenda[i].quantidade, ItemDaVenda[i].preco, metododavenda);
            }
            printf("\nVenda registrada com sucesso!");
            system("pause");
            system("cls");   
        } else {
            printf("\nTem certeza que quer finalizar o dia de vendas? (S/N)? ");
            scanf(" %c", &saida);
        }
    } while (toupper(saida) != 'N');
    printf("\nDia de vendas finalizado com sucesso! Você pode utilizar a função de visualizar relatórios de vendas diárias");
    system("pause");
    system("cls");
}
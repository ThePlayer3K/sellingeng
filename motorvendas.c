#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "motorvendas.h"

struct Produto {
    int id;
    char nome[76];
    float preco;
}; 

struct ItemDaVenda{
    int id;
    int quantidade;
    float preco;
};

char metpagamento[5] = {'$', 'C', 'D', 'P', 'A'};

float encontrarPreco(int id) {
    FILE *produtos = fopen("produtos.txt", "r");
    if (produtos == NULL) return 0;
    
    struct Produto produto;
    while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
        if (id == produto.id) {
            fclose(produtos);
            return produto.preco;
        }
    }
    fclose(produtos);
    return 0;
}

void motorVendas() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);

    printf("\nAre you sure you want to start the sales engine? (Y/N): ");
    char resposta;
    scanf(" %c", &resposta);
    resposta = toupper(resposta);
    if (resposta != 'S') {
        return;
    }
    printf("\nCurrent date: %s\nStarting sales engine\n", buffer);
    strftime(buffer, sizeof(buffer), "%d%m%Y", timeinfo);
    strcat(buffer, ".txt");
    FILE *relatoriovendas = fopen(buffer, "a+");
    if (relatoriovendas == NULL) {
        printf("Error opening report file!\n");
        return;
    }
    
    char saida = 'n';
    do {
        printf("\nDo you want to start a new sale? (Y/N): ");
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
                    printf("\nEnter the ID of the product to be sold: ");
                    erroproduto = scanf("%d", &idproduto);
                    if (erroproduto != 1) {
                        printf("\nError registering product ID. Try again\n");
                        while(getchar() != '\n');
                    } else {
                        FILE *produtos = fopen("produtos.txt", "r");
                        if (produtos == NULL) {
                            printf("\nError opening products file!\n");
                            erroproduto = 0;
                            continue;
                        }
                        struct Produto produto;
                        char nomeproduto[76] = "";
                        int produtoEncontrado = 0;
                        while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                            if (idproduto == produto.id) {
                                strcpy(nomeproduto, produto.nome);
                                produtoEncontrado = 1;
                                break;
                            }
                        }
                        fclose(produtos);
                        if (!produtoEncontrado) {
                            printf("\nProduct not found!\n");
                            erroproduto = 0;
                            continue;
                        }
                        printf("\nIs the product %s correct? (Y/N): ", nomeproduto);
                        char verificarproduto;
                        scanf(" %c", &verificarproduto);
                        if (toupper(verificarproduto) != 'Y') {
                            printf("\nProduct cancelled\n");
                            erroproduto = 0;
                        }
                    }
                } while (erroproduto != 1);
                int verificarquantidade = 0;
                do {
                    printf("\nHow many of this product will be sold? ");
                    verificarquantidade = scanf("%d", &quantidade);
                    if (verificarquantidade != 1 || quantidade < 0) {
                        verificarquantidade = 0;
                        printf("\nInvalid quantity. Try again.");
                    }
                } while (verificarquantidade != 1);
                precovenda = quantidade * encontrarPreco(idproduto);
                if (contadorvetor >= 30) {
                    printf("\nItem limit per sale exceeded!\n");
                    break;
                } else {
                    venda[contadorvetor].id = idproduto;
                    venda[contadorvetor].quantidade = quantidade;
                    venda[contadorvetor].preco = precovenda;
                    contadorvetor++;
                }
                printf("\nDo you want to continue the sale? (Y/N): ");
                scanf(" %c", &continuarvenda);
            } while (toupper(continuarvenda) != 'N');
            
            char metododavenda;
            int pagamentovalido = 0;
            do {
                printf("\nSelect a payment method using a character: ");
                printf("\n($) Cash");
                printf("\n(C) Credit");
                printf("\n(D) Debit");
                printf("\n(A) Food Voucher");
                printf("\n(P) Pix\n");
                printf("\nYour answer: ");
                scanf(" %c", &metododavenda);
                metododavenda = toupper(metododavenda);
                pagamentovalido = 0;
                for (int i = 0; i < 5; i++) {
                    if (metododavenda == metpagamento[i]) {
                        pagamentovalido = 1;
                        break;
                    }
                }
                if (pagamentovalido != 1) {
                    printf("\nError getting payment method, try again!\n");
                    continue;
                } else {
                    printf("\nAre you sure about the payment method? (Y/N): ");
                    char confirmacaometodo;
                    scanf(" %c", &confirmacaometodo);
                    if (toupper(confirmacaometodo) != 'Y') pagamentovalido = 0;
                }
            } while (pagamentovalido != 1);
            
            for (int i = 0; i < contadorvetor; i++) {
                fprintf(relatoriovendas, "%d|%d|%.2f|%c\n", venda[i].id, venda[i].quantidade, venda[i].preco, metododavenda);
            }
            printf("\nSale registered successfully!\n");
            system("pause");
            system("cls");   
        } else {
            printf("\nAre you sure you want to end the sales day? (Y/N)? ");
            char confirmarsaida;
            scanf(" %c", &confirmarsaida);
            confirmarsaida = toupper(confirmarsaida);
            if (confirmarsaida != 'Y') saida = 'N';
        }
    } while (toupper(saida) != 'N');
    
    fclose(relatoriovendas);
    printf("\nSales day successfully ended! You can use the function to view daily sales reports\n");
    system("pause");
    system("cls");
}
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

    setlocale(LC_ALL, "Portuguese");
    printf("\nTem certeza de que quer iniciar o motor de vendas? (S/N): ");
    char resposta;
    scanf(" %c", &resposta);
    resposta = toupper(resposta);
    if (resposta != 'S') {
        return;
    }
    printf("\nData atual: %s\nIniciando motor de vendas\n", buffer);
    strftime(buffer, sizeof(buffer), "%d%m%Y", timeinfo);
    strcat(buffer, ".txt");
    FILE *relatoriovendas = fopen(buffer, "a+");
    if (relatoriovendas == NULL) {
        printf("Erro ao abrir arquivo de relatório!\n");
        return;
    }
    
    char saida = 'n';
    do {
        printf("\nDeseja iniciar uma nova venda? (S/N): ");
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
                    printf("\nInsira o ID do produto a ser vendido: ");
                    erroproduto = scanf("%d", &idproduto);
                    if (erroproduto != 1) {
                        printf("\nErro ao registar ID de produto. Tente novamente\n");
                        while(getchar() != '\n');
                    } else {
                        FILE *produtos = fopen("produtos.txt", "r");
                        if (produtos == NULL) {
                            printf("\nErro ao abrir arquivo de produtos!\n");
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
                            printf("\nProduto não encontrado!\n");
                            erroproduto = 0;
                            continue;
                        }
                        
                        printf("\nO produto %s está correto? (S/N): ", nomeproduto);
                        char verificarproduto;
                        scanf(" %c", &verificarproduto);
                        if (toupper(verificarproduto) != 'S') {
                            printf("\nProduto cancelado\n");
                            erroproduto = 0;
                        }
                    }
                } while (erroproduto != 1);
                
                int verificarquantidade = 0;
                do {
                    printf("\nQuantos deste produto serão vendidos? ");
                    verificarquantidade = scanf("%d", &quantidade);
                    if (verificarquantidade != 1 || quantidade < 0) {
                        verificarquantidade = 0;
                        printf("\nQuantidade inválida. Tente novamente.");
                    }
                } while (verificarquantidade != 1);
                precovenda = quantidade * encontrarPreco(idproduto);
                
                if (contadorvetor >= 30) {
                    printf("\nLimite de itens por venda excedido!\n");
                    break;
                } else {
                    venda[contadorvetor].id = idproduto;
                    venda[contadorvetor].quantidade = quantidade;
                    venda[contadorvetor].preco = precovenda;
                    contadorvetor++;
                }
                
                printf("\nDeseja continuar a venda? (S/N): ");
                scanf(" %c", &continuarvenda);
            } while (toupper(continuarvenda) != 'N');
            
            char metododavenda;
            int pagamentovalido = 0;
            do {
                printf("\nSelecione um meio de pagamento usando um caractere: ");
                printf("\n($) Dinheiro");
                printf("\n(C) Crédito");
                printf("\n(D) Débito");
                printf("\n(A) Vale-Alimentação");
                printf("\n(P) Pix\n");
                printf("\nSua resposta: ");
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
                    printf("\nErro ao obter método de pagamento, tente novamente!\n");
                    continue;
                } else {
                    printf("\nVocê tem certeza do método de pagamento? (S/N): ");
                    char confirmacaometodo;
                    scanf(" %c", &confirmacaometodo);
                    if (toupper(confirmacaometodo) != 'S') pagamentovalido = 0;
                }
            } while (pagamentovalido != 1);
            
            for (int i = 0; i < contadorvetor; i++) {
                fprintf(relatoriovendas, "%d|%d|%.2f|%c\n", venda[i].id, venda[i].quantidade, venda[i].preco, metododavenda);
            }
            printf("\nVenda registrada com sucesso!\n");
            system("pause");
            system("cls");   
        } else {
            printf("\nTem certeza que quer finalizar o dia de vendas? (S/N)? ");
            char confirmarsaida;
            scanf(" %c", &confirmarsaida);
            confirmarsaida = toupper(confirmarsaida);
            if (confirmarsaida != 'S') saida = 'N';
        }
    } while (toupper(saida) != 'N');
    
    fclose(relatoriovendas);
    printf("\nDia de vendas finalizado com sucesso! Você pode utilizar a função de visualizar relatórios de vendas diárias\n");
    system("pause");
    system("cls");
}
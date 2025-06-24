#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
#include "produtos.h"


struct Produto {
    int id;
    char nome[76];
    float preco;
};

int verificarPipe(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '|') {
            return 1;
        }
        i++;
    }
    return 0; 
}

void produtos() {
    setlocale(LC_ALL, "Portuguese");
    char saidaprod = 'n';
    do {
        system("cls");
        printf("\nBem-vindo ao menu de gerenciamento de produtos! Escolha o que deseja fazer: ");
        printf("\n(C) Cadastrar Produto");
        printf("\n(L) Listar produtos");
        printf("\n(E) Editar produto");
        printf("\n(X) Excluir produto");
        printf("\n(S) Sair");
        printf("\nSua opção: ");
        char opcaoprod;
        scanf(" %c", &opcaoprod);
        opcaoprod = toupper(opcaoprod);
        switch(opcaoprod) {
            case 'C': {
                FILE *produtos = fopen("produtos.txt", "a+");
                if (!produtos) {
                    printf("\nErro ao carregar arquivo de produtos.");
                    break;
                }
                struct Produto produto;
                int erroid = 0;
                do {
                    printf("\nDigite um ID para o produto (anote-o em algum lugar): ");
                    erroid = scanf("%d", &produto.id);
                    if (erroid != 1) printf("\nErro! Digite um ID numérico adequado");
                } while (erroid != 1);
                int erronome = 0, erropipe = 1;
                do {
                    printf("\nDigite o nome do produto: ");
                    scanf(" %75[^\n]", produto.nome);
                    erropipe = verificarPipe(produto.nome);
                    if (erropipe != 0) {
                        printf("\nErro, digite novamente o nome do produto!");
                    } else {
                        erronome = 1;
                    }
                } while (erronome != 1);
                int erropreco = 0;
                do {
                    printf("\nDigite o preço do produto: ");
                    erropreco = scanf("%f", &produto.preco);
                    if (erropreco != 1) printf("\nErro! Digite o preço do produto em reais novamente!");
                } while (erropreco != 1);
                fprintf(produtos, "%d|%s|%.2f\n", produto.id, produto.nome, produto.preco);
                fclose(produtos);
                printf("\nProduto cadastrado com sucesso!");
                break;
            }
            case 'L': {
                FILE *produtos = fopen("produtos.txt", "r");
                struct Produto produto;
                printf("\nLista de produtos");
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    printf("ID: %d, nome: %s, preço: %.2f\n", produto.id, produto.nome, produto.preco);
                }
                system("pause");
                fclose(produtos);
                break;
            }
            case 'E': {
                FILE *produtos = fopen("produtos.txt", "r+");
                if (!produtos) {
                    printf("\nErro ao abrir arquivo de produtos.");
                    break;
                }

                int idAlvo;
                printf("\nDigite o ID do produto a ser editado: ");
                scanf("%d", &idAlvo);
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
                if (!temp) {
                    printf("\nErro ao abrir arquivo temporário");
                    break;
                }
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (produto.id == idAlvo) {
                        int erronome = 0, erropipe = 1;
                        do {
                            printf("\nDigite o novo nome do produto (ou -1 para não alterar): ");
                            char novoNome[76];
                            erronome = scanf( " %75[^\n]", novoNome);
                            erropipe = verificarPipe(novoNome);
                            if (strcmp(novoNome, "-1") != 0) break;
                            if (strlen(novoNome) > 0 && strcmp(novoNome, "-1") != 0) strcpy(produto.nome, novoNome);
                            if (erronome != 1 || erropipe != 0) printf("\nErro ao registrar novo nome! tente novamente!");
                        } while (erronome != 1);
                        int erropreco = 0;
                        do {
                            printf("\nDigite o novo preço do produto (ou -1 para não alterar): ");
                            float novoPreco;
                            erropreco = scanf("%f", &novoPreco);
                            if (novoPreco == -1) break;
                            if (novoPreco > 0) produto.preco = novoPreco;
                            if (erropreco != 1 || novoPreco < 0) printf("\nErro ao registrar novo preço! Digite novamente o preço em reais");
                        } while (erropreco != 1);
                        encontrado = 1;
                    }
                    fprintf(temp, "%d|%s|%.2f\n", produto.id, produto.nome, produto.preco);
                }
                fclose(temp);
                fclose(produtos);
                remove("produtos.txt");
                rename("temp.txt", "produtos.txt");
                if (encontrado) {
                    printf("\nProduto editado com sucesso!");
                } else {
                    printf("\nProduto não encontrado.");
                }
                break;
            }
            case 'X': {
                FILE *produtos = fopen("produtos.txt", "r");
                if (!produtos) {
                    printf("\nErro ao abrir arquivo de produtos.");
                    break;
                }
                int idAlvo;
                printf("\nInforme o ID do produto a ser excluído:");
                scanf("%d", &idAlvo);
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
                if (!temp) {
                    printf("\nErro ao abrir arquivo temporário.");
                    break;
                }
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (produto.id == idAlvo) {
                        printf("\nDeseja mesmo exlcluir o produto %s? (S/N): ", produto.nome);
                        char confirmacao;
                        scanf(" %c", &confirmacao);
                        if (toupper(confirmacao) == 'S') {
                            encontrado = 2;
                        } else {
                            encontrado = 1;
                        }
                    } else {
                        fprintf(temp, "%d|%s|%.2f\n", produto.id, produto.nome, produto.preco);
                    }
                }
                fclose(temp);
                fclose(produtos);
                if (encontrado == 2) {
                    remove("produtos.txt");
                    rename("temp.txt", "produtos.txt");
                    printf("\nProduto excluído com sucesso!");
                } else if (encontrado == 1) {
                    printf("\nProduto não excluído.");
                } else {
                    printf("\nProduto não encontrado.");
                }
                break;
            }
            case 'S':
                saidaprod = 'S';
                break;
            default:
                printf("\nOpção inválida, tente novamente.");

        }
    } while (toupper(saidaprod) != 'S');
}
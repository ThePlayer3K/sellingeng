// ENGLISH TRANSLATION

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
        printf("\nWelcome to the product management menu! Choose what you want to do: ");
        printf("\n(C) Register Product");
        printf("\n(L) List products");
        printf("\n(E) Edit product");
        printf("\n(X) Delete product");
        printf("\n(S) Exit");
        printf("\nYour option: ");
        char opcaoprod;
        scanf(" %c", &opcaoprod);
        opcaoprod = toupper(opcaoprod);
        switch(opcaoprod) {
            case 'C': {
                FILE *produtos = fopen("produtos.txt", "a+");
                if (!produtos) {
                    printf("\nError loading product file.");
                    break;
                }
                struct Produto produto;
                int erroid = 0;
                do {
                    printf("\nEnter an ID for the product (write it down somewhere): ");
                    erroid = scanf("%d", &produto.id);
                    if (erroid != 1) printf("\nError! Enter a valid numeric ID");
                } while (erroid != 1);
                int erronome = 0, erropipe = 1;
                do {
                    printf("\nEnter the product name: ");
                    scanf(" %75[^\n]", produto.nome);
                    erropipe = verificarPipe(produto.nome);
                    if (erropipe != 0) {
                        printf("\nError, please enter the product name again!");
                    } else {
                        erronome = 1;
                    }
                } while (erronome != 1);
                int erropreco = 0;
                do {
                    printf("\nEnter the product price: ");
                    erropreco = scanf("%f", &produto.preco);
                    if (erropreco != 1) printf("\nError! Enter the product price again!");
                } while (erropreco != 1);
                fprintf(produtos, "%d|%s|%.2f\n", produto.id, produto.nome, produto.preco);
                fclose(produtos);
                printf("\nProduct registered successfully!");
                break;
            }
            case 'L': {
                FILE *produtos = fopen("produtos.txt", "r");
                struct Produto produto;
                printf("\nProduct list:\n");
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    printf("ID: %d, name: %s, price: %.2f\n", produto.id, produto.nome, produto.preco);
                }
                system("pause");
                fclose(produtos);
                break;
            }
            case 'E': {
                FILE *produtos = fopen("produtos.txt", "r+");
                if (!produtos) {
                    printf("\nError opening product file.");
                    break;
                }

                int idAlvo;
                printf("\nEnter the ID of the product to be edited: ");
                scanf("%d", &idAlvo);
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
                if (!temp) {
                    printf("\nError opening temporary file");
                    break;
                }
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (produto.id == idAlvo) {
                        int erronome = 0, erropipe = 1;
                        do {
                            printf("\nEnter the new product name (or -1 to keep unchanged): ");
                            char novoNome[76];
                            erronome = scanf( " %75[^\n]", novoNome);
                            erropipe = verificarPipe(novoNome);
                            if (erronome != 1 || erropipe != 0 || strlen(novoNome) <= 0) printf("\nError registering new name! Try again!");
                            if (strlen(novoNome) > 0 && strcmp(novoNome, "-1") != 0) strcpy(produto.nome, novoNome);
                        } while (erronome != 1);
                        int erropreco = 0;
                        do {
                            printf("\nEnter the new product price (or -1 to keep unchanged): ");
                            float novoPreco;
                            erropreco = scanf("%f", &novoPreco);
                            if (novoPreco == -1) break;
                            if (novoPreco > 0) produto.preco = novoPreco;
                            if (erropreco != 1 || novoPreco < 0) printf("\nError registering new price! Enter the price again");
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
                    printf("\nProduct edited successfully!");
                } else {
                    printf("\nProduct not found.");
                }
                break;
            }
            case 'X': {
                FILE *produtos = fopen("produtos.txt", "r");
                if (!produtos) {
                    printf("\nError opening product file.");
                    break;
                }
                int idAlvo;
                printf("\nEnter the ID of the product to be deleted: ");
                scanf("%d", &idAlvo);
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
                if (!temp) {
                    printf("\nError opening temporary file.");
                    break;
                }
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (produto.id == idAlvo) {
                        printf("\nAre you sure you want to delete the product %s? (Y/N): ", produto.nome);
                        char confirmacao;
                        scanf(" %c", &confirmacao);
                        if (toupper(confirmacao) == 'Y') {
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
                    printf("\nProduct deleted successfully!");
                } else if (encontrado == 1) {
                    printf("\nProduct not deleted.");
                } else {
                    printf("\nProduct not found.");
                }
                break;
            }
            case 'S':
                saidaprod = 'S';
                break;
            default:
                printf("\nInvalid option, try again.");

        }
    } while (toupper(saidaprod) != 'S');
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>


struct Produto {
    int id;
    char nome[76];
    float preco;
};
void produtos() {
    char saidaprod = 'n';
    do {
        system("cls");
        system("clear");
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
                struct Produto produto;
                printf("\nDigite um ID para o produto (anote-o em algum lugar): ");
                scanf("%d", &produto.id);
                printf("\nDigite o nome do produto: ");
                scanf(" %75[^\n]", produto.nome);
                printf("\nDigite o preço do produto: ");
                scanf("%f", &produto.preco);
                fprintf(produtos, "%d|%s|%.2f\n", produto.id, produto.nome, produto.preco);
                fclose(produtos);
                printf("\nProduto cadastrado com sucesso!");
                break;
            }
            case 'L': {
                FILE *produtos = fopen("produtos.txt", "a+");
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
                FILE *produtos = fopen("produtos.txt", "a+");

                int idAlvo;
                printf("\nDigite o ID do produto a ser editado: ");
                scanf("%d", &idAlvo);
                
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
                while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
                    if (produto.id == idAlvo) {
                        printf("\nDigite o novo nome do produto (ou pressione Enter para não alterar): ");
                        char novoNome[76];
                        scanf( " %75[^\n]", novoNome);
                        if (strlen(novoNome) > 0) {
                            strcpy(produto.nome, novoNome);
                        }
                        printf("\nDigite o novo preço do produto (ou pressione Enter para não alterar): ");
                        float novoPreco;
                        scanf("%f", &novoPreco);
                        if (novoPreco > 0) {
                            produto.preco = novoPreco;
                        }
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
                int idAlvo;
                printf("\nInforme o ID do produto a ser excluído:");
                scanf("%d", &idAlvo);
                struct Produto produto;
                int encontrado = 0;
                FILE *temp = fopen("temp.txt", "w");
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
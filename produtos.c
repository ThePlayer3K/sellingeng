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
}
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
            case 'C':
                FILE *produtos = fopen("produtos.txt", "a+");
                // lero lero etc e tal
        }
    } while (toupper(saidaprod) != 'S');
}
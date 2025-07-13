#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "visualizacao.h"
#include "produtos.h"
#include "motorvendas.h"
#include "despesas.h"


int main(void){
    char saida = 'n';
    do {
        char opcao;
        printf("\nHello! Select an option below by typing a letter to continue");
        printf("\n(P) Register, update or remove product");
        printf("\n(I) Start sales day");
        printf("\n(V) View previous sales day");
        printf("\n(D) Manage periodic expenses");
        printf("\n(S) Exit");
        printf("\nYour option: ");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);
        switch(opcao) {
            case 'P':
                produtos();
                system("cls");
                break;
            case 'I':
                motorVendas();
                system("cls");
                break;
            case 'V':
                Visualizar();
                break;
            case 'D':
                despesas();
                break;
            case 'S':
                printf("\nAre you sure you want to exit? (Y/N) ");
                scanf(" %c", &saida);
                break;
            default:
                system("cls");
                printf("\nInvalid option, try again");
        }

    } while (saida != 'S'&& saida != 's');
    return 0;
}
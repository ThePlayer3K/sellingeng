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
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > nul");
    char saida = 'n';
    do {
        char opcao;
        printf("\nOlá! Selecione uma opção abaixo digitando uma letra para continuar");
        printf("\n(P) Cadastrar, atualizar ou remover produto");
        printf("\n(I) Iniciar dia de vendas");
        printf("\n(V) Visualizar dia de vendas anterior");
        printf("\n(D) Gerenciar despesas periódicas");
        printf("\n(S) Sair");
        printf("\nSua opção: ");
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
                despesas();
                break;
            case 'D':
                break;
            case 'S':
                printf("\nVocê tem certeza que quer sair? (S/N) ");
                scanf(" %c", &saida);
                break;
            default:
                system("cls");
                printf("\nOpção inválida, tente novamente");
        }

    } while (saida != 'S'&& saida != 's');
    return 0;
}
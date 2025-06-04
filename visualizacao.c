#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

void Visualizar() {
    char saida = 'A';
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > nul");
    do {
        system("cls");
        printf("\nBem-vindo ao visualizador de vendas! Escolha as funções disponíveis");
        printf("\n(U) Visualizar dia único");
        printf("\n(M) Visualizar múltiplos dias de vendas");
        printf("\n(S) Sair");
        printf("\nSua opção: ");
        scanf(" %c", &saida);
    } while (toupper(saida) != 'S');
}

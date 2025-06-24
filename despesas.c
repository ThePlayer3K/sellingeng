#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>
#include "despesas.h"

struct Despesa {
    char nomedespesa[76];
    float valor;
};

struct Relatorio {
    int id;
    char datainicial[9];
    char datafinal[9];
    float valortotal;
};

int idRelatorioAtual() {
    FILE *listarelatorios = fopen("listarelatorios.txt", "r");
    if (!listarelatorios) return 1;
    int contador = 0;
    int id;
    char datainicial[9], datafinal[9];
    float valortotal;
    while (fscanf(listarelatorios, "%d|%8[^|]|%8[^|]|%f\n", &id, datainicial, datafinal, &valortotal) != EOF) {
        contador++;
    }
    fclose(listarelatorios);
    contador = contador + 1;
    return contador;
}

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

void despesas() {
    char saida = 'N';
    do {
        system("cls");
        printf("\nSelecione uma opção para gerenciar suas despesas: ");
        printf("\n(C) Criar tabela de despesas");
        printf("\n(V) Visualizar tabelas de despesas disponíveis");
        printf("\n(A) Apagar relatório de despesas");
        printf("\n(S) Sair");
        printf("\nSua opção: ");
        scanf(" %c", &saida);
        saida = toupper(saida);
        switch(saida) {
            case 'C':
                struct Despesa despesas[30];
                int verificacao = 0, verificacaopipe = 1;
                char datainicial[9], datafinal[9];
                do {
                    printf("\nInsira a data inicial da despesa (DDMMYYYY): ");
                    verificacao = scanf(" %8[^\n]", datainicial);
                    verificacaopipe = verificarPipe(datainicial);
                    if (verificacao != 1 || strlen(datainicial) != 8 || verificacaopipe != 0) printf("\nErro ao escanear data!");
                } while (verificacao != 1);
                verificacao = 0;
                verificacaopipe = 1;
                do {
                    printf("\nInsira a data final da despesa (DDMMYYYY): ");
                    verificacao = scanf(" %8[^\n]", datafinal);
                    verificacaopipe = verificarPipe(datafinal);
                    if (verificacao != 1) printf("\nErro ao escanear data!");
                } while (verificacao != 1 || strlen(datafinal) != 8 || verificacaopipe != 0);
                char continuacao = 'S';
                int numerodespesas = 0;
                do {
                    int verificarnome = 0;
                    int verificarpipenonome = 1;
                    do {
                        char nomedalinha[76];
                        printf("\nInsira o nome da despesa (%d/30 max): ", numerodespesas + 1);
                        verificarnome = scanf(" %75[^\n]", nomedalinha);
                        verificarpipenonome = verificarPipe(nomedalinha);
                        if (verificarnome != 1 || verificarpipenonome != 0) {
                            printf("\nErro ao ler nome da despesa!");
                        } else {
                            strcpy(despesas[numerodespesas].nomedespesa, nomedalinha);
                        }
                    } while (verificarnome != 1);
                    int verificarvalor = 0;
                    do {
                        float valor;
                        printf("\nInsira o valor da despesa: ");
                        verificarvalor = scanf("%f", &valor);
                        if (verificarvalor != 1) {
                            printf("\nErro ao registrar valor");
                        } else {
                            despesas[numerodespesas].valor = valor;
                            numerodespesas++;
                        }
                    } while (verificarvalor != 1);
                    if (numerodespesas < 30) {
                        printf("\nDeseja adicionar nova despesa para este relatório? (S/N)");
                        scanf(" %c", &continuacao);
                        continuacao = toupper(continuacao);
                    } else {
                        printf("\nLimite máximo de despesas atingido.");
                        continuacao = 'N';
                    }
                } while (continuacao != 'N' && numerodespesas < 30);
                int idatual = idRelatorioAtual();
                char nomearquivo[16];
                sprintf(nomearquivo, "%d.txt", idatual);
                char caminhoarquivo[64];
                _mkdir("relatorios");
                sprintf(caminhoarquivo, "relatorios/%s", nomearquivo);
                FILE *arquivorelatorio = fopen(caminhoarquivo, "a+");
                if (arquivorelatorio == NULL) {
                    printf("\nErro ao criar arquivo de relatório!");
                    system("pause");
                    break;
                }
                float valortotal = 0.0;
                for (int i = 0; i < numerodespesas; i++) {
                    fprintf(arquivorelatorio, "%s|%.2f\n", despesas[i].nomedespesa, despesas[i].valor);
                    valortotal += despesas[i].valor;
                }
                fclose(arquivorelatorio);
                FILE *listarelatorios = fopen("listarelatorios.txt", "a+");
                fprintf(listarelatorios, "%d|%s|%s|%.2f\n", idatual, datainicial, datafinal, valortotal);
                printf("\nRelatório salvo com sucesso!\nID: %d\nData inicial: %s\nData final: %s\nValor do relatório: R$%.2f", idatual, datainicial, datafinal, valortotal);
                fclose(listarelatorios);
                system("pause");
                break;
            case 'V':
                system("cls");
                printf("\n\n\n\n");
                FILE *relatorios = fopen("listarelatorios.txt", "r");
                struct Relatorio relatorio;
                printf("ID         Data Inicial          Data Final           Valor Total\n");
                while (fscanf(relatorios, "%d|%s|%s|%.2f\n", &relatorio.id, relatorio.datainicial, relatorio.datafinal, &relatorio.valortotal) != EOF) {
                    printf("%d          %s          %s          R$%.2f\n", relatorio.id, relatorio.datainicial, relatorio.datafinal, relatorio.valortotal);
                }
                printf("\n\n\n\n");
                system("pause");
                fclose(relatorios);
                break;
            case 'A':
                int idvalidado = 0;
                int idpraapagar;
                do {
                    printf("\nInsira o ID do relatório a excluir: ");
                    idvalidado = scanf("%d", &idpraapagar);
                    if (idvalidado != 1) printf("\nErro ao processar ID!");
                } while (idvalidado != 1);
                FILE *listaid = fopen("listarelatorios.txt", "r+");
                struct Relatorio buscaid;
                int idencontrado = 0;
                while (fscanf(listaid, "%d|%s|%s|%.2f\n", &buscaid.id, buscaid.datainicial, buscaid.datafinal, &buscaid.valortotal) != EOF) {
                    if (buscaid.id == idpraapagar) {
                        idencontrado = 1;
                    }
                    if (idencontrado == 1) break;
                }
                if (idencontrado == 0) {
                    printf("\nID não encontrado!");
                } else {
                    printf("\nTem certeza de que quer apagar o relatório de ID %d? (S/N)", idpraapagar);
                    char apagar = 'N';
                    scanf(" %c", &apagar);
                    apagar = toupper(apagar);
                    if (apagar == 'S') {
                        char diretorio[21];
                        sprintf(diretorio, "relatorios/%d.txt", idpraapagar);
                        remove(diretorio);
                        fclose(listaid);
                        FILE *listaid = fopen("listarelatorios.txt", "r");
                        FILE *temp = fopen("temp.txt", "a+");
                        while (fscanf(listaid, "%d|%s|%s|%.2f\n", &buscaid.id, buscaid.datainicial, buscaid.datafinal, &buscaid.valortotal) != EOF) {
                            if (buscaid.id != idpraapagar) {
                                fprintf(temp, "%d|%s|%s|%.2f\n", buscaid.id, buscaid.datainicial, buscaid.datafinal, buscaid.valortotal);
                            }
                        }
                        remove("listarelatorios.txt");
                        rename("temp.txt", "listarelatorios.txt");
                        printf("\nRelatório de despesas de ID %d removido com sucesso!\n", idpraapagar);
                        fclose(temp);
                    }
                }
                fclose(listaid);
                system("pause");
                break;
            case 'S':
                system("pause");
                system("cls");
                return;
            default:
                printf("\nEntrada inválida! Tente novamente!\n");
        }
    } while (saida != 'S');
}
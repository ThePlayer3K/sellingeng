#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include "despesas.h"
#include "produtos.h"

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

void despesas() {
    char saida = 'N';
    do {
        system("cls");
        printf("\nSelect an option to manage your expenses: ");
        printf("\n(C) Create expense table");
        printf("\n(V) View available expense tables");
        printf("\n(A) Delete expense table");
        printf("\n(D) View detailed expenses");
        printf("\n(S) Exit");
        printf("\nYour option: ");
        scanf(" %c", &saida);
        saida = toupper(saida);
        switch(saida) {
            case 'C': {
                struct Despesa despesas[30];
                int verificacao = 0, verificacaopipe = 1;
                char datainicial[9], datafinal[9];
                do {
                    printf("\nEnter the initial date of the expense (DDMMYYYY): ");
                    verificacao = scanf(" %8[^\n]", datainicial);
                    verificacaopipe = verificarPipe(datainicial);
                    if (verificacao != 1 || strlen(datainicial) != 8 || verificacaopipe != 0) printf("\nError scanning date!");
                } while (verificacao != 1);
                verificacao = 0;
                verificacaopipe = 1;
                do {
                    printf("\nEnter the final date of the expense (DDMMYYYY): ");
                    verificacao = scanf(" %8[^\n]", datafinal);
                    verificacaopipe = verificarPipe(datafinal);
                    if (verificacao != 1) printf("\nError scanning date!");
                } while (verificacao != 1 || strlen(datafinal) != 8 || verificacaopipe != 0);
                char continuacao = 'S';
                int numerodespesas = 0;
                do {
                    int verificarnome = 0;
                    int verificarpipenonome = 1;
                    do {
                        char nomedalinha[76];
                        printf("\nEnter the name of the expense (%d/30 max): ", numerodespesas + 1);
                        verificarnome = scanf(" %75[^\n]", nomedalinha);
                        verificarpipenonome = verificarPipe(nomedalinha);
                        if (verificarnome != 1 || verificarpipenonome != 0) {
                            printf("\nError reading expense name!");
                        } else {
                            strcpy(despesas[numerodespesas].nomedespesa, nomedalinha);
                        }
                    } while (verificarnome != 1);
                    int verificarvalor = 0;
                    do {
                        float valor;
                        printf("\nEnter the value of the expense: ");
                        verificarvalor = scanf("%f", &valor);
                        if (verificarvalor != 1) {
                            printf("\nError registering value");
                        } else {
                            despesas[numerodespesas].valor = valor;
                            numerodespesas++;
                        }
                    } while (verificarvalor != 1);
                    if (numerodespesas < 30) {
                        printf("\nDo you want to add a new expense to this report? (Y/N): ");
                        scanf(" %c", &continuacao);
                        continuacao = toupper(continuacao);
                    } else {
                        printf("\nMaximum number of expenses reached.");
                        continuacao = 'N';
                    }
                } while (continuacao != 'N' && numerodespesas < 30);
                int idatual = idRelatorioAtual();
                char nomearquivo[16];
                sprintf(nomearquivo, "%d.txt", idatual);
                char caminhoarquivo[64];
                system("mkdir relatorios > nul");
                sprintf(caminhoarquivo, "relatorios/%s", nomearquivo);
                FILE *arquivorelatorio = fopen(caminhoarquivo, "a+");
                if (arquivorelatorio == NULL) {
                    printf("\nError creating report file!");
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
                printf("\nReport saved successfully!\nID: %d\nInitial date: %s\nFinal date: %s\nReport value: $%.2f", idatual, datainicial, datafinal, valortotal);
                printf("\nReport ID: %d", idatual);
                fclose(listarelatorios);
                system("pause");
                break;
            }
            case 'V': {
                system("cls");
                printf("\n\n\n\n");
                FILE *relatorios = fopen("listarelatorios.txt", "r");
                struct Relatorio relatorio;
                printf("ID         Initial Date          Final Date           Total Value\n");
                while (fscanf(relatorios, "%d|%8[^|]|%8[^|]|%f\n", &relatorio.id, relatorio.datainicial, relatorio.datafinal, &relatorio.valortotal) == 4) {
                    printf("%d          %s          %s          $%.2f\n", relatorio.id, relatorio.datainicial, relatorio.datafinal, relatorio.valortotal);
                }
                printf("\n\n\n\n");
                system("pause");
                fclose(relatorios);
                break;
            }
            case 'A': {
                int idvalidado = 0;
                int idpraapagar;
                do {
                    printf("\nEnter the ID of the report to delete: ");
                    idvalidado = scanf("%d", &idpraapagar);
                    if (idvalidado != 1) printf("\nError processing ID!");
                } while (idvalidado != 1);
                FILE *listaid = fopen("listarelatorios.txt", "r+");
                struct Relatorio buscaid;
                int idencontrado = 0;
                while (fscanf(listaid, "%d|%s|%s|%f\n", &buscaid.id, buscaid.datainicial, buscaid.datafinal, &buscaid.valortotal) != EOF) {
                    if (buscaid.id == idpraapagar) {
                        idencontrado = 1;
                    }
                    if (idencontrado == 1) break;
                }
                if (idencontrado == 0) {
                    printf("\nID not found!");
                } else {
                    printf("\nAre you sure you want to delete the report with ID %d? (Y/N): ", idpraapagar);
                    char apagar = 'N';
                    scanf(" %c", &apagar);
                    apagar = toupper(apagar);
                    if (apagar == 'Y') {
                        char diretorio[21];
                        sprintf(diretorio, "relatorios/%d.txt", idpraapagar);
                        remove(diretorio);
                        fclose(listaid);
                        FILE *listaid = fopen("listarelatorios.txt", "r");
                        FILE *temp = fopen("temp.txt", "a+");
                        while (fscanf(listaid, "%d|%s|%s|%f\n", &buscaid.id, buscaid.datainicial, buscaid.datafinal, &buscaid.valortotal) != EOF) {
                            if (buscaid.id != idpraapagar) {
                                fprintf(temp, "%d|%s|%s|%.2f\n", buscaid.id, buscaid.datainicial, buscaid.datafinal, buscaid.valortotal);
                            }
                        }
                        remove("listarelatorios.txt");
                        rename("temp.txt", "listarelatorios.txt");
                        printf("\nExpense report with ID %d successfully removed!\n", idpraapagar);
                        fclose(temp);
                    }
                }
                fclose(listaid);
                system("pause");
                break;
            }
            case 'S': {
                system("pause");
                system("cls");
                return;
            }
            case 'D': {
                int idvalidado = 0;
                int idprocurado;
                do {
                printf("\nEnter the ID of the report you want to view in detail: ");
                idvalidado = scanf("%d", &idprocurado);
                if (idvalidado != 1) printf("\nInvalid ID!");
            } while (idvalidado != 1);
            char nomerelatorio[21];
            sprintf(nomerelatorio, "relatorios/%d.txt", idprocurado);
            FILE *relatorioprocurado = fopen(nomerelatorio, "r");
            if (!relatorioprocurado) {
                printf("\nReport not found.");
                break;
            } else {
                printf("\nReport for id %d:\n\n", idprocurado);
                printf("Expense                     Value");
                struct Despesa despesa;
                while (fscanf(relatorioprocurado, "%75[^|]|%f\n", despesa.nomedespesa, &despesa.valor) != EOF) {
                    printf("\n%s                  $%.2f", despesa.nomedespesa, despesa.valor);
                }
                printf("\n");
                system("pause");
            }
            }
            default: {
                printf("\nInvalid input! Try again!\n");
            }
        }
    } while (saida != 'S');
}
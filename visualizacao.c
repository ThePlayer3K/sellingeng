#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#include "visualizacao.h"

struct Produto {
    int id;
    char nome[76];
    float preco;
};

struct Linharelatorio {
    int id;
    int quantidade;
    float preco;
    char metododavenda;
};

struct Produtolistado {
    int id;
    int quantidade;
};

int numprodutos = 0;

int carregarProdutos() {
    FILE *listaprodutos = fopen("produtos.txt", "r");
    if (!listaprodutos) {
        printf("\nError opening produtos.txt");
        return 1;
    }
    struct Produto produto;
    while (fscanf(listaprodutos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) numprodutos++;
    fclose(listaprodutos);
    return 0;
}

int lerDia() {
    int n = 0;
    do {
        scanf("%d", &n);
        if (n < 1 || n > 31) {
            printf("\nInvalid day: try again: ");
        } 
    } while (n < 1 || n > 31);
    return n;
}

int lerMes() {
    int n = 0;
    do {
        scanf("%d", &n);
        if (n < 1 || n > 12) {
            printf("\nInvalid month: try again: ");
        } 
    } while (n < 1 || n > 12);
    return n;
}

int lerAno() {
    int n = 10000;
    do {
        scanf("%d", &n);
        if (n > 9999) {
            printf("\nInvalid year: try again: ");
        } 
    } while (n > 9999);
    return n;
}

int procurarArquivo(char nomearquivo[]) {
    FILE *arquivo = fopen(nomearquivo, "r");
    if (arquivo) {
        return 0;
    }
    return 1;
}

char *procurarNomeProduto(int id) {
    static char nome[76];
    FILE *listaprodutos = fopen("produtos.txt", "a+");
    if (!listaprodutos) return "ERRO AO LER";
    struct Produto produto;
    while (fscanf(listaprodutos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
        if (produto.id == id) {
            strcpy(nome, produto.nome);
            fclose(listaprodutos);
            return nome;
        }
    }
    fclose(listaprodutos);
    return "ERRO AO LER";
}

void Visualizar() {
    char saida = 'A';
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > nul");
    int verificarcarregamento = carregarProdutos();
    do {
        system("cls");
        printf("\nWelcome to the sales viewer! Choose the available functions");
        printf("\n(U) View single day");
        printf("\n(M) View multiple sales days (values only)");
        printf("\n(S) Exit");
        printf("\nYour option: ");
        scanf(" %c", &saida);
        saida = toupper(saida);
        switch(saida) {
            case 'U': {
                if (verificarcarregamento != 0) {
                    printf("\nError loading products! It will not be possible to use this function now.\nRestart the program or create a product and try again.");
                    return;
                }
                char data[13];
                int erroscan = 0, arquivoencontrado = 1;
                do {
                    printf("\nEnter the date of the mentioned file (DDMMYYYY, all together): ");
                    erroscan = scanf(" %8[^\n]", data);
                    if (erroscan != 1) {
                        printf("\nError reading date! ");
                        system("pause");
                        continue;
                    }
                    strcat(data, ".txt");
                    int verificararquivo = procurarArquivo(data);
                    if (verificararquivo == 1)  {
                        printf("\nFile with this date not found");
                        continue;
                    } else {
                        arquivoencontrado = 0;
                    }
                } while (erroscan != 1 || arquivoencontrado != 0);
                FILE *relatorio = fopen(data, "a+");
                if (!relatorio) {
                    printf("\nError opening file.");
                    break;
                }
                struct Linharelatorio linha;
                float valorcredito = 0.0;
                float valordebito = 0.0;
                float valorpix = 0.0;
                float valordinheiro = 0.0;
                float valoralimentacao = 0.0;
                struct Produtolistado prodvendidos[numprodutos];
                int contadorprodutos = 0;
                while (fscanf(relatorio, "%d|%d|%f|%c\n", &linha.id, &linha.quantidade, &linha.preco, &linha.metododavenda) != EOF) {
                    switch(linha.metododavenda) {
                        case 'C': 
                            valorcredito += linha.preco;
                            break;
                        case 'D':
                            valordebito += linha.preco;
                            break;
                        case 'P':
                            valorpix += linha.preco;
                            break;
                        case '$':
                            valordinheiro += linha.preco;
                            break;
                        case 'A':
                            valoralimentacao += linha.preco;
                            break;
                        default:
                            printf("\nError reading payment method.");
                    }
                    int encontrado = 0;
                    for (int i = 0; i < contadorprodutos; i++) {
                        if (linha.id == prodvendidos[i].id) {
                            prodvendidos[i].quantidade += linha.quantidade;
                            encontrado = 1;
                            break;
                        }
                    }
                    if (encontrado == 0) {
                        prodvendidos[contadorprodutos].id = linha.id;
                        prodvendidos[contadorprodutos].quantidade = linha.quantidade;
                        contadorprodutos++;
                    }
                }
                fclose(relatorio);
                
                for (int i = 0; i < contadorprodutos; i++) {
                    char produtoatual[76]; 
                    strcpy(produtoatual, procurarNomeProduto(prodvendidos[i].id));
                    int quantidadeprod = prodvendidos[i].quantidade;
                    printf("\n%s x%d", produtoatual, quantidadeprod);
                }

                printf("\n\n\nCash value: $%.2f", valordinheiro);
                printf("\nFood voucher value: $%.2f", valoralimentacao);
                printf("\nGross credit value: $%.2f", valorcredito);
                printf("\nGross debit value: $%.2f", valordebito);
                printf("\nGross Pix value: $%.2f", valorpix);
                float creditoliquido = valorcredito * 0.97;
                float debitoliquido = valordebito * 0.98;
                float pixliquido = valorpix * 0.995;
                printf("\nNet credit value: $%.2f", creditoliquido);
                printf("\nNet debit value: $%.2f", debitoliquido);
                printf("\nNet Pix value: $%.2f", pixliquido);
                float totalbruto = valordinheiro + valoralimentacao + valorcredito + valordebito + valorpix;
                float totalliquido = valordinheiro + valoralimentacao + creditoliquido + debitoliquido + pixliquido;
                printf("\nTotal gross value: $%.2f", totalbruto);
                printf("\nTotal net value: $%.2f", totalliquido);
                system("pause");
                break;
            }
            case 'M': {
                int diaI, mesI, anoI;
                int diaF, mesF, anoF;
                printf("\nKeep in mind the initial and final dates (day, month, year) of the period you want to analyze.");
                printf("\nEnter the initial day: ");
                diaI = lerDia();
                printf("\nEnter the initial month: ");
                mesI = lerMes();
                printf("\nEnter the initial year: ");
                anoI = lerAno();
                printf("\nEnter the final day: ");
                diaF = lerDia();
                printf("\nEnter the final month: ");
                mesF = lerMes();
                printf("\nEnter the final year: ");
                anoF = lerAno();
                struct tm data_atual = {0};
                struct tm data_final = {0};
                data_atual.tm_mday = diaI;
                data_atual.tm_mon = mesI - 1;
                data_atual.tm_year = anoI - 1900;
                data_final.tm_mday = diaF;
                data_final.tm_mon = mesF - 1;
                data_final.tm_year = anoF - 1900;
                char stringdatafinal[13];
                strftime(stringdatafinal, sizeof(stringdatafinal), "%d%m%Y", &data_final);
                strcat(stringdatafinal, ".txt");
                float valordinheirototal = 0; 
                float valoralimentacaototal = 0; 
                float valorcreditototal = 0; 
                float valordebitototal = 0;
                float valorpixtotal = 0;
                struct Linharelatorio linha;

                int killswitch = 0;
                int arquivosfalhos = 0;
                while (killswitch != 1) {
                    char stringdataatual[13];
                    strftime(stringdataatual, sizeof(stringdataatual), "%d%m%Y", &data_atual);
                    strcat(stringdataatual, ".txt");

                    if (procurarArquivo(stringdataatual) == 0) {
                        FILE *arquivoatual = fopen(stringdataatual, "r");
                        if (!arquivoatual)  {
                            arquivosfalhos += 1;
                            continue;
                        }
                        
                        while (fscanf(arquivoatual, "%d|%d|%f|%c\n", &linha.id, &linha.quantidade, &linha.preco, &linha.metododavenda) != EOF) {
                            switch(linha.metododavenda) {
                                case 'C':
                                    valorcreditototal += linha.preco;
                                    break;
                                case 'D':
                                    valordebitototal += linha.preco;
                                    break;
                                case 'P':
                                    valorpixtotal += linha.preco;
                                    break;
                                case '$':
                                    valordinheirototal += linha.preco;
                                    break;
                                case 'A':
                                    valoralimentacaototal += linha.preco;
                                    break;
                                default:
                                    printf("\nErro ao ler método de pagamento.");
                            }                           
                        }
                        fclose(arquivoatual);
                    }
                    time_t segatual = mktime(&data_atual) + 86400;
                    data_atual = *localtime(&segatual);
                    if (strcmp(stringdataatual, stringdatafinal) == 0) killswitch = 1;
                }
                printf("\nTotal cash value: $%.2f", valordinheirototal);
                printf("\nTotal food voucher value: $%.2f", valoralimentacaototal);
                printf("\nGross credit value: $%.2f", valorcreditototal);
                printf("\nGross debit value: $%.2f", valordebitototal);
                printf("\nGross Pix value: $%.2f", valorpixtotal);
                float creditoliquidototal = valorcreditototal * 0.97;
                float debitoliquidototal = valordebitototal * 0.98;
                float pixliquidototal = valorpixtotal * 0.995;
                printf("\nNet credit value: $%.2f", creditoliquidototal);
                printf("\nNet Pix value: $%.2f", pixliquidototal);
                float valorbrutototal = valordinheirototal + valoralimentacaototal + valorcreditototal + valordebitototal + valorpixtotal;
                float valorliquidototal = valordinheirototal + valoralimentacaototal + creditoliquidototal + debitoliquidototal + pixliquidototal;
                printf("\nTotal gross value: $%.2f", valorbrutototal);
                printf("\nTotal net value: $%.2f", valorliquidototal);
                if (arquivosfalhos > 0) {
                    printf("\n\n%d days were not processed due to errors. Either the files do not exist, or some circumstance prevented reading.", arquivosfalhos);
                }
                system("pause");
                break;
            }
            case 'S': {
                system("pause");
                break;
            }
            default: {
                printf("\nInvalid option, try again.");
                system("pause");
            }
        }
    } while (toupper(saida) != 'S');
}

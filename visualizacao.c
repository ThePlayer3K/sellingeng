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

void carregarProdutos() {
    FILE *listaprodutos = fopen("produtos.txt", "a+");
    if (!listaprodutos) {
        printf("\nErro ao abrir produtos.txt");
        return;
    }
    struct Produto produto;
    while (fscanf(listaprodutos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) numprodutos++;
    fclose(listaprodutos);
}

int lerDia() {
    int n = 0;
    do {
        scanf("%d", &n);
        if (n < 1 || n > 31) {
            printf("\nDia inválido: tente novamente: ");
        } 
    } while (n < 1 || n > 31);
    return n;
}

int lerMes() {
    int n = 0;
    do {
        scanf("%d", &n);
        if (n < 1 || n > 12) {
            printf("\nMês inválido: tente novamente: ");
        } 
    } while (n < 1 || n > 12);
    return n;
}

int lerAno() {
    int n = 10000;
    do {
        scanf("%d", &n);
        if (n > 9999) {
            printf("\nAno inválido: tente novamente: ");
        } 
    } while (n > 9999);
    return n;
}

int procurarArquivo(char nomearquivo[]) {
    FILE *arquivo = fopen(nomearquivo, "a+");
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
    do {
        system("cls");
        printf("\nBem-vindo ao visualizador de vendas! Escolha as funções disponíveis");
        printf("\n(U) Visualizar dia único");
        printf("\n(M) Visualizar múltiplos dias de vendas (valores apenas)");
        printf("\n(S) Sair");
        printf("\nSua opção: ");
        scanf(" %c", &saida);
        saida = toupper(saida);
        switch(saida) {
            case 'U':
                char data[13];
                int erroscan = 0, arquivoencontrado = 1;
                do {
                    printf("\nInsira a data do arquivo mencionado (DDMMAAAA, tudo junto): ");
                    erroscan = scanf(" %9[^\n]", data);
                    if (erroscan != 1) {
                        printf("\nErro ao ler data! ");
                        system("pause");
                        continue;
                    }
                    strcat(data, ".txt");
                    int verificararquivo = procurarArquivo(data);
                    if (verificararquivo == 1)  {
                        printf("\nArquivo com esta data não encontrado");
                        continue;
                    } else {
                        arquivoencontrado = 0;
                    }
                } while (erroscan != 1 || arquivoencontrado != 0);
                FILE *relatorio = fopen(data, "a+");
                if (!relatorio) {
                    printf("\nErro ao abrir arquivo.");
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
                while (fscanf(relatorio, "%d|%d|%.2f|%c\n", &linha.id, &linha.quantidade, &linha.preco, &linha.metododavenda) != EOF) {
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
                            printf("\nErro ao ler método de pagamento.");
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

                printf("\n\n\nValor em dinheiro: R$%.2f", valordinheiro);
                printf("\nValor vale-alimentação: R$%.2f", valoralimentacao);
                printf("\nValor crédito bruto: R$%.2f", valorcredito);
                printf("\nValor débito bruto: R$%.2f", valordebito);
                printf("\nValor Pix bruto: R$%.2f", valorpix);
                float creditoliquido = valorcredito * 0.97;
                float debitoliquido = valordebito * 0.98;
                float pixliquido = valorpix * 0.995;
                printf("\nValor crédito líquido: R$%.2f", creditoliquido);
                printf("\nValor débito líquido: R$%.2f", debitoliquido);
                printf("\nValor Pix líquido: R$%.2f", pixliquido);
                float totalbruto = valordinheiro + valoralimentacao + valorcredito + valordebito + valorpix;
                float totalliquido = valordinheiro + valoralimentacao + creditoliquido + debitoliquido + pixliquido;
                printf("\nValor bruto total: R$%.2f", totalbruto);
                printf("\nValor líquido total: R$.%2f", totalliquido);
                system("pause");
                break;
            case 'M':
                int diaI, mesI, anoI;
                int diaF, mesF, anoF;
                printf("\nTenha em mente as datas (dia, mês, ano) inicial e final do período que quer compreender as entradas do mês.");
                printf("\nInsira o dia inicial: ");
                diaI = lerDia();
                printf("\nInsira o mês inicial: ");
                mesI = lerMes();
                printf("\nInsira o ano inicial: ");
                anoI = lerAno();
                printf("\nInsira o dia final: ");
                diaF = lerDia();
                printf("\nInsira o mês final: ");
                mesF = lerMes();
                printf("\nInsira o ano final: ");
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
                while (killswitch != 1) {
                    char stringdataatual[13];
                    strftime(stringdataatual, sizeof(stringdataatual), "%d%m%Y", &data_atual);
                    if (strcmp(stringdataatual, stringdatafinal) == 0) killswitch = 1;
                    strcat(stringdataatual, ".txt");

                    if (procurarArquivo(stringdataatual) == 0) {
                        FILE *arquivoatual = fopen(stringdataatual, "a+");
                        if (!arquivoatual) continue;
                        
                        while (fscanf(arquivoatual, "%d|%d|%.2f|%c\n", &linha.id, &linha.quantidade, &linha.preco, &linha.metododavenda) != EOF) {
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
                }
                printf("\nValor em dinheiro total: R$%.2f", valordinheirototal);
                printf("\nValor em vale-alimentação total: R$%.2f", valoralimentacaototal);
                printf("\nValor crédito bruto: R$%.2f", valorcreditototal);
                printf("\nValor débito bruto: R$%.2f", valordebitototal);
                printf("\nValor Pix bruto: R$%.2f", valorpixtotal);
                float creditoliquidototal = valorcreditototal * 0.97;
                float debitoliquidototal = valordebitototal * 0.98;
                float pixliquidototal = valorpixtotal * 0.995;
                printf("\nValor crédito líquido: R$%.2f", debitoliquidototal);
                printf("\nValor Pix líquido: R$%.2f", pixliquidototal);
                float valorbrutototal = valordinheirototal + valoralimentacaototal + valorcreditototal + valordebitototal + valorpixtotal;
                float valorliquidototal = valordinheirototal + valoralimentacaototal + creditoliquidototal + debitoliquidototal + pixliquidototal;
                printf("\nValor bruto total: R$%.2f", valorbrutototal);
                printf("\nValor líquido total: R$%.2f", valorliquidototal);
                system("pause");
                break;
            case 'S':
                system("pause");
                break;
            default:
                printf("\nOpção inválida, tente novamente.");
                system("pause");
        }
    } while (toupper(saida) != 'S');
}

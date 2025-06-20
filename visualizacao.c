#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

int numprodutos = 0;
while (fscanf(produtos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
    numprodutos++;
}

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

int lerDia() {
    int n = 0;
    do {
        scanf("%d");
        if (n < 1 || n > 31) {
            printf("\nDia inválido: tente novamente: ");
        } 
    } while (n < 1 || n > 31);
    return n;
}

int lerMes() {
    int n = 0;
    do {
        scanf("%d");
        if (n < 1 || n > 12) {
            printf("\nMês inválido: tente novamente: ");
        } 
    } while (n < 1 || n > 12);
    return n;
}

int lerMes() {
    int n = 10000;
    do {
        scanf("%d");
        if (n > 9999) {
            printf("\nAno inválido: tente novamente: ");
        } 
    } while (n > 9999);
    return n;
}

int procurarArquivo(char[] nomearquivo) {
    char caminhoprograma[MAX_PATH];
    char caminhocompletoarquivo[MAX_PATH];
    DWORD tamanhocaminho;
    tamanhocaminho = GetModuleFileName(NULL, caminhoprograma, MAX_PATH);
    if (tamanhocaminho == 0) {
        printf("\nErro ao obter caminho do executável do programa. Relate o erro.");
        return 2;
    }

    char *ultimabarra = strrchr(caminhoprograma, '\\');
    if (ultimabarra != NULL) {
        *(ultimabarra + 1) = '\0';
    } else {
        strcpy(caminhoprograma, MAX_PATH, ".\\");
    }

    if (strcpy(caminhocompletoarquivo, MAX_PATH, nomearquivo) != 0) {
        printf("\nErro ao copiar nome do arquivo. Relate o erro.");
        return 2;
    }
    if (strcat(caminhocompletoarquivo, MAX_PATH, nomearquivo) != 0) {
        printf("\nErro ao concatenar nome do arquivo. Relate o erro.");
        return 2;
    }

    if (GetFileAttributes(caminhocompletoarquivo) == INVALID_FILE_ATTRIBUTES) {
        return 1;
    } else {
        return 0;
    }
    
}

char *procurarNomeProduto(int id) {
    FILE *listaprodutos("produtos.txt", "a+");
    struct Produto produto;
    while (fscanf(listaprodutos, "%d|%75[^|]|%f\n", &produto.id, produto.nome, &produto.preco) != EOF) {
        if (produto.id = id) {
            return produto.nome;
        }
    }
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
                        continue;
                    }
                    strcat(data, ".txt");
                    int verificararquivo = procurarArquivo(data);
                    if (verificararquivo == 1) printf("\nArquivo com esta data não encontrado");
                } while (erroscan != 1 && arquivoencontrado != 0);
                FILE *relatorio = (data, "a+");
                struct Linharelatorio linha;
                float valorcredito = 0.0;
                float valordebito = 0.0;
                float valorpix = 0.0;
                float valordinheiro = 0.0;
                float valoralimentacao = 0.0;
                struct Produtolistado prodvendidos[numprodutos];
                int contadorprodutos = 0;
                while (fscanf(data, "%d|%d|%.2f|%c\n", &linha.id, &linha.quantidade, &linha.preco, &linha.metododavenda) != EOF) {
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
                    for (int i = 0; i <= contadorprodutos; i++) {
                        if (linha.id == prodvendidos[contadorprodutos]) {
                            prodvendidos[contadorprodutos].quantidade += linha.quantidade;
                        } else {
                            prodvendidos[contadorprodutos].id = linha.id;
                            prodvendidos[contadorprodutos].quantidade = linha.quantidade;
                            contadorprodutos++;
                        }
                    }
                }
                for (int i = 0; i <= contadorprodutos; i++) {
                    char produtoatual[76] = procurarNomeProduto(prodvendidos[contadorprodutos].id);
                    int quantidadedoprod = prodvendidos[contadorprodutos].quantidade;
                    printf("\n%s x%d", produtoatual, quantidadedoprod);
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
                printf("\nValor líquido total: R$.2f", totalliquido);
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
                diaII = lerDia();
                printf("\nInsira o mês final: ");
                mesII = lerMes();
                printf("\nInsira o ano final: ");
                anoII = lerAno();
                struct tm data_atual = {0};
                struct tm data_final = {0};
                data_atual.tm_mday = diaI;
                data_atual.tm_mon = --mesI;
                data_atual.tm_year = anoI - 1900;
                data_final.tm_mday = diaII;
                data_final.tm_mon = --mesII;
                data_final.tm_year = anoII - 1900;
                char stringdatafinal[13];
                strftime(stringdatafinal, sizeof(stringdatafinal), "%02d%m%Y", &data_final);
                strcat(stringdatafinal, ".txt");
                float valordinheirototal = 0; 
                float valoralimentacaototal = 0; 
                float valorcreditototal = 0; 
                float valordebitototal = 0;
                float valorpixtotal = 0;
                int killswitch = 0;
                while (killswitch != 1) {
                    char stringdataatual[13];
                    strftime(stringdataatual, sizeof(stringdataatual), "%02d%m%Y", data_atual);
                    if (strcmp(stringdataatual, stringdatafinal) == 0) killswitch++;
                    strcat(stringdataatual, ".txt");
                    if (procurarArquivo(stringdataatual) == 0) {
                        FILE *arquivoatual = fopen(stringdataatual, "a+");
                        struct Linharelatorio linha;
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
                    time_t segatual = mktime(&data_atual);
                    segatual += 86400;
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
                printf("\nValor crédito líquido: R$.2f", creditoliquidototal);
                printf("\nValor débito líquido: R$.2f", debitoliquidototal);
                printf("\nValor Pix líquido: R$.2f", pixliquidototal);
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

#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEG_TO_RAD(x) ((x) * M_PI / 180.0)

#define MAX_STACK 512


typedef struct {
    float valores[MAX_STACK];
    int topo;
} PilhaFloat;

void initFloatStack(PilhaFloat *pilha) { pilha->topo = -1; }
void pushFloat(PilhaFloat *pilha, float valor) { 
    if (pilha->topo < MAX_STACK - 1) pilha->valores[++(pilha->topo)] = valor; 
}
float popFloat(PilhaFloat *pilha) { 
    return (pilha->topo >= 0) ? pilha->valores[(pilha->topo)--] : 0.0f; 
}



typedef struct {
    char conteudo[512];
    int prioridade;
} NoExpressao;


typedef struct {
    NoExpressao itens[MAX_STACK];
    int topo;
} PilhaExpressao;

void initExprStack(PilhaExpressao *pilha) { pilha->topo = -1; }
void pushExpr(PilhaExpressao *pilha, NoExpressao no) { 
    if (pilha->topo < MAX_STACK - 1) pilha->itens[++(pilha->topo)] = no; 
}
NoExpressao popExpr(PilhaExpressao *pilha) {
    NoExpressao vazio = {"", 0};
    return (pilha->topo >= 0) ? pilha->itens[(pilha->topo)--] : vazio;
}




int ehNumero(char *token) {
    return isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]));
}

int obterPrioridadeOp(char operador) {
    if (operador == '+' || operador == '-') return 1;
    if (operador == '*' || operador == '/' || operador == '%') return 2;
    if (operador == '^') return 3;
    return 0;
}




float getValorPosFixa(char *exprPosfixa) {
    if (!exprPosfixa) return 0.0f;

    PilhaFloat pilhaValores;
    initFloatStack(&pilhaValores);

    char buffer[512];
    strncpy(buffer, exprPosfixa, 511);
    buffer[511] = '\0';

    char *token = strtok(buffer, " ");

    while (token != NULL) {

        if (ehNumero(token)) {
            pushFloat(&pilhaValores, strtof(token, NULL));
        } else {

            /* Operadores unarios (funcoes) */
            if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
                strcmp(token, "tg") == 0  || strcmp(token, "log") == 0 ||
                strcmp(token, "raiz") == 0) {

                float valor = popFloat(&pilhaValores);
                float resultado = 0.0;

                if (strcmp(token, "sen") == 0) resultado = sin(DEG_TO_RAD(valor));
                else if (strcmp(token, "cos") == 0) resultado = cos(DEG_TO_RAD(valor));
                else if (strcmp(token, "tg") == 0) resultado = tan(DEG_TO_RAD(valor));
                else if (strcmp(token, "log") == 0) resultado = log10(valor);
                else if (strcmp(token, "raiz") == 0) resultado = sqrt(valor);

                pushFloat(&pilhaValores, resultado);
            }
            /* Operadores binarios */
            else {
                float operando2 = popFloat(&pilhaValores);
                float operando1 = popFloat(&pilhaValores);
                float resultado = 0.0;

                switch (token[0]) {
                    case '+': resultado = operando1 + operando2; break;
                    case '-': resultado = operando1 - operando2; break;
                    case '*': resultado = operando1 * operando2; break;
                    case '/': resultado = operando1 / operando2; break;
                    case '%': resultado = fmod(operando1, operando2); break;
                    case '^': resultado = pow(operando1, operando2); break;
                }

                pushFloat(&pilhaValores, resultado);
            }
        }

        token = strtok(NULL, " ");
    }

    return popFloat(&pilhaValores);
}




char * getFormaInFixa(char *exprPosfixa) {
    if (!exprPosfixa) return NULL;

    PilhaExpressao pilhaExpr;
    initExprStack(&pilhaExpr);

    char buffer[512];
    strncpy(buffer, exprPosfixa, 511);
    buffer[511] = '\0';

    char *token = strtok(buffer, " ");

    while (token != NULL) {

        if (ehNumero(token)) {
            NoExpressao novo;
            strcpy(novo.conteudo, token);
            novo.prioridade = 10;
            pushExpr(&pilhaExpr, novo);
        } 
        else if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
                 strcmp(token, "tg") == 0  || strcmp(token, "log") == 0 ||
                 strcmp(token, "raiz") == 0) {

            NoExpressao arg = popExpr(&pilhaExpr);
            NoExpressao novo;

            sprintf(novo.conteudo, "%s(%s)", token, arg.conteudo);
            novo.prioridade = 10;
            pushExpr(&pilhaExpr, novo);
        }
        else {
            NoExpressao dir = popExpr(&pilhaExpr);
            NoExpressao esq = popExpr(&pilhaExpr);
            NoExpressao novo;

            int prioridadeOp = obterPrioridadeOp(token[0]);

            char textoEsq[256], textoDir[256];

            if (esq.prioridade < prioridadeOp)
                sprintf(textoEsq, "(%s)", esq.conteudo);
            else
                strcpy(textoEsq, esq.conteudo);

            if (dir.prioridade < prioridadeOp || 
                (dir.prioridade == prioridadeOp && 
                (token[0] == '-' || token[0] == '/' || token[0] == '^')))
                sprintf(textoDir, "(%s)", dir.conteudo);
            else
                strcpy(textoDir, dir.conteudo);

            sprintf(novo.conteudo, "%s%s%s", textoEsq, token, textoDir);
            novo.prioridade = prioridadeOp;

            pushExpr(&pilhaExpr, novo);
        }

        token = strtok(NULL, " ");
    }

    NoExpressao resultado = popExpr(&pilhaExpr);

    char *saida = (char*) malloc(512);
    if (saida) strcpy(saida, resultado.conteudo);

    return saida;
}

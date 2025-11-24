#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

void executarTeste(int idTeste, char *exprPosfixa, char *infixaEsperada, float valorEsperado) {
    printf("--- Teste %d ---\n", idTeste);
    printf("Entrada Pos-Fixa: %s\n", exprPosfixa);
    
    char *infixaGerada = getFormaInFixa(exprPosfixa);
    printf("Infixa Gerada...: %s\n", infixaGerada ? infixaGerada : "NULL");
    printf("Infixa Esperada.: %s\n", infixaEsperada);
    
    float valorObtido = getValorPosFixa(exprPosfixa);
    printf("Valor Calculado.: %.4f\n", valorObtido);
    printf("Valor Esperado..: %.4f\n", valorEsperado);
    
    printf("\n");
    
    if (infixaGerada)
        free(infixaGerada);
}

int main() {

    executarTeste(1, "3 4 + 5 *", "(3+4)*5", 35.0);
    executarTeste(2, "7 2 * 4 +", "7*2+4", 18.0);
    executarTeste(3, "8 5 2 4 + * +", "8+5*(2+4)", 38.0);
    executarTeste(4, "6 2 / 3 + 4 *", "(6/2+3)*4", 24.0);
    executarTeste(5, "9 5 2 8 * 4 + * +", "9+5*(2*8+4)", 109.0);
    executarTeste(6, "2 3 + log 5 /", "log(2+3)/5", 0.1397);
    executarTeste(7, "10 log 3 ^ 2 +", "log(10)^3+2", 3.0);
    executarTeste(8, "45 60 + 30 cos *", "(45+60)*cos(30)", 90.9327);
    executarTeste(9, "0.5 45 sen 2 ^ +", "0.5+sen(45)^2", 1.0);

    return 0;
}

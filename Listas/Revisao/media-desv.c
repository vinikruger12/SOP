#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
    int n;
    scanf("%d", &n);
    if(n == -1) return 0;
    int somatorio = n;
    int somatorioQuadrado = n*n;
    int k = 1;
    while(1){
        scanf("%d", &n);
        if(n == -1) break;
        somatorio += n;
        somatorioQuadrado += n*n;
        k++;
        
    }

    double m = (double) somatorio / k;
    double d = sqrt((double) somatorioQuadrado / k - m * m);

    printf("Média: %.2f \n", m);
    printf("Desvio padrão: %.2f \n", d);
}
#include <stdio.h>

int main(int argc, char *argv[]){
    
    printf("Programa: %s\n", argv[0]);
    printf("Parametros: \n");
    
    for(int i = argc - 1;i > 0;i--){
        printf("%s\n", argv[i]);
    }
}
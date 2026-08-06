#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int f(int n){
    int ans = 0;
    while(n != 0){
        if(n % 2 != 0) ans++;
        n /= 2;
    }
    return ans;
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    printf("O número %s tem %d bits em 1\n",argv[1], f(n));
}
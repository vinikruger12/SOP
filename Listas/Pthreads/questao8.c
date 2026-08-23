#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int n;

void f1(void *argp) {
    int a;
    a = 20;
    n += a;
    printf("f1: a=%d\n", a);
}

void f2(void *argp) {
    int a;
    a = 10;
    n -= a;
    printf("f2: a=%d\n", a);
}

int main(void){
    pthread_t t1, t2;
    int rc;
    n = 0;
    rc = pthread_create(&t1, NULL, (void *)f1, NULL);
    rc = pthread_create(&t2, NULL, (void *)f2, NULL);
    rc = pthread_join(t1, NULL);
    rc = pthread_join(t2, NULL);
    printf("n=%d\n", n);
    return 0;
}

/*
O programa irá printar: 
a)
f1: a=20
f2: a=10
n=10

b)
f2: a=10
f1: a=20
n=10

c)
f1: a=20
f2: a=10
n=-10

d)
f2: a=10
f1: a=20
n=-10

e)
f1: a=20
f2: a=10
n=20

f)
f2: a=10
f2: a=20
n=20


Confirmando:
f1: a=20
f2: a=10
n=10
*/
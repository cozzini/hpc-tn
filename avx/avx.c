/*
 * authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
 * date      : October 2018
 * copyright : GNU Public License
 */


#include <stdlib.h>
#include <stdio.h>
#define SIZE 4096 

void multiplyValues(float *a, float *b, float * c)
{
int i;
    for (i = 0; i < SIZE; i++) {
        a[i] = b[i] * c[i]/3.0;
    }
}


int main()
{
float a[SIZE];
float b[SIZE];
float c[SIZE];
int i;


for (i = 0; i < 800000; i++){
    multiplyValues(a,b,c);
}

printf("%f", a[SIZE/2]);

return 0;

}

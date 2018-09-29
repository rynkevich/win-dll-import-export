#include <stdio.h>
#include <string.h>

volatile char entityToGreet[10];

int main()
{
    strcpy_s((char *)entityToGreet, sizeof(entityToGreet), "world");

    while (true) {
        printf("Hello, %s\n", entityToGreet);
    }
}
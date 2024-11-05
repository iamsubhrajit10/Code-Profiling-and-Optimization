#include <stdio.h>

int main()
{
    char str[] = "Hello, World!";
    printf("str = %s\n", str);

    // modifying the string literal
    str[0] = 'J';
    printf("Modified str = %s\n", str);

    char *str2 = "Hello, World!";
    printf("str2 = %s\n", str2);

    // modifying the string literal
    str2[7] = 'P';
    printf("Modified str2 = %s\n", str2);

    return 0;
}
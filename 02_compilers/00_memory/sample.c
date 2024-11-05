#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables, initialized, will be allocated in the data segment
int a = 10;
int b = 20;

// Global variables, uninitialized, will be allocated in the bss segment
int c;
int d;

void foo(int p, int q)
{
    // initializing global variables
    c = p;
    d = q;
    printf("c = %d, d = %d\n", c, d);

    // defining local variables, will be allocated on the stack
    int e = 50;
    int f = 60;
    printf("e = %d, f = %d\n", e, f);

    char *str = "Hello, World!";
    printf("str = %s\n", str);
}

void bar()
{
    // accessing global variables
    printf("a = %d, b = %d\n", a, b);
}

void baz()
{
    // array of size 10 using malloc, so will be allocated on the heap
    int *arr = (int *)malloc(10 * sizeof(int));

    srand(time(NULL)); // seed the random number generator

    // Initialize the array randomly
    for (int i = 0; i < 10; i++)
    {
        arr[i] = rand() % 10;
    }
    printf("Elements of arr: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    // accessing global variables
    printf("a = %d, b = %d\n", a, b);
    foo(30, 40);
    bar();
    baz();
    return 0;
}
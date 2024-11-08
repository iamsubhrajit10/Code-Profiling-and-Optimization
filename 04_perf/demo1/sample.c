#include <stdio.h>

int sample_function(int p)
{
    int n = 0;
    for (int i = 0; i < p; i++)
    {
        n = n + 1;
    }
    return n;
}

int main()
{
    int x = sample_function(100000);
    printf("x = %d\n", x);

    return 0;
}
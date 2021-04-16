#include <stdio.h>

int main(void)
{
    char symbols[] = {'@', '#', '*', '+'};

    printf("Hello world!\n");
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            printf("%3d%c", i * j, symbols[(i + j) % 4]);
        }
        printf("\n");
    }

    return 0;
}
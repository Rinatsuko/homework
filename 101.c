#include <stdio.h>
int main()
{
    int result;
    int number;
    char operation;
    scanf("%d", &result);
    while (1)
    {
        scanf(" %c", &operation);
        if (operation == '=')
        {
            break;
        }
        else if (operation != '+' && operation != '-' && operation != '*' && operation != '/')
        {
            printf("错误的运算符:%c", operation);
            return 0;
        }
        scanf("%d", &number);
        switch (operation)
        {
        case '+':
            result = result + number;
            continue;
        case '-':
            result = result - number;
            continue;
        case '*':
            result = result * number;
            continue;
        case '/':
            if (number == 0)
            {
                printf("错误的运算符:%c", operation);
                return 0;
            }
            else
            {
                result = result / number;
                continue;
            }
        }
    }
    printf("%d", result);
    return 0;
}
#include <stdio.h>

// 函数原型声明(函数的具体实现需要你在文件末尾完成)
void sort_interest(int *interest, int n);

int main()
{
    int n;
    scanf("%d", &n);
    int interest[100];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &interest[i]);
    }
    sort_interest(interest, n);
    for (int i = 0; i < n; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }
        printf("%d", interest[i]);
    }
    printf("\n");
    return 0;
}

void sort_interest(int *p, int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        int maxindex = i;
        for (int j = i + 1; j < length; j++)
        {
            if (p[j] > p[maxindex])
            {
                maxindex = j;
            }
        }
        if (maxindex != i)
        {
            int temp = p[i];
            p[i] = p[maxindex];
            p[maxindex] = temp;
        }
    }
}
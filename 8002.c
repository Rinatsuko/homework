#include <stdio.h>

// 函数原型声明(函数的具体实现需要你在文件末尾完成)
void find_top_k_index(int *interest, int n, int k, int *top_k_index);

int main()
{
    int n;
    scanf("%d", &n);
    int k;
    scanf("%d", &k);
    int interest[100];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &interest[i]);
    }
    int top_k_index[100]; // 其长度固定为 100，但实际上只需将返回结果写入到其前 k 个元素即可
    find_top_k_index(interest, n, k, top_k_index);
    for (int i = 0; i < k; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }
        printf("%d", top_k_index[i]);
    }
    printf("\n");
    return 0;
}

// 你需要实现的函数
void find_top_k_index(int *interest, int n, int k, int *top_k_index)
{
    for (int j = 0; j < k; j++)
    {
        int max = interest[0];
        for (int i = 0; i < n; i++)
        {
            if (interest[i] > max)
            {
                max = interest[i];
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (interest[i] == max)
            {
                top_k_index[j] = i;
                interest[i] = -1;
                break;
            }
        }
    }
}
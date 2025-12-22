#include <stdio.h>
#include <string.h>
#include <math.h>
int main()
{
    char str1[100], str2[100];
    int a[100] = {0};
    int b[100] = {0};
    int sum[100] = {0};
    scanf("%s", str1);
    scanf("%s", str2);
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    for (int i = 0; i < len1; i++)
    {
        a[i] = str1[len1 - 1 - i] - '0'; // a[0]=str[19] 假设是20位数 在下标为0的地方存个位数
    }
    for (int i = 0; i < len2; i++)
    {
        b[i] = str2[len2 - 1 - i] - '0';
    }
    // 把字符串转化为数组数据存进去
    int len_max = (len1 > len2) ? len1 : len2;
    int k = 0;
    for (int i = 0; i < len_max; i++)
    {
        sum[i] += a[i] + b[i]; // 处理进位值
        if (sum[i] >= 10)
        {
            sum[i + 1] += sum[i] / 10;
            sum[i] %= 10;
        }
    }
    k = len_max;
    if (sum[k] > 0)
    {
        k++;
    }
    if (k == 0)
    {
        printf("0");
    }
    else
    {
        for (int i = k - 1; i >= 0; i--)
        {
            printf("%d", sum[i]);
        }
    }
    printf("\n");
    return 0;
}
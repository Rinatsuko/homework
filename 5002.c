#include <stdio.h>
#include <string.h>

#define MAX_WORDS 1000
#define MAX_LEN 100
#define BUFFER_SIZE 1024

// 判断是不是字母
int my_is_alpha(char c)
{
    // 检查是否在小写区间 或者 大写区间
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// 大写转小写
char my_to_lower(char c)
{

    // 如果是大写，加上 32 把它“推”到小写的位置
    if (c >= 'A' && c <= 'Z')
    {
        return c + 32;
    }
    return c;
}

int main()
{
    char line[BUFFER_SIZE];
    char dictionary[MAX_WORDS][MAX_LEN];
    int distinct_count = 0;

    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        return 0;
    }

    char *p = line;

    while (*p)
    {
        // 只要不是字母，就跳过
        while (*p && !my_is_alpha(*p))
        {
            p++;
        }

        if (*p == '\0')
            break;

        char *start = p;

        // 只要是字母，就继续走
        while (*p && my_is_alpha(*p))
        {
            p++;
        }

        int len = p - start;

        char temp[MAX_LEN];
        if (len >= MAX_LEN)
            len = MAX_LEN - 1;

        strncpy(temp, start, len);
        temp[len] = '\0';

        for (int i = 0; temp[i]; i++)
        {
            temp[i] = my_to_lower(temp[i]);
        }

        int found = 0;
        for (int i = 0; i < distinct_count; i++)
        {
            if (strcmp(dictionary[i], temp) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            strcpy(dictionary[distinct_count], temp);
            distinct_count++;
        }
    }

    printf("%d\n", distinct_count);

    return 0;
}
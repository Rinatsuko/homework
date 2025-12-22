#include <stdio.h>
#include <string.h>

#define MAX_WORDS 1000
#define MAX_LEN 100
#define BUFFER_SIZE 1024 // 一次读入的文本最长的长度
// 预处理 方便修改

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
    char line[BUFFER_SIZE];              // 接收用户输入的原始文本
    char dictionary[MAX_WORDS][MAX_LEN]; // 一共能存1000个单词，每个单词不超过100个字母
    int distinct_count = 0;

    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        return 0;
    } // 从键盘里存一行字放到line里

    char *p = line; // 数组名在表达式中退化成指向第一个元素的指针
    // 用这个指针p去扫描数组

    while (*p)
    {
        // 只要不是字母，就跳过
        while (*p && !my_is_alpha(*p))
        {
            p++;
        }

        if (*p == '\0')
            break; // 到末尾了 结束

        char *start = p; // 记录一下跳过字符后的单词开头

        // 只要是字母，就继续走
        while (*p && my_is_alpha(*p))
        {
            p++;
        }

        int len = p - start; // 单词长度

        char temp[MAX_LEN];
        if (len >= MAX_LEN)
            len = MAX_LEN - 1; // 太长截断

        strncpy(temp, start, len); // 拷贝len个字符到temp
        temp[len] = '\0';

        for (int i = 0; temp[i]; i++)
        {
            temp[i] = my_to_lower(temp[i]); // 转换大小写
        }

        int found = 0;                           // 标志，默认这个单词是新的 还没出现过
        for (int i = 0; i < distinct_count; i++) // 单词数量
        {
            if (strcmp(dictionary[i], temp) == 0)
            {
                found = 1;
                break;
            } // 如果出现过，标记 跳出循环
        }

        if (!found)
        {
            strcpy(dictionary[distinct_count], temp);
            distinct_count++;
        }
    } // 没出现过 拷贝 计数器加一

    printf("%d\n", distinct_count);
    return 0;
}
// strlen(s)里面装了多少字
// sizeof(s)内存大小
char s[100] = "hello";
strlen(s); // 结果是 5
sizeof(s); // 结果是 100

// 复制
strcpy(dest, src)
    // 把src的内容连同\0一起复制到dest 可能会撑爆内存
    strncpy()
    // 只拷贝前n个字符
    strcat(dest, strcasec)
    // 拼接到dest的后面
    strcmp(s1, s2)
    // 比较两个字符串的内容 一样 return 0
    //<0 s1在字典里排在s2前面 不能用if(s1==s2) 这是比较内存地址是不是同一个

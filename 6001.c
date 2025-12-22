#include <stdio.h>
#include <stdlib.h>

typedef struct student
{
    int *scores;
} student;

// 2. 辅助函数：计算单个学生的最终得分
// 参数传入 student 结构体(值传递即可) 和 评委数 m
double get_final_score(student s, int m)
{
    int max = s.scores[0];
    int min = s.scores[0];
    int sum = 0;

    for (int i = 0; i < m; i++)
    {
        int val = s.scores[i];
        sum += val;

        if (val > max)
            max = val;
        if (val < min)
            min = val;
    }

    // 核心公式：(总分 - 最高 - 最低) / (人数 - 2)
    // 注意：必须除以 (m - 2.0) 或者强转 double，否则会丢失小数
    return (double)(sum - max - min) / (m - 2);
}

int main()
{
    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
        return 1;
    student *students = (student *)malloc(n * sizeof(student));
    if (students == NULL)
        return 1;
    for (int i = 0; i < n; i++)
    {
        // 给当前学生的 scores 指针分配 m 个整数的空间
        students[i].scores = (int *)malloc(m * sizeof(int));

        // 读取 m 个评委的分数
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &students[i].scores[j]);
        }
    }

    double highest_score = -1.0;

    for (int i = 0; i < n; i++)
    {
        // 调用函数
        double current_avg = get_final_score(students[i], m);

        if (current_avg > highest_score)
        {
            highest_score = current_avg;
        }
    }

    printf("%.2f\n", highest_score);

    for (int i = 0; i < n; i++)
    {
        free(students[i].scores);
    }
    free(students);

    return 0;
}
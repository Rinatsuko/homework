#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct student
{
    int *scores;
} student;
double calculate_average(student *ptr);
int findmax(student *ptr);
int findmin(student *ptr);
int main()
{
    int n, m; // n个学生,每个学生有m个评委打分
    scanf("%d %d", &n, &m);
    student *student_ptr = (student *)malloc(n * sizeof(student));
    for (int i = 0; i < n; i++)
    { // 给每个学生的数组分配m个内存空间
        student_ptr[i].scores = (int *)malloc(m * sizeof(int));
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int k;
            scanf("%d", &k);
            student_ptr[i].scores[j] = k;
        }
    }
}
int findmax(student *ptr, int scoresnumber)
{
    int max = ptr->scores[0];
    for (int i = 0; i < scoresnumber; i++)
    {
        if (ptr->scores[i] > max)
        {
            max = ptr->scores[i];
        }
    }
    return max;
}
double calculate_average(student *ptr, int scoresnumber, int studentnumber, int arr[]) // arr来存储每个学生的分数
{
    for (int i = 0; i < studentnumber; i++)
    {
        int sum = 0;
        int max = findmax(ptr[i], scoresnumber);
        int min = findmin(ptr[i], scoresnumber);
        for (int j = 0; j < scoresnumber; j++)
        {
            sum += ptr[i].scores[j];
        }
        int sum1 = sum - max - min;
        double average = (.2 % lf)(sum1 / (m - 2));
        arr[i] = average;
    }
    // findmax logic
    // return max
}

int findmin(student *ptr, int scoresnumber)
{
    int min = ptr.scores[0];
    for (int i = 0; i < scoresnumber; i++)
    {
        if (ptr.scores[i] < max)
        {
            min = ptr.scores[i];
        }
    }
    return min;
}

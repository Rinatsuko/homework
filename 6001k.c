// 1. 结构体修正
typedef struct student
{
    int *scores; // 星号在前面
} student;

// 2. 辅助函数修正 (注意用了 -> )
int findmax(student *ptr, int scoresnumber)
{
    int max = ptr->scores[0]; // 使用箭头
    for (int i = 1; i < scoresnumber; i++)
    { // 从1开始比就行
        if (ptr->scores[i] > max)
        {
            max = ptr->scores[i];
        }
    }
    return max;
}

int findmin(student *ptr, int scoresnumber)
{
    int min = ptr->scores[0];
    for (int i = 1; i < scoresnumber; i++)
    {
        if (ptr->scores[i] < min)
        { // 这里之前写成了 max
            min = ptr->scores[i];
        }
    }
    return min;
}

// 3. 计算函数修正
void calculate_average(student *ptr, int scoresnumber, int studentnumber, double arr[])
{
    for (int i = 0; i < studentnumber; i++)
    {
        // 传入 &ptr[i] (取地址)
        int max = findmax(&ptr[i], scoresnumber);
        int min = findmin(&ptr[i], scoresnumber);

        int sum = 0; // 定义在循环外
        for (int j = 0; j < scoresnumber; j++)
        {
            sum += ptr[i].scores[j];
        }

        int sum1 = sum - max - min;

        // 4. 计算修正：除数不能为0 (防御性编程), 且修正类型转换语法
        if (scoresnumber > 2)
        {
            arr[i] = (double)sum1 / (scoresnumber - 2);
        }
        else
        {
            arr[i] = 0.0; // 避免除以0
        }
    }
}
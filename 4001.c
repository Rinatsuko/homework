#include <stdio.h>
#include <stdlib.h>

// 全局变量：0表示空，1表示有人
int seat[20][5] = {0};

// 函数声明
void allocation(int times, int amount[]);

int main()
{
    int times; // 购买指令次数
    if (scanf("%d", &times) != 1)
        return 0;

    // 使用C99变长数组
    int amount[times]; // 可以考虑使用堆内存
    for (int i = 0; i < times; i++)
    {
        // 修正输入语法：amount + i 或 &amount[i] 都可以
        scanf("%d", amount + i);
    }

    // 调用分配函数
    allocation(times, amount);

    return 0;
}

void allocation(int times, int amount[])
{
    // 遍历每一个订单
    for (int i = 0; i < times; i++)
    {
        int k = amount[i]; // 当前订单需要几张票
        int success = 0;   // 标记是否成功在同一排分配

        // --- 策略1：尝试在同一排连续分配 ---
        for (int row = 0; row < 20; row++)
        {
            // 1. 检查当前行剩余空位数
            int empty_cnt = 0;
            for (int j = 0; j < 5; j++)
            {
                if (seat[row][j] == 0)
                    empty_cnt++;
            }

            // 2. 如果够坐，开始分配
            if (empty_cnt >= k)
            {
                int booked = 0; // 记录本单已分配数量
                for (int j = 0; j < 5 && booked < k; j++)
                {
                    if (seat[row][j] == 0)
                    {
                        seat[row][j] = 1; // 占座

                        // 输出控制：不是第一个座位才打印空格
                        if (booked > 0)
                            printf(" ");

                        // 打印座位号 (注意: 第5个座位下标4对应F)
                        printf("%d%c", row + 1, j == 4 ? 'F' : 'A' + j);

                        booked++;
                    }
                }
                printf("\n"); // 本单结束换行
                success = 1;  // 标记成功
                break;        // 跳出行的循环，处理下一单
            }
        }

        // --- 策略2：如果策略1失败，进行分散分配 ---
        if (success == 0)
        {
            int booked = 0;
            // 遍历整个车厢寻找空位
            for (int row = 0; row < 20 && booked < k; row++)
            {
                for (int j = 0; j < 5 && booked < k; j++)
                {
                    if (seat[row][j] == 0)
                    {
                        seat[row][j] = 1; // 占座

                        if (booked > 0)
                            printf(" ");
                        printf("%d%c", row + 1, j == 4 ? 'F' : 'A' + j);

                        booked++;
                    }
                }
            }
            printf("\n"); // 本单结束换行
        }
    }
}

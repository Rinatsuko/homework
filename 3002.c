#include <stdio.h>
#include <stdbool.h>

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// 递归函数：计算当前玩家相对于对手的最大分差
// 返回值: (当前玩家得分 - 对手得分)
int maxDiff(int nums[], int left, int right)
{
    // 基本情况：如果只剩一个元素，拿走它
    if (left == right)
    {
        return nums[left];
    }

    // 选项 1：拿走左边的元素
    // 我的得分 = nums[left] - (对手在剩下部分能拿到的最大净胜分)
    int pickLeft = nums[left] - maxDiff(nums, left + 1, right);

    // 选项 2：拿走右边的元素
    // 我的得分 = nums[right] - (对手在剩下部分能拿到的最大净胜分)
    int pickRight = nums[right] - maxDiff(nums, left, right - 1);

    // 作为最优策略，选择收益最大的一项
    return max(pickLeft, pickRight);
}

int main()
{
    int n;
    // 读取数组长度
    if (scanf("%d", &n) != 1)
        return 0;

    // 使用C99标准的变长数组 (VLA)
    int nums[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &nums[i]);
    }

    // 计算玩家 1 的最终净胜分 (范围是 0 到 n-1)
    int result = maxDiff(nums, 0, n - 1);

    // 如果净胜分 >= 0，说明玩家 1 赢了或者平局
    if (result >= 0)
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }

    return 0;
}
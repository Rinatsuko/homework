#include <stdio.h>
#include <stdbool.h>
// 求最大值
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// 返回值: (当前玩家得分 - 对手得分)
int maxDiff(int nums[], int left, int right)
{
    if (left == right)
    {
        return nums[left];
    }
    int maxinleft = nums[left] - maxDiff(nums, left + 1, right);
    int maxinright = nums[right] - maxDiff(nums, left, right - 1);
    return (maxinleft > maxinright) ? maxinleft : maxinright;
}

int main()
{
    int n;

    if (scanf("%d", &n) != 1)
        return 0;

    int nums[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &nums[i]);
    }

    int result = maxDiff(nums, 0, n - 1);

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
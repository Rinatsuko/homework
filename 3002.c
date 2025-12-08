#include <stdio.h>
#include <stdlib.h>
int main()
{
    return 0;
}
// 辅助递归函数
// i 是左边界，j 是右边界
int net_score(int *nums, int i, int j)
{
    // 1. Base Case: 如果 i 和 j 撞上了 (只剩一个数)
    if (i == j)
    {
        return nums[i];
    }

    // 2. 选择 A: 拿左边
    // 这里的逻辑是：我拿的分 - (对手在剩下局面里比我多拿的分)
    int pick_left = nums[i] - net_score(nums, i + 1, j);

    // 3. 选择 B: 拿右边
    int pick_right = nums[j] - net_score(nums, i, j - 1);

    // 4. 返回两种选择里对我最有利的 (最大值)
    return (pick_left > pick_right) ? pick_left : pick_right;
}

// 主函数
bool PredictTheWinner(int *nums, int numsSize)
{
    // 如果 玩家1 的净胜分 >= 0，就赢了
    return net_score(nums, 0, numsSize - 1) >= 0;
}
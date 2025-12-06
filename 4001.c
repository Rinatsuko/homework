#include <stdio.h>
#include <stdlib.h>
int seat[20][5] = {0};
int main()
{
    // use 0 mark empty,1 marks full
    int times; // how many times u buy a ticket
    scanf("%d", &times);
    int amount[times];
    for (int i = 0; i < times; i++)
    {
        scanf("%d", (amount + i));
    }
}
void allocation(int times, int amount[])
{
    for (int i = 0; i < times; i++)
    {
        int assigned = 0;
        for (int row = 0; row < 20; row++)
        {
            int a = check(row, amount[i]); // check if a row could contain the request
            // a==1 means that it can contain.
            if (a == 1)
            {
            }
        }
    }
}
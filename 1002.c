#include <stdio.h>
int main()
{
    int year, month, day;
    int month_year[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total;
    int sum = 0;
    int number;
    scanf("%d-%d-%d", &year, &month, &day);
    if (year < 1990 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Invalid input");
        if (month == 2 && day > 29)
        {
            printf("Invalid input");
        }
        return 0;
    }
    if (year % 4 == 0)
    {
        month_year[1] = 29;
    }
    for (int i = 0; i < year - 1991; i++)
    {
        if ((year - 1990) % 4 == 0)
        {
            number++;
        }
    }
    for (int i = 0; i < month - 1; i++)
    {
        sum += month_year[i];
    }
    total = (year - 1991) * 365 + number + sum + (day - 1);
    if (total % 5 == 0 || total & 5 == 4)
    {
        printf("rest");
    }
    else
    {
        printf("working");
    }
    return 0;
}
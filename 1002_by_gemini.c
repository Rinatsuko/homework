#include <stdio.h>

int main()
{
    int year, month, day;
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total = 0;

    scanf("%d-%d-%d", &year, &month, &day);

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        month_days[1] = 29;
    }

    if (year < 1990 || month < 1 || month > 12 || day < 1 || day > month_days[month - 1])
    {
        printf("Invalid input");
        return 0;
    }

    for (int y = 1990; y < year; y++)
    {
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))
        {
            total += 366;
        }
        else
        {
            total += 365;
        }
    }

    for (int m = 0; m < month - 1; m++)
    {
        total += month_days[m];
    }

    total += (day - 1);

    if (total % 5 < 3)
    {
        printf("working");
    }
    else
    {
        printf("rest");
    }

    return 0;
}
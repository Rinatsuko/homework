#include <stdio.h>
#include <stdlib.h>
void move(int number, char beginning, char intermediaries, char destination);
int main()
{
    int n;
    scanf("%d", &n);
    char beginning = 'A';
    char intermediaries = 'B';
    char destination = 'C';
    move(n, beginning, intermediaries, destination);
    return 0;
}
void move(int number, char beginning, char intermediaries, char destination)
{
    if (number == 0)
    {
        return;
    }
    move(number - 1, beginning, destination, intermediaries);
    printf("Move disk from %c to %c\n", beginning, destination);
    move(number - 1, intermediaries, beginning, destination);
}
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
// from beginning A to move all disks into C,use B as intermediaries
{
    if (number == 0)
    {
        return;
    }
    move(number - 1, beginning, destination, intermediaries);
    // from beginning (A) to move n-1 disks into (B),use (C) as intermediaries,so we switch destination and intermediaries.
    printf("Move disk from %c to %c\n", beginning, destination);
    // mannually move the biggest one from the beginning to destination
    // the beginning maybe new,dont as constant A or C
    move(number - 1, intermediaries, beginning, destination);
    // from intermediaries (B) to destination (C),use A as intermediaries.
}
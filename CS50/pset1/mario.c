/* Mario.c 
 *
 * Made by: Anaum
 *
 * Builds a half pyramid after taking input from user
 *
 */

#include <stdio.h>
#include <cs50.h>
#include <string.h>


int main(void)
{
   
    int height=0;

    /* Asking for height from user till
     * he enters a number between 0 and 23 */
 
    do
    {
        printf("height? ");
        height = GetInt();
    }
    while(height < 0 || height > 23);
 
    int k = height;

    /* Printing the half pyramid of #s
     * using nested for loops */
 
    for (int row = 1; row <= height; row++)
    {
        for (int pr_sp = k - 1; pr_sp > 0; pr_sp--)
        {
            printf(" "); 
        }

        for (int pr_ha = 1; pr_ha <= row + 1; pr_ha++)
        {
            printf("#");
        }
        printf("\n");
        k--; 
    }
}



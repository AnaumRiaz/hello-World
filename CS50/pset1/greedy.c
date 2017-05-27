/* greedy.c
 *
 * Author: Anaum
 *
 * Calculates change using greedy algo
 *
 */

#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void)
{
   
    float inp_change;
    int calc_coin;
    int coins = 0;

    /* Asking for chage from user till he 
     * enters a non negative number*/
 
    do
    {
        printf("O hai! How much change is owed?\n");
        inp_change = GetFloat();
    }
    while (inp_change < 0);
  
    int change = round(inp_change * 100);
    
 
    /* Checks whether the change will use 25C or 10C coins etc   
     * by dividing with the appropriate number
     * saves the number of coins in coins
     */  
   
    if (change >= 25)
    { 
        calc_coin = change / 25;
        // printf("25 c will be %i\n",d);
        change = change - calc_coin * 25;
        coins += calc_coin;
    }      
         
    if (change >= 10)
    { 
        calc_coin = change / 25;
        change = change - calc_coin * 25;
        coins += calc_coin;
    }      
             
    if (change >= 10)
    { 
        calc_coin = change / 10;
        change = change - calc_coin * 10;
        coins += calc_coin;
    }      
             
    if (change >= 5)
    { 
        calc_coin = change / 5;
        change = change - calc_coin * 5;
        coins += calc_coin;
    }   
              
    if (change < 5)
    { 
        calc_coin = change / 1;
        change = change - calc_coin * 1;
        coins += calc_coin;
    }         
        
    printf("%i\n",coins);         
             
             
}



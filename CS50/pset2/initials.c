/*
 * initials.c
 *
 * Anaum Riaz 
 *
 * Prints initials of names entered
 *
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // get name of user
    string name = GetString();  
    bool check = true;
  
  
    /* Checks if name is a valid string before traversing the whole string. 
     * checks if the flag is true and prints alphabet in capital if it is
     */
     
    if (name != NULL)
    {
        for (int i = 0, n = strlen(name); i < n; i++)
        {  
            if (check == true )
            {
                if (islower (name[i]))
                {
                    name[i] = toupper(name[i]);
                }    
                printf("%c",name[i]);  
            }


            if (isspace(name[i]))
            {
                check = true;
            }
            else 
            {
                check = false;
            } 
        }
        
        printf("\n");
    }
    
    return 0;
}

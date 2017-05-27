/*
 * caesar.c
 *
 * Anaum Riaz 
 *
 * Implements Caesar Puzzle
 *
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    
      
     if (argc != 2)
     {
     printf("You did not enter valid number ");
     return 1;
     }
     
     
     
     int key= atoi(argv[1]);
     string message = GetString(); 
     char enc_mess='a';
     int ascii_alphabet;
     int inter;
     
     
    for (int i = 0, mess_len = strlen (message); i < mess_len; i++)
    {
        if (isalpha(message[i]))
        {
            ascii_alphabet=(int) message[i];
      
            
            if (islower(message[i]))
            { 
                inter=ascii_alphabet + (key % 26);
                if (inter > 122)
                {
                    inter = (inter % 122) + 96;
                }
   
            }
            else
            {
                inter = ascii_alphabet + (key % 26); 
                if (inter > 90) 
                {
                   inter = (inter % 90) + 64;
                }
                
            }
            
            enc_mess= (char) (inter);
        }
        
        else
        {
            enc_mess= message[i];
        }
        printf("%c",enc_mess);
        
    }
    
    printf("\n");
     return 0;
}

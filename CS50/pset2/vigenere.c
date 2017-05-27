/*
 * vigenere.c
 *
 * Anaum Riaz 
 *
 * Implements Vigenere Puzzle
 *
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{

    int len = 0;
    bool condition = true;
 
    /* Following few lines of code checks whether the command line inputs 
     * are correct or not.
     */
     
    if (argc != 2)
    {
        printf("You did not enter valid key\n");
        return 1;
    }
        
    if (argc == 2 )  
    {
        len = strlen (argv[1]);
        char key [len];
        strcpy( key , argv[1]);
         
        for(int i=0; i < len; i++)
        {
            if (!isalpha (key[i]))
            {
                condition = false; 
                break;
            }
        }
           
        if (condition == false)
        { 
            printf("You did not enter valid key\n");
            return 1;
        }    
    }    
       

    char key[len];
    strcpy( key , argv[1]);
    
    // Converting key into lowercase
    for(int i = 0; i < len; i++)
    {
        key[i] = tolower (key[i]);
    } 

       
    string message = GetString(); 
    char enc_mess='a';
    int asci_alpha;
    int inter;
    int inter2;
    int offset;
    int key_index = 0;
        
         
    /* Following loop traverses the whole message. It finds the key's index 
     * applicable converting it into the corresponding offset added to  
     * the message. Depending on the case of the message, it changes the  
     * and stores it in enc_mess. Lastly, if its not an alphabet, 
     * it prints the original message character
     */     
     
    for (int i = 0, mess_len = strlen (message); i < mess_len; i++)
    {
        if (isalpha(message[i]))
        {
            asci_alpha = (int) message[i];
            inter2 = (int) key[key_index];
            offset = inter2 - 97;   
             
            if (islower(message[i]))
            { 
                inter = asci_alpha + offset;  
                if (inter > 122)
                {
                    inter = (inter % 122) + 96;
                }
            }
            else
            {
                inter = asci_alpha + offset; 
                if (inter > 90) 
                {
                    inter = (inter % 90) + 64;
                }
            }
          
            // Finding the index of the key for next cycle
            key_index = (key_index + 1) % len;
            enc_mess = (char) (inter);
                
        }
        else
        {
            enc_mess = message[i];
        }
       
        printf("%c",enc_mess);
    }        
    
    printf("\n");
    return 0;
}

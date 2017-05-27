/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */


#include<string.h>
#include<strings.h>
#include <stdbool.h>
#include<stdlib.h>
#include <stdio.h>
#include "dictionary.h"
FILE* dic_inptr = NULL;
//defining node in the linked list
typedef struct point
{
    char wordic[LENGTH +1];
    struct point *np;
} point ;
point *head = NULL;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    point *a = head; 
   
    while (a->np  != NULL)
    {
          if (!(strcasecmp(a->wordic, word)))
          {
            return true;
          }
          else
          a = a->np;
    }
    
     if (!(strcasecmp(a->wordic, word)))
      {
            return true;
      }
    
    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // open input file 
    dic_inptr  = fopen(dictionary, "r");
    if (dic_inptr == NULL)
    {
        printf("Could not open dictionary\n");
        return false;
    }
   
   //main part: reading from file and inserting in linkedlist 
   char word_dic[LENGTH + 1];
   int index = 0;
    
   
     for (int c = fgetc(dic_inptr); c != EOF; c = fgetc(dic_inptr))
    {
       
        if (c != '\n')
        {
            word_dic[index] = c;
            index++;
        }
        else if (c == '\n')
        {
            word_dic[index] = '\0';
            index = 0;
            insert(word_dic);
        }
     
    }
    
    //display();
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    char ch;
    int n_words = 0;
    if (dic_inptr == NULL)
    {
        return false;
    }
    ch = fgetc(dic_inptr);
    
    while (ch != EOF)
    {
        if (ch == '\n')
        {
           n_words ++ ;
        }
        ch = fgetc(dic_inptr);
       
    }
    
    printf("number of words %i\n", n_words);
    fseek(dic_inptr, 0, SEEK_SET);
    return n_words;
    
    
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    
    point *temp = head;
    while(temp->np != NULL) 
    {
           temp = head->np;
           free(head);
           head = temp;
   }
    
    return true;
}

//display 

void display()
{
    point* a = head;
    
    if (head == NULL)
    {
        printf("no members in linked list\n");
    }
    
    else 
    {   
        printf("The memebers of Linked list are \n");
        while(a->np != NULL)
        {
            printf("%s\n",a->wordic);
            a = a->np;
        }
  
    printf("%s\n",a->wordic);
    }
    
}
//insert implementation

void insert(const char* wor)
{
    //checking size of linked list
    
    int sizeofll = 0;
    point *a = head;
   
    if (head != NULL)
    {
        sizeofll=1;
        while(a->np != NULL)
        {
            sizeofll++;
            a = a->np;
        }
        
    }
     
    if (sizeofll == 0)
    {
        head = (point *)malloc(sizeof(struct point));
        if (head != NULL)
        {
          strcpy(head->wordic , wor);
          head->np = NULL;
         
        }
        else printf("In insert func, it did not create a ll\n");
    }
    
     else if (sizeofll >= 1 )
    {
        
        point* t1 = (point*)(malloc(sizeof(struct point)));
        strcpy(t1->wordic, head->wordic);
        t1->np = head->np;
        strcpy(head->wordic , wor);
        head->np = t1;
   }

}

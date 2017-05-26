/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */


#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "dictionary.h"

// globalizing the dictionary file pointer
FILE* dic_inptr = NULL;

// making an array of linkedlists (hashtablea)
node *hashtable[HTSIZE];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int ls = strlen(word);
    char n_word[ls + 1];
    
    // convert word to lowercase and store it in word_copy
    for (int i = 0; i < ls; i++)
    {
        n_word[i] = tolower(word[i]);
    }
    
    n_word[ls] = '\0';
    int code = hashfn(n_word);
    node * a = hashtable[code];
    
    while (a != NULL)
    {
        if (!strcasecmp(n_word, a->wordic))
        {
            return true;
        }
        else 
        {
            a = a->next;
        }
    }
    return false;
  
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{    
    // open input file 
    dic_inptr = fopen(dictionary, "r");
    if (dic_inptr == NULL)
    {
        printf("Could not open dictionary\n");
        return false;
    }
   
    // main part: reading from file and inserting in hashtable 
    char word_dic[LENGTH + 1] = {'\0'};
    n_words_dic = 0;
   
    while (1)
    {
        fscanf(dic_inptr, "%s", word_dic);   
                     
        if (feof(dic_inptr))
        {
            break;
        }
        else 
        {
            insert(word_dic);
            n_words_dic += 1;
        }
    }
    
    
    fclose(dic_inptr);
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return n_words_dic;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < HTSIZE; i++)
    {
        delete_ll(hashtable[i]);
    }
   
    return true;
}

/* delete a linked list whose first element is pointed by 'head'
** used by the unload function*/

void delete_ll(node* head)
{
    node* temp = head;
    
    while (temp != NULL)
    { 
        temp = head->next;
        free(head);
        head = temp;
    }
 
}
    
// Fowler-Nol-Vo implementation taken from prgramingalgorithms.com
int hashfn(const char *str)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < strlen(str); str++, i++)
    {
        hash *= fnv_prime;
        hash ^= (*str);
    }

    return hash % HTSIZE;
      
}

// returns size of linked list in a particular bucket
int size_ll(node* a)
{
    int sizeofll = 0;
   
    if (a != NULL)
    {
        sizeofll = 1;
        while(a->next != NULL)
        {
            sizeofll++;
            a = a->next;
        }
        
        return sizeofll;
    }
    else 
        return 0;
}


// displays the hashtable and size of individual linked lists

void display()
{
    for(int i = 0; i < HTSIZE; i++)
    {
        int size = size_ll(hashtable[i]);
        printf("size of linked list at location %i is %i\n",i, size);
        node * t = hashtable[i];
        
        for (int j = 0; j < size ; j++)
        {
            printf("%s\n", t->wordic);
            t = t->next;
        }
         
    }
    
}

// inserts word in hashtable
void insert(const char* word)
{
    int code = hashfn(word); 
    node *element = malloc(sizeof(node));
    
    // if its the first element
    if (size_ll(hashtable[code]) == 0)
    {
        strcpy(element->wordic , word);
        element->next = NULL;
        hashtable[code] = element; 
    }
    else
    {
        strcpy(element->wordic, word);
        element->next = hashtable[code];
        hashtable[code] = element;
    }
     

}

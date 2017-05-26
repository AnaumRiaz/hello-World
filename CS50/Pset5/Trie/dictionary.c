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
#include <ctype.h>
#include <stdio.h>
#include "dictionary.h"

//globalizing the dictionary file pointer
FILE* dic_inptr = NULL;

//globalizing the number of words read from a dictionary
int totalwords = 0;

//globalizing start of the trie
node* start = NULL;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    node* mov = start;        //making traversing node
    
    int ls = strlen(word);
    int k;
    
    // convert word to lowercase and getting index
    for (int i = 0; i < ls; i++)
    {
        if (isalpha(word[i]))
        {
            k = tolower(word[i]) - 97;
        }
        else   //it has to be apostrophee
        {    
            k = tolower(word[i]) - 13;
        }   
        
        //if the node exists, just move on to that
        if (mov->children[k] != NULL)
        {
            mov = mov->children[k];
        }
        else
            return false;
    }
  
    //we have a whole word from dictionary
    //we put node's eow is true
    if (mov-> eow == true)
    {
        return true;
    }
    else 
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
   
    //make new nodes
    start = new_node();
    node* mov = start;
    
    
    for (int c = fgetc(dic_inptr); c != EOF; c = fgetc(dic_inptr))
    {
       
        if (c != '\n')
        {
            //either its alphabet or apostrophee
            if isalpha(c)
            {
                c = tolower(c);
                c = c - 97; //97 is ASCII of 'a'
            }
            else
            {
                c = c - 13; //putting apostrophee in the last child children[26]
            }
       
           
            //if a trie node exists
            if (mov -> children [c] != NULL)
            {
                mov = mov -> children [c];
            }
            
            //if a trie node doesnt exist, make a new node
            else if (mov -> children [c] == NULL)
            {
                node* new = new_node();
                mov->children[c] = new;
                mov = new;
            }
            
        }
        
        
        else if (c == '\n')
        {
            //we have a whole word from dictionary
            //we put node's eow is true
            mov-> eow = true;
            mov = start;
            totalwords++ ; //keeping track of words read from dictionary
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
    
    return totalwords;
    
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node* mov = start;
    bool unloaded = delete_rec(mov);
    if (unloaded == true)
    {
        return true;
    }
    else
        return false;
}


//newnode

node* new_node()
{
    node* nn = malloc(sizeof(node));
    nn->eow = false;                   //keep the eow(end of word) as false 
    for (int i = 0; i< 27; i++)
    {
        nn->children[i] = NULL;        //initialize the children to NULL
    }
    return nn;
}

//delete the trie recursively
bool delete_rec(node* mov)
{
    
    for (int i = 0; i < 27; i++)
    {
        if (mov -> children[i] != NULL)
        {
            delete_rec(mov -> children[i]);
        }
    }
    free(mov);
    mov = NULL;
    return true;
}

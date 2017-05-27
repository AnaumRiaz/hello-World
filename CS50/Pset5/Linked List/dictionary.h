/**
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdbool.h>

//size of hashtable
#define HTSIZE 6200


// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

//globalizing the number of words read from dictionary
int n_words_dic;

//our struct for linked list
typedef struct node
{
    char wordic[LENGTH + 1];
    struct node *next;
} node ;

//function declarations for operation
//takes a string and gives a hash code
int hashfn(const char *str);

//returns the size of a particular bucket
int size_ll(node*);

//inserts a value in the hashtable
void insert(const char*);

//displays the hashtable and size of individual linked lists
void display();

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
 
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

//inserts element in Linkedlist
void insert(const char*);

//displays the elements of a linked list 
void display();

//deletes a linked list
void delete_ll(node* );

#endif // DICTIONARY_H

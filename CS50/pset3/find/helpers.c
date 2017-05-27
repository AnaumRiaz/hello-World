/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <math.h>
#include "helpers.h"
bool BinSearch(int arra[], int,int, int);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{    
    if (n < 0)
    {
        return false;
    }
     
    // using recursive function
    return BinSearch(values, value, 0, n - 1);
           
}

bool BinSearch(int arra[], int value, int start, int end)
{
    while (start <= end)
    {
    // Binary search
        int middle = floor ((start + end) / 2);  
        if (value == arra[middle])
        {
            return true;
        }
    
        else if (value > arra[middle])
        {
            // start = middle + 1;
            return   BinSearch(arra,value, middle + 1,end);
        }
        else if (value < arra[middle])
        {
            // end = middle - 1;
            return BinSearch(arra, value, start, middle - 1);
        }
    }  
    return false;
   
}
 


/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{    
    bool swap;
    int sortpart=0;
    int temp;
    
    do
    {
        
        swap = false;
        for (int i = 0; i < (n - 1 - sortpart); i++)
        {
            if (values[i] > values[i + 1])
            {
                temp = values[i];
                values[i] = values[i + 1];
                values[i + 1] = temp;
                swap = true;
            }
        }
        sortpart++;
    }
    while (swap == true );
 
    return;
}

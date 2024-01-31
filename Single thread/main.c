/* ==============================================================================
 * Author:  Jonathan Weaver, jonw0224@gmail.com
 * E-mail Contact: jonw0224@gmail.com
 * Description:  Single threaded implementation of prime number generator
 * Version: 1.0
 * Date: 11/29/2023
 * Filename:  main.c
 *
 * Version History:
 *      1.00 - 11/29/2023 - Created files
 *
 * Copyright (C) 2023 Jonathan Weaver
 *
 * This file is part of Multiprocessing example.
 *
 * Multiprocessing example is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ==============================================================================
 */
 
#include <stdint.h>
#define HIGHEST 1000
long testPrime;
int primeList[HIGHEST];

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Calculates the offset of the number in the prime array
 * n: the number in the array
 * i: returns the byte offset
 * b: returns the bit offset
 *****************************************************************************/
void offsets(long n, long* i, int* b)
{
    *b = (n - 3)/2 & 31;
    *i = (n - 3)/2 / 32;
}

/******************************************************************************
 * Calculates the offset of the number in the prime array
 * n: the number in the array
 * i: returns the byte offset
 * b: returns the bit offset
 *****************************************************************************/
void setPrime(long n)
{
    long i = 0;
    int b = 0;
    offsets(n, &i, &b);
    primeList[i] = primeList[i] | (1 << b);
}

/******************************************************************************
 *****************************************************************************/
int getPrime(long n)
{
    long i = 0;
    int b = 0;
    offsets(n, &i, &b);
    if((primeList[i] & (1 << b)) == 0)
      return 1;
    return 0;
}

/******************************************************************************
 * The main program
 *****************************************************************************/
int main()
{   
    // First prime number to test is 3
    testPrime = 3;
    for(long i = testPrime*3; i < 84; i = i + testPrime*2)
    {
      setPrime(i);
    }

    for(long i = 3; i < 84; i = i+2)
    {
        if(getPrime(i) == 1) 
            printf("%ld\n", i);
    }
    return 0;
}

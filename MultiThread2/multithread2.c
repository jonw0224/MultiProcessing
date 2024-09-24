/* ==============================================================================
 * Author:  Jonathan Weaver, jonw0224@gmail.com
 * E-mail Contact: jonw0224@gmail.com
 * Description:  Single threaded implementation of prime number generator
 * Version: 1.01
 * Date: 3/28/2024
 * Filename:  singlethread.c
 *
 * Version History:
 *      1.00 - 11/29/2023 - Created files
 *      1.00 - 1/31/2024 - Added the single thread example
 *      1.01 - 3/28/2024 - Finished writing and testing single thread example
 *
 * Copyright (C) 2024 Jonathan Weaver
 *
 * This file is part of Multiprocessing example. The Multiprocessing
 * example is calculating prime numbers for a range using (1) a
 * single thread, (2) multiple threads, (3) parallel multiprocessing
 * in one thread, and (4) multiprogram using a client and server
 * architecture and communicating using TCP/IP sockets.
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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>   // for gettimeofday()
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define HIGHEST 1000000
#define HIGHINDEX (((HIGHEST - 3) / 2 / sizeof(unsigned long) / 8) + 1)
#define OUTPUTFILE "primelist.txt"

unsigned long primeList[HIGHINDEX];
unsigned long *testPrime;
int noOfThreads;

// lock for the shared variable
pthread_mutex_t nextbaselock = PTHREAD_MUTEX_INITIALIZER;

struct pthreadStruct {
    unsigned long startPrime;
    int index;
};

/******************************************************************************
 * Calculates the offset of the number in the prime array
 * n: the number in the array
 * i: returns the byte offset
 * b: returns the bit offset
 * returns: nothing
*****************************************************************************/
void offsets(unsigned long n, unsigned long* i, unsigned long* b)
{
    unsigned long c = (n - 3) / 2;
    *b = c & (sizeof(unsigned long)*8-1);
    *i = c / (sizeof(unsigned long)*8);
}

/******************************************************************************
 * Marks a number as prime
 * n: the number to mark as prime.
 * returns: nothing
 *****************************************************************************/
void setPrime(unsigned long n)
{
    unsigned long i = 0;
    unsigned long b = 0;
    offsets(n, &i, &b);
    primeList[i] = primeList[i] | (((long) 1) << b);
}

/******************************************************************************
 * Returns whether or not a number has been marked as prime.
 * n: the number to return whether or not it's a prime
 * returns: 1 if the number is marked as a prime, 0 if the number is not marked
 *  as a prime.
 *****************************************************************************/
int getPrime(unsigned long n)
{
    unsigned long i = 0;
    unsigned long b = 0;
    offsets(n, &i, &b);
    if((primeList[i] & (((long) 1) << b)) == 0)
    {
        return 1;
    }
    return 0;
}

/******************************************************************************
 * Mark multiples of prime as prime numbers starting with prime*prime up to
 * HIGHEST
 * prime: the prime number to mark multiples of as not prime
 * returns: nothing
 *****************************************************************************/
void markPrimeMultiples(unsigned long prime, int i)
{
    for(testPrime[i] = prime*prime; testPrime[i] < HIGHEST; testPrime[i] = testPrime[i] + prime*2)
    {
        setPrime(testPrime[i]);
    }
}

void* threadProcess(void* arg)
{
    int i = ((struct pthreadStruct *) arg)->index;
    unsigned long tprime = ((struct pthreadStruct *) arg)->startPrime;
    // Test prime numbers until we exceed the high limit
    while(tprime*tprime < HIGHEST)
    {
        // Mark all of the multiples of testPrime as not prime
        markPrimeMultiples(tprime, i);
        tprime = tprime + 2*((unsigned long) noOfThreads);
        // Find the next prime number to test
        while((tprime*tprime < HIGHEST) && getPrime(tprime) == 0)
        {
            tprime = tprime + 2*((unsigned long) noOfThreads);
            // Wait until primes less than seedprime squared have been tested by the existing threads before creating a new thread
            //for(int j = 0; j < noOfThreads; j++)
            //{
            //    while(testPrime[j] < tprime*tprime) {}
            //}
        }
    }
    pthread_exit(NULL);
}

/******************************************************************************
 * The main program
 *****************************************************************************/
int main()
{
    // Keep time
    struct timeval start, end, endB;

    // Save the start time
    gettimeofday(&start, NULL);

    long noOfProcessors = sysconf(_SC_NPROCESSORS_CONF);
    noOfThreads = (int) noOfProcessors;
    printf("The number of processors is %ld\n", noOfProcessors);
    testPrime = malloc ((unsigned long) noOfThreads * sizeof(unsigned long));

    pthread_t ptid[noOfThreads];
    struct pthreadStruct threadArg[noOfThreads];
    
    // First Prime Number to Test is 3
    unsigned long seedPrime = 3;
    // Create new threads up to the noOfThreads
    for(int i = 0; i < noOfThreads; i++)
    {
        // Set up the argument to the new thread. Include thread to test, index to use in testPrime, and step to use in testing
        threadArg[i].startPrime = seedPrime;
        threadArg[i].index = i;
        // Create new thread
        pthread_create(&ptid[i], NULL, &threadProcess, &threadArg[i]);
        // Find next prime number to test
        seedPrime = seedPrime + 2;
//        while((seedPrime*seedPrime < HIGHEST) && getPrime(seedPrime) == 0)
//        {
//            seedPrime = seedPrime + 2;
//        }
    }
    
    // Merge all of the threads
    for(int i = 0; i < noOfThreads; i++)
    {
        pthread_join(ptid[i], NULL);
    }

    // Save the end time
    gettimeofday(&end, NULL);

    // Calculate and print the elapsed time
    float seconds = ((float) (end.tv_sec - start.tv_sec)) +
        ((float) (end.tv_usec - start.tv_usec))/1000000;
    printf("The elapsed time is %f seconds\n", seconds);

    FILE* outf = fopen(OUTPUTFILE,"w");
    for(unsigned long i = 3; i < HIGHEST; i = i+2)
    {
        if(getPrime(i) == 1)
            fprintf(outf, "%lu\n", i);
    }
    fclose(outf);

    // Save the end time
    gettimeofday(&endB, NULL);

    // Calculate and print the elapsed time
    seconds = ((float) (endB.tv_sec - end.tv_sec)) +
        ((float) (endB.tv_usec - end.tv_usec))/1000000;
    printf("Time to write file is %f seconds\n", seconds);

    return 0;
}

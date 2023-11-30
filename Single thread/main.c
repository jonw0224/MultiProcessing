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
 * Multiprocessing example is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int cnt;
void* threadFunct(void* arg)
{
    int i, a;
    printf("Tread: %lu\n",pthread_self());
    cnt = cnt + 1;
    a = 0;
    for(i = 0; i< 100000000; i++)
    {
        a++;
    }
    pthread_exit(NULL);

}

int main()
{
    int i;
    pthread_t ptid[2000];

    printf("Before threads!\n");
    cnt = 0;
    for(i = 0; i< 2000; i++)
    {
        //threadFunct(NULL);
        pthread_create(&ptid[i], NULL, &threadFunct, NULL);
    }
    for(i = 0; i< 2000; i++)
    {
        pthread_join(ptid[i], NULL);
    }
    printf("Cnt: %d\n", cnt);
    printf("Goodbye\n");
    return 0;
}

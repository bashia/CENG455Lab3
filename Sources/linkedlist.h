/*
 * linkedlist.h
 *
 *  Created on: Feb 20, 2016
 *      Author: cccarr
 */

#ifndef SOURCES_LINKEDLIST_H_
#define LINKEDLIST_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "mutex_print.h"

struct node
{
    char *character;
    struct node *next;
    struct node *prev;
} *top,*bottom;

void push(char *new_char);

char* pop();

void display();

char* llconvert();

void erase();

int llcount = 0;

extern popstop = 1;

#endif /* SOURCES_SERVER_H_ */

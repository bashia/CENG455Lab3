#include "linkedlist.h"

//void* _mem_alloc(size_t size)
//{
//        return malloc(size);
//}
//void* _mem_copy(void* src_ptr, void* dest_ptr, size_t num_units)
//{
//        return memcpy(dest_ptr,src_ptr,num_units);
//}
//
//int _mem_free(void* ptr)
//{
//        free(ptr);
//}

//struct node
//{
//    char *character;
//    struct node *next;
//    struct node *prev;
//} *top,*bottom;



void push(char *new_char)
{
    /* allocate a new node */
    struct node *node = _mem_alloc(sizeof(struct node));

    /* assign the person to the node */
    node->character = new_char;
    node->next = NULL;

    if (top == NULL)
    {
    	node->prev = NULL;
        top = node;
        bottom = node;
    }
    else
    {
        top->next = node;
        node->prev = top;
        top = node;
    }
    popstop = 0;
    llcount++;
}

char* pop()
{
        if (top == NULL)
        {
                return NULL;
        }
        else
        {
                char* toreturn = top->character;
                struct node *formertop = top;
                if (top == bottom)
				{
                	top = NULL;
                	bottom = NULL;
                	popstop = 1;
				}
                else
                {
                	top = top->prev;
                	top->next = NULL;

                }
                _mem_free(formertop);
                llcount--;
                return toreturn;
        }
}

void display()
{
    if( bottom == NULL)
    {
        mprintf("empty list\n");
    }
    else
    {
        struct node *current = bottom;
        while(current != NULL)
        {
            mprintf("%c",*current->character);
            current = current->next;
            //if(current != NULL)
                //printf(",");
        }
        printf("\n");
    }
}

//Turn linked list into string
char* llconvert()
{
	if (bottom == NULL)
	{
		return NULL;
	}
	struct node * pointer = bottom;
	char *converted_array = _mem_alloc(llcount*sizeof(char)+1);
	for(int i = 0; i<llcount;i++)
	{
	 converted_array[i] = *pointer->character;
	 pointer = pointer->next;
	}
	converted_array[llcount] = '\0';
	return converted_array;
}

void erase()
{
	struct node *current = bottom;
	while(current != NULL)
	{
		pop();
		current = top;
	}

}

//int main()
//{
//
//    for(int i=0;i<20;i++)
//    {
//        char newly[20] = "asdfghjklzxcvbnmqwer";
//
//        push(&newly[i]);
//    }
//    display();
//}

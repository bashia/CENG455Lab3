/*
 * mutex_print.c
 *
 *  Created on: Feb 21, 2016
 *      Author: cccarr
 */
#include "mutex_print.h"



int init_mutex()
{
	MUTEX_ATTR_STRUCT mutexattr;

	if (_mutatr_init(&mutexattr) != MQX_OK)
	{
		printf("Initializing mutex attributes failed.\n");
		return FAILURE;
	}
	if (_mutex_init(&print_mutex, &mutexattr) != MQX_OK)
	{
		return FAILURE;
	}
	return SUCCESS;
}

void mprintf(const char* format, ...)
{
	if (_mutex_lock(&print_mutex) != MQX_OK)
	{
		return;//Mutex Lock Failed
	}
	va_list args;
	va_start( args, format );
	vprintf(format,args);
	va_end(args);

	_mutex_unlock(&print_mutex);
}

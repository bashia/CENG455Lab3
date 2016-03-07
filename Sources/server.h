/*
 * server.h
 *
 *  Created on: Feb 10, 2016
 *      Author: cccarr
 */

#ifndef SOURCES_SERVER_H_
#define SOURCES_SERVER_H_
/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains definitions for this application
*
*
*END************************************************************************/

//#include <mqx.h>
//#include <message.h>

///* Number of clients */
//#define NUM_USERS           (7)
//#define FP_SYSTEM_QUEUE_BASE   2
//
///* Task IDs */
//#define SERVER_TASK       5
//#define USER_TASK       6
//
///* Queue IDs */
//#define SERVER_QUEUE_R      9
//#define SERVER_QUEUE_W      8
//#define ISR_QUEUE_BASE 10
//#define USER_QUEUE_BASE_READ 11
//#define USER_QUEUE_BASE_WRITE 12
//
///* This structure contains a data field and a message header structure */
//typedef struct server_message
//{
//   MESSAGE_HEADER_STRUCT   HEADER;
//   unsigned char                   DATA[5];
//} SERVER_MESSAGE, * SERVER_MESSAGE_PTR;
//
//
////extern _queue_id message_qid;
//extern _pool_id message_pool;
//extern int interrupt_occur = 0;
//extern _queue_id server_qid_r = 0;
//extern _queue_id server_qid_w = 0;
//extern int taskwriteline = 0;


/* Function prototypes */
//extern void server_task (uint32_t initial_data);
//extern void user_task (int initial_data);





#endif /* SOURCES_SERVER_H_ */

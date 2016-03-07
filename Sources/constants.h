/*
 * constants.h
 *
 *  Created on: Feb 21, 2016
 *      Author: cccarr
 */

#include <mqx.h>
#include <message.h>

#ifndef SOURCES_CONSTANTS_H_
#define SOURCES_CONSTANTS_H_

#define PERMLEN 16

#define SUCCESS 1
#define FAILURE 0

/* Number of clients */
#define NUM_USERS           (7)
#define FP_SYSTEM_QUEUE_BASE   2

/* Task IDs */
#define SERVER_TASK       5
#define USER_TASK       6

/* Queue IDs */
#define SERVER_QUEUE_R      8
#define SERVER_QUEUE_W      9
#define ISR_QUEUE_BASE 10
#define USER_QUEUE_BASE_R 12
#define USER_QUEUE_BASE_W 11

/* This structure contains a data field and a message header structure */
typedef struct server_message
{
   MESSAGE_HEADER_STRUCT   HEADER;
   unsigned char                   DATA[5];
} SERVER_MESSAGE, * SERVER_MESSAGE_PTR;

typedef struct pointer_message
{
	MESSAGE_HEADER_STRUCT   HEADER;
	   unsigned char*              DATA;
}POINTER_MESSAGE, * POINTER_MESSAGE_PTR;

//Global variables
//Message pools
extern _pool_id pointer_pool;
extern _pool_id message_pool;
//interrupt flag
extern int interrupt_occur = 0;
//Handler qids
extern _queue_id server_qid_r = 0;
extern _queue_id server_qid_w = 0;
//write line handler flag
extern int taskwriteline = 0;
//write line qid
extern _queue_id userwriteqid = 0;
//user task id lists
extern _queue_id userqids[PERMLEN] = {0};
extern _task_id taskids[PERMLEN] = {0};
//permissions
extern int readpermission[PERMLEN] = {0};
extern int readindex = 0;
extern int writepermission = 0;
//extern _queue_id write_qid = 0;
//extern int command = 0;


#endif /* SOURCES_CONSTANTS_H_ */


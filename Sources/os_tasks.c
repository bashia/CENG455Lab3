/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : serial_echoLab2
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-01, 16:06, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         serial_task - void serial_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <mqx.h>
#include <message.h>

#ifdef __cplusplus
extern "C" {
#endif 




/* User includes (#include below this line is not maintained by Processor Expert) */
#include "constants.h"
#include "accessfunctions.h"
#include "linkedlist.h"
#include "mutex_print.h"


/*
** ===================================================================
**     Callback    : serial_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

/*TODO
 *1.) Finish user task functions (putline getline) DONE!!?
 *2.) Make them work with more than one usertask
 *3.)Fix messaging issues causing first character problem. SOMEHOW FIXED!?
 *4.)Fix text messaging becoming delayed by 1 character.
 *5.) Make non-static way to test user tasks.
 *6.) Free memory for sendstring in getline.
 *7.) TEST IT
 *
 */
struct task_list {
uint32_t tid;
uint32_t deadline;
uint32_t task_type;
uint32_t creation_time;
struct task_list *next_cell;
struct task_list *previous_cell;
} TASKLIST;

struct overdue_tasks {
uint32_t tid;
uint32_t deadline;
uint32_t task_type;
uint32_t creation_time;
struct overdue_tasks *next_cell;
struct overdue_tasks *previous_cell;
} ODLIST;

_task_id dd_tcreate(uint32_t template_index, uint32_t deadline)
{
//	1. Opens a queue
	POINTER_MESSAGE_PTR msg_ptrSend;
	CreateTask_q = _msgq_open(SERVER_QUEUE_R, 0);

//	2. Creates the task specified and assigns it the minimum priority possible
	_task_id task_id;
	task_id = _task_create(0,USERTASK_TASK, deadline); //Not sure how to assign priority

//	3. Composes a create_task_message and sends it to the DD-scheduler

	msg_ptrSend = (POINTER_MESSAGE_PTR) _msg_alloc(pointer_pool);
	msg_ptrSend->HEADER.SOURCE_QID = CreateTask_q;
	msg_ptrSend->DATA = "Create Task: %d", tid;
	_msgq_send(msg_ptrSend);

//	4. Waits for a reply at the queue it created above
	msg_ptrSend = _msgq_receive(CreateTask_q, 0);

//	5. Once the reply is received, it obtains it

//	6. Destroys the queue

//	7. Returns to the invoking task
	return Task_id;
}
uint32_t dd_delete(uint32_t task_id)
{

}
uint32_t dd_return_active_list(struct task_list **list)
{

}
uint32_t dd_return_overdue_list(struct task_list **list)
{

}


void serial_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */

	init_mutex();

	int numencoding;

	SERVER_MESSAGE_PTR msg_ptrRead;
	SERVER_MESSAGE_PTR msg_ptrWrite;
	POINTER_MESSAGE_PTR msg_ptrSend;
	 _task_id           task_id;
	 int validcount = 0;
	 //_queue_id server_qid;

	 /* Open a message queue: */
	 server_qid_r = _msgq_open(SERVER_QUEUE_R, 0);
	 server_qid_w = _msgq_open(SERVER_QUEUE_W, 0);

	 /* Create a message pool: */
	 message_pool = _msgpool_create(sizeof(SERVER_MESSAGE),
	 NUM_USERS, 5, 0);
	 _pool_id pointer_pool = _msgpool_create(sizeof(POINTER_MESSAGE),
	 	 NUM_USERS, 5, 0);

	 if (message_pool == MSGPOOL_NULL_POOL_ID) {
	      mprintf("\nCount not create a message pool\n");
	      //_task_block();
	   }

	 	/*Create Tasks */
	 //DUE TO message queue limit we can only have up to 4 tasks at a time
	 	//First value of commands is index of user task.
		int commands[10] = {0,1,2,3,4,3,4,3,4,5};
		task_id = _task_create(0,USERTASK_TASK, commands);
		if (task_id == 0)
		{
			mprintf("\nCould not create a User task\n");
		}
		int commands2[10] = {1,1,2,3,4,3,4,3,4,5};
		task_id = _task_create(0,USERTASK_TASK, commands2);
		if (task_id == 0)
		{
			mprintf("\nCould not create a User task\n");
		}
		int commands3[10] = {2,1,2,3,4,3,4,3,4,5};
		task_id = _task_create(0,USERTASK_TASK, commands3);
		if (task_id == 0)
		{
			mprintf("\nCould not create a User task\n");
		}
		//}


	//printf("serialTask Created!\n\r");

	char buf[13];

	sprintf(buf, "\n\rType here: ");
	UART_DRV_SendDataBlocking(myUART_IDX, buf, sizeof(buf), 1000);

	//printf("\nChecking for messages\n");
#ifdef PEX_USE_RTOS
  while (1) {
#endif

	//Handler
	if (interrupt_occur == 1)
	{

		/*Recieve message */
		msg_ptrRead = _msgq_receive(server_qid_r, 0);
		char* inchar = _mem_alloc(sizeof(char));
		_mem_copy(msg_ptrRead->DATA,inchar,sizeof(char));

		numencoding = (int)*inchar;


		interrupt_occur = 0;

		/*If no tasks are open for reading Reloop*/
		if (open_for_read())
		{
			/*Check if valid ASCII character*/ // ^U=21 ^H=8 ^W=23
			if ((numencoding > 31) && (numencoding < 127))
			{
				//Add character to linkedlist to be sent to user tasks
				push(inchar);
				display();
			}
			else
			{
				switch(numencoding)
				{
				case 8: // 8=^H Erase Character
					pop();
					break;
				case 13: // 13= New Line (Enter)
					//Send user strings to user task w/ Read permssion using messsage queue
					//mprintf("\n");
					for(int i = 0;i<PERMLEN;i++)
					{
						if (userqids[i] != 0)
						{
							validcount++;
						}
					}
					_queue_id* validqids = _mem_alloc(sizeof(_queue_id)*validcount+1);
					for(int i = 0;i<PERMLEN;i++)
					{
						if (userqids[i] != 0)
						{
							validqids[i]=userqids[i];
						}
					}
					validqids[validcount]= 0;
					validcount = 0;
					char* sendstring = llconvert();
					//mprintf("SENDING: %s\n", sendstring);
					msg_ptrSend = (POINTER_MESSAGE_PTR) _msg_alloc(pointer_pool);
					msg_ptrSend->HEADER.SOURCE_QID = server_qid_w;
					msg_ptrSend->DATA = sendstring;
					_msgq_send_broadcast(msg_ptrSend, validqids, message_pool);
					_mem_free(validqids);
					//Erase Linked list
					erase();
					break;

				case 21: // 21=^U Erase Line
					while(popstop == 0)
					{
						pop();
					}
					break;

				case 23: //8=^W Erase Word
					while (popstop == 0 && *top->character == ' ' )
					{
						pop();
					}
					while(popstop == 0)
					{
						if (*top->character == ' ')
						{
							break;
						}

						pop();
					}
					break;
				}
			}
		}
			_msg_free(msg_ptrRead);
	}

	/* If putline has been called by task with write permission, recieve message and print to terminal*/
	if(taskwriteline > 0)
	{
		msg_ptrWrite = _msgq_receive(server_qid_w, 0); //server_qid_w
		mprintf("%c",msg_ptrWrite->DATA[0]);
		_msg_free(msg_ptrWrite);
		taskwriteline--;
	}

    OSA_TimeDelay(10);
   

#ifdef PEX_USE_RTOS
 }
#endif
}


/*
** ===================================================================
**     Callback    : user_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void user_task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */
	_task_id tid = _task_get_id();
	mprintf("\n""UserTask Created: %d\n", tid);
	usertaskstart((int*)task_init_data);

#ifdef PEX_USE_RTOS
  while (1) {
#endif
    /* Write your code here ... */


    
    
    OSA_TimeDelay(10);                 /* Example code (for task release) */
   
    
    
    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */

#ifdef __cplusplus
}  /* extern "C" */
#endif 



/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

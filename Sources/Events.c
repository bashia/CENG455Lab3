/* ###################################################################
**     Filename    : Events.c
**     Project     : serial_echoLab2
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-01, 15:35, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include "constants.h"
#include "accessfunctions.h"


#ifdef __cplusplus
extern "C" {
#endif

_pool_id message_pool;


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : myUART_RxCallback
**     Description : This callback occurs when data are received.
**     Parameters  :
**       instance - The UART instance number.
**       uartState - A pointer to the UART driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void myUART_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
	SERVER_MESSAGE_PTR msg_ptr;
	 _queue_id ISR_qid;

	 ISR_qid = _msgq_open((_queue_number)(ISR_QUEUE_BASE), 0);

	 msg_ptr = (SERVER_MESSAGE_PTR) _msg_alloc(message_pool);
	  if(msg_ptr == NULL){
	  //printf("\nCould not allocate a message\n");
	  //_mqx_exit(0);

	  }/* if */
	  //write_qid = client_qid;
	  msg_ptr->HEADER.SOURCE_QID = ISR_qid;
	  msg_ptr->HEADER.TARGET_QID = server_qid_r;  //_msgq_get_id(0, SERVER_QUEUE_R)
	  msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) +
	  strlen((char *)msg_ptr->DATA) + 1;
	  msg_ptr->DATA[0] = (myRxBuff[0]);
	  _msgq_send(msg_ptr);




	interrupt_occur = 1;
	UART_DRV_SendData(myUART_IDX, myRxBuff, sizeof(myRxBuff));


	return;
}

/* END Events */

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

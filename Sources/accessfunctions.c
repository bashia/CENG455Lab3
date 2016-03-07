
#include "Cpu.h"
#include "Events.h"
#include "os_tasks.h"
#include <mqx.h>
#include <message.h>
#include <mutex.h>
#include "accessfunctions.h"
#include "constants.h"
#include "mutex_print.h"

#ifdef __cplusplus
extern "C" {
#endif

void usertaskstart(int commandcode[10])
{
		// _queue_id usermsg_qid_r;
		 _queue_id usermsg_qid_r;
		 _task_id tid = _task_get_id();
		 //PLACEHOLDER
		 char sendstring[128] = {'\0'};
		 int userindex = commandcode[0];
		 usermsg_qid_r = _msgq_open((_queue_number)(USER_QUEUE_BASE_R + userindex), 0);

    int write_qid = 0;
    //while(TRUE)
	for (int i =1; i<10; i++)
	{
		int result = -1;
		switch (commandcode[i])
		{
			case 1: //OpenR
				//PLACEHOLDER

				result = OpenR(usermsg_qid_r);
				if (result == 1)
				{
					mprintf("Read Permission Granted to Task %d\n", tid);
				}
				else if(result == 0)
				{
					mprintf("Read Permission Denied to Task %d\n", tid);
				}
				break;
			case 2: //OpenW
				//printf("OpenW\n");
				result = OpenW();
				if (result > 0)
				{
					mprintf("Write Permission Granted to Task %d\n", tid);
					write_qid = result;
				}
				else if(result == 0)
				{
					mprintf("Write Permission Denied to Task %d\n", tid);
				}
				break;
			case 3: //getline
				mprintf("getline\n");
				result = _getline(sendstring);
				break;
				if(result == 0)
				{
					mprintf("Task %d getline failed\n", tid);
				}
				else
				{
					mprintf("Task %d GOTLINE: %s\n", tid, sendstring);
				}
			case 4: //putline
				mprintf("putline\n");
				if(write_qid == 0)
				{
					mprintf("Task %d has no Write Permission\n", tid);
				}
				else
				{
					result = _putline(result, sendstring);
					if(result == 0)
					{
						mprintf("Task %d Unable to Write\n", tid);
					}
				}

				break;
			case 5: //close
				mprintf("Close\n");
				result = Close();
				if (result == 1)
				{
					mprintf("Permissions Revoked from Task %d\n", tid);
				}
				else if(result == 0)
				{
					mprintf("Close Failed. Task %d has no Permissions\n", tid);
				}

				break;

		}
	}


}



int arraycontains(int array[], int value)
{
	for (int i = 0; i < sizeof(array); i++)
	{
		if (array[i] == value)
		{
			return i;
		}

	}
	return -1;
}

int open_for_read()
{
	for (int i = 0; i < PERMLEN; i++)
	{
		/*Check if tasks are open for reading */
		if (readpermission[i] > 0)
		{
			return 1;
		}
	}
	return 0;
}


//Gives Read Permission to Task
int OpenR(uint16_t stream_no)
{
	/*get user id */
	_task_id tid = _task_get_id();

	if (stream_no == 0)
	{
		mprintf("BADvic READ QID\n");
		return FAILURE;
	}

	if (arraycontains(readpermission, (int)tid) != -1)
	{
		return FAILURE;
	}
	else
	{
		readpermission[readindex] = (int)tid;
		userqids[readindex] = (_queue_id)stream_no;
		readindex += 1;
		userqids[readindex] =  MSGQ_NULL_QUEUE_ID;
		return SUCCESS;
	}

}

int _getline(char* string)
{
	_task_id tid = _task_get_id();
	POINTER_MESSAGE_PTR msg_ptrWrite;

	int index = arraycontains(readpermission, (int)tid);
			if (index != -1)
			{
				_queue_id readqid = userqids[index];
				msg_ptrWrite = _msgq_receive(readqid, 0);
				char *gotline = msg_ptrWrite->DATA;
				_mem_copy(gotline,string,strlen(gotline)+1);
				return SUCCESS;
			}

	return FAILURE;
}
//Gives write Permission to Task
_queue_id OpenW(void)
{
	if (writepermission == 0)
	{
		_task_id tid = _task_get_id();
		writepermission = (int)tid;

		_queue_id usermsg_qid_w = _msgq_open((_queue_number)(USER_QUEUE_BASE_W), 0);
		userwriteqid = usermsg_qid_w;
		return usermsg_qid_w;
	}
	else
	{
		return FAILURE;
	}

}

int _putline(_queue_id qid, char* string)
{
	SERVER_MESSAGE_PTR msg_ptr;
	bool            result;
	_task_id tid = _task_get_id();
	//_queue_id write_qid;
	if (writepermission == tid)
	{
			  if(msg_ptr == NULL){
			  mprintf("\nCould not allocate a message\n");
			  return -1;
			  }
			  else
			  {
				  mprintf("SENDING A MESSAGE FROM %d\n", tid);
				  //write_qid =

				  for(int i= 0; i<=strlen(string); i++)
				  {
				      /*Create and send message*/
					  msg_ptr = (SERVER_MESSAGE_PTR) _msg_alloc(message_pool);

					  msg_ptr->HEADER.SOURCE_QID = qid;
					  msg_ptr->HEADER.TARGET_QID = server_qid_w;
					  msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) +
							  strlen((char *)msg_ptr->DATA) + 1;
					  if (i == strlen(string))
					  {
						  msg_ptr->DATA[0] = '\n';
					  }
					  else
					  {
					  msg_ptr->DATA[0] = (string[i]);
					  }


					  result = _msgq_send(msg_ptr);
					  taskwriteline++;
					  if (result != TRUE)
					  {
						  printf("\nCould not send a message\n");
						  printf("%d\n", _task_errno);
					  }
				  }

			  return SUCCESS;
			  }
	}
	else
	{
	return FAILURE;
	}
}

//Revokes read & write Permission from Task
int Close(void)
{
	_task_id tid = _task_get_id();
		int privrevoked = 0;
		//Revoke Write Permission
		if (writepermission == (int)tid)
		{
			writepermission = 0;
			_msgq_close(userwriteqid);
			userwriteqid = 0;
			privrevoked = 1;
		}
		//Revoke Read Permission
		int index = arraycontains(readpermission, (int)tid);
		if (index != -1)
		{
			readpermission[index] = 0;
			userqids[index] = -1;
			privrevoked = 1;
		}

		if (privrevoked == 1)
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

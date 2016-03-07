/*
 * accessfunctions.h
 *
 *  Created on: Feb 10, 2016
 *      Author: cccarr
 */

#ifndef SOURCES_ACCESSFUNCTIONS_H_
#define SOURCES_ACCESSFUNCTIONS_H_

#include "constants.h"


void usertaskstart(int commandcode[8]);

int Open_for_Read();

int OpenR(uint16_t stream_no);

int _getline(char* string);

_queue_id OpenW(void);

int _putline(_queue_id qid, char* string);

int Close(void);


#endif /* SOURCES_SERVER_H_ */

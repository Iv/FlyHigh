/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <strings.h>
#include <time.h>
#include "flytec_pl.h"

static int ftty = -1;

int flytec_pl_init(const char* pDevName)
{
	struct termios sTermSet;

	ftty = open(pDevName, O_RDWR | O_NOCTTY | O_NONBLOCK);
	
	if(ftty == -1)
	{
	#ifdef _PL_DEBUG
		printf("cannot open %s\n", pDevName);
	#endif
		return -1;
	}
	
	if(isatty(ftty))
	{
		/* new port settings */
		bzero(&sTermSet, sizeof(sTermSet)); 
		sTermSet.c_iflag = IGNPAR | IXON | IXOFF;
		sTermSet.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
		sTermSet.c_lflag = 0;
		sTermSet.c_cc[VSTART]   = 0x11;     /* DC1 */
		sTermSet.c_cc[VSTOP]    = 0x13;     /* DC3 */
	
		if(tcsetattr(ftty, TCSANOW, &sTermSet) == -1)
		{
		#ifdef _PL_DEBUG
			printf("couldn't set %s status\n", pDevName);
		#endif
			return -1;
		}
	}
	return 0;
} 

int flytec_pl_getChar(u_char *pch)
{
	const time_t waitTime = 3;
	time_t timeout = time(0) + waitTime;
	int res = -1;
		
	if(ftty > 0)
	{
		do
		{
			if(read(ftty, pch, 1) > 0)
			{
				res = 0;
			#ifdef _PL_DEBUG
				printf("flytec_pl_getByte: %c\n", *pch);
			#endif
				break;
			}
		}while(time(0) < timeout);
	}
	
	return res;
}

int flytec_pl_putChar(u_char ch)
{
	int res = -1;
	
	if((ftty > 0) && (write(ftty, &ch, 1) > 0))
	{
		res = 0;
	#ifdef _PL_DEBUG
		printf("flytec_pl_putByte: %c\n", ch);
	#endif
	}
	
	return res;
}

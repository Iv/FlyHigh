/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <stdio.h>
#include <unistd.h>
#include "flytec_ll.h"
#include "flytec_pl.h"

static u_char calculateZZ(u_char *pData, u_char size);
static int cmpRecieveZZ(u_char calcZZ);
static void sendZZ(u_char ZZ);

int flytec_ll_recieve(PacketType id, u_char *pData, u_char *pSize)
{
	int res = -1;
	u_char charNr;
	u_char calcZZ;
	u_char ch;
	u_char size = 0;
	
	if(id == ProductResp)
	{ /* lookup for header */
		for(charNr=0; charNr<255; charNr++)
		{
			if(flytec_pl_getChar(&ch) < 0)
			{
				return -1;
			}
			
			if(ch == '$')
			{
				break;
			}
		}
	}
	
	for(charNr=0; charNr<255; charNr++)
	{
		if(flytec_pl_getChar(&ch) < 0)
		{
			return -1;
		}

		/* break for Product resp */
		if((ch == '*') && (id == ProductResp))
		{
			calcZZ = calculateZZ(&pData[0], size);
			res = cmpRecieveZZ(calcZZ);
			break;
		}
		
		/* save the char */
		pData[size] = ch;
		size++;
		
		/* break for Track respond */
		if((ch == '\n') && (id == TrackResp))
		{
			res = 0;
			break;
		}
	}
	
	if(res == 0)
	{
		*pSize = size;
	}
	
	return res;
}

int cmpRecieveZZ(u_char calcZZ)
{
	u_char strRecZZ[2];
	u_char ch;
	u_char strCalcZZ[3];
	int res = -1;
	
	/* compare checksum */
	sprintf(&strCalcZZ[0], "%02X", (int)calcZZ);
	flytec_pl_getChar(&strRecZZ[0]);
	flytec_pl_getChar(&strRecZZ[1]);
	
	if((strRecZZ[0] == strCalcZZ[0]) && (strRecZZ[1] == strCalcZZ[1]))
	{
		res = 0;
	}
	
	/* flush the end of telegram */
	flytec_pl_getChar(&ch);
	flytec_pl_getChar(&ch);

	return res;
}

int flytec_ll_send(u_char *pData, u_char size)
{
	int res = -1;
	u_char charNr;
	u_char checkSum = 0;
	
	/* put the header */
	flytec_pl_putChar('$');
	
	/* send request */
	for(charNr=0; charNr<size; charNr++)
	{
		checkSum ^= pData[charNr];
		if(flytec_pl_putChar(pData[charNr]) < 0)
		{
			return -1;
		}
	}
	
	/* send checksum  */
	sendZZ(checkSum);
	
	if(charNr == size)
	{
		res = 0;
	}
	
	/* delay as recommended by flytec */
	usleep(200*1000);
	
	return res;
}

static u_char calculateZZ(u_char *pData, u_char size)
{
	u_char charNr;
	u_char ZZ = 0;
		
	for(charNr=0; charNr<size; charNr++)
	{
		ZZ ^= (*pData);
		pData++;
	}
	
	return ZZ;
}

static void sendZZ(u_char ZZ)
{
	u_char buff[3];
	
	sprintf(&buff[0], "%02X", (int)ZZ);
	
	/* send checksum  */
	flytec_pl_putChar('*');
	flytec_pl_putChar(buff[0]);
	flytec_pl_putChar(buff[1]);
	
	/* end of request */
	flytec_pl_putChar('\r');
	flytec_pl_putChar('\n');
}

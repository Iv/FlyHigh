/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _flytec_ll_h
#define _flytec_ll_h

#include <sys/types.h>

typedef enum
{
	ProductResp,
	TrackResp
}PacketType;

extern int flytec_ll_recieve(PacketType id, u_char *pData, u_char *pSize);
extern int flytec_ll_send(u_char *pData, u_char size);

#endif

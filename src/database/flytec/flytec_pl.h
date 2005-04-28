/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _flytec_pl_h_
#define _flytec_pl_h_

#include <sys/types.h>

/* 0 = OK, -1 = fail */
extern int flytec_pl_init(const char* pDevName);
extern int  flytec_pl_exit();
extern int flytec_pl_getChar(u_char *pch);
extern int flytec_pl_putChar(u_char ch);

#endif

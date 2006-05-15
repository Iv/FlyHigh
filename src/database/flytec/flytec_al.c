/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "flytec_al.h"
#include "flytec_ll.h"
#include "flytec_pl.h"

#define BUFF_SIZE 255

static const u_char PageSize = 8;
static u_char buff[BUFF_SIZE];

static void latlonToString(double lat, double lon, u_char *pBuff);
static void degToString(double value, u_char* pbuff, u_char size);
static double stringToDeg(u_char *pstr, u_char size);

int ft_init(const char *pDevName)
{
	return flytec_pl_init(pDevName);
}

int ft_exit()
{
	return flytec_pl_exit();
}
 
int ft_deviceInfoRead(DeviceInfoType *pDeviceInfo)
{
	int res;
	u_char len;

	sprintf(&buff[0],  "PBRSNP,");
	len = strlen(&buff[0]);
	
	/* send the request */
	res = flytec_ll_send(&buff[0], len);
	
	if(res == 0)
	{
		res = flytec_ll_recieve(ProductResp, &buff[0], &len);
		
		if(res == 0)
		{
			res = memcmp(&buff[0], "PBRSNP", 6);
		}
	}
	
	/* read the response */
	if(res == 0)
	{
		/* device ident */
		memcpy(&pDeviceInfo->deviceIdent[0], &buff[7], 4);
		pDeviceInfo->deviceIdent[4] = '\0';

		/* pilot name */
		ft_ftstring2string(&pDeviceInfo->pilotName[0], &buff[12]);
	
		/* serial number */
		pDeviceInfo->serialNr = atoi(&buff[30]);
		
		/* SW Version */
		memcpy(&pDeviceInfo->swVersion[0], &buff[36], 4);
		pDeviceInfo->swVersion[4] = '\0';
	}
	else
	{
		res = -1;
	}
	
	return res;
}

int ft_updateConfiguration()
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRCONF,");
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
	
	return res;
}

/*******************************************************************************************
CTR SERVICES
*******************************************************************************************/
int ft_ctrListReq()
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRCTR,");
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
	usleep(1000*1000);
	
	return res;
}

int ft_ctrListRec(ft_CTRType *pCTR)
{
	u_char len;
	int res;
	
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRCTR", 6);
	}
	
	if(res == 0)
	{
		/* toal sentences */
		buff[10] = '\0';
		pCTR->totalSent = atoi(&buff[7]);
		
		/* actual sentence */
		buff[14] = '\0';
		pCTR->actSent = atoi(&buff[11]);
		
		if(pCTR->actSent == 0)
		{ 
			/* ctr name */
			ft_ftstring2string(pCTR->sent.first.name, &buff[15]);
			
			/* warning distance */
			buff[37] = '\0';
			pCTR->sent.first.warnDist = atoi(&buff[33]);
		}
		else if(pCTR->actSent == 1)
		{
			/* remark */
			ft_ftstring2string(pCTR->sent.second.remark, &buff[15]);
		}
		else
		{ /* a member */
			pCTR->sent.member.type = buff[15];
			pCTR->sent.member.latitude = stringToDeg(&buff[17], 10);
			pCTR->sent.member.longitude = stringToDeg(&buff[28], 11);
			
			if(pCTR->sent.member.type == 'C') /* circle */
			{
				/* radius */
				buff[45] = '\0';
				pCTR->sent.member.radius = (uint)atoi(&buff[40]);
			}
			else if((pCTR->sent.member.type == 'T') /* start segment */ ||
					(pCTR->sent.member.type == 'Z')) /* end segment */
			{
				pCTR->sent.member.direction = buff[40];
			}
		}
	}
	
	return res;
}

int ft_ctrSnd(ft_CTRType *pCTR)
{
	u_char len;
	
	sprintf(&buff[0],  "PBRCTRW,%03d,%03d,", pCTR->totalSent, pCTR->actSent);

	if(pCTR->actSent == 0)
	{
		/* name */
		ft_string2ftstring(pCTR->sent.first.name, &buff[16]);
		buff[33] = ',';
		
		/* warning distance */
		sprintf(&buff[34], "%04d", pCTR->sent.first.warnDist);
		len = 38;
	}
	else if(pCTR->actSent == 1)
	{
		/* remark */
		ft_string2ftstring(pCTR->sent.first.name, &buff[16]);
		len = 33;
	}
	else
	{
		buff[16] = pCTR->sent.member.type;
		buff[17] = ',';
		latlonToString(pCTR->sent.member.latitude, pCTR->sent.member.longitude, &buff[18]);
			
		if((pCTR->sent.member.type == 'P') /* point */ ||
			(pCTR->sent.member.type == 'X') /* center */)
		{
			len = 40;
		}
		else if(pCTR->sent.member.type == 'C') /* circle */
		{
			buff[40] = ',';

			/* radius */
			sprintf(&buff[41], "%05d", pCTR->sent.member.radius);
			len = 46;
		}
		else if((pCTR->sent.member.type == 'T') /* start segment */ ||
				(pCTR->sent.member.type == 'Z')) /* end segment */
		{
			buff[40] = ',';
			
			/* direction */
			buff[41] = pCTR->sent.member.direction;
			len = 42;
		}
	}
	
	return flytec_ll_send(&buff[0], len);
}

extern int ft_ctrDel(const char *pName)
{
	u_char len;
	
	sprintf(&buff[0],  "PBRCTRD,");

	if(pName == NULL)
	{ /* delete all */
		buff[8] = ',';
		len = 9;
	}
	else
	{ /* delete one */
		ft_string2ftstring(pName, &buff[8]);
		len = 25;
	}
	
	return flytec_ll_send(&buff[0], len);
}

int ft_ctrRecAck()
{
	u_char len;
	int res;
	
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRANS,", 7);
	}

	if(res == 0)
	{
		if(buff[7] != '1') /* Acknowledge */
		{
			res = -1;
		}
	}
	
	return res;
}

/*******************************************************************************************
WAYPOINT SERVICES
*******************************************************************************************/
int ft_wayPointListReq()
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRWPS,");
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
	usleep(500*1000);
	
	return res;
}

int ft_wayPointListRec(ft_WayPointType *pft_WayPoint)
{
	u_char len;
	int res;
	
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRWPS", 6);
	}
	
	if(res == 0)
	{
		/* latitude */
		pft_WayPoint->latitude = stringToDeg(&buff[7], 10);
	
		/* longitude */
		pft_WayPoint->longitude = stringToDeg(&buff[18], 11);
		
		/* name */
		ft_ftstring2string(&pft_WayPoint->name[0], &buff[37]);
		
		/* altitude */
		buff[59] = '\0';
		pft_WayPoint->altitude = atoi(&buff[55]);
	}
	
	return res;
}

int ft_wayPointSnd(ft_WayPointType *pft_WayPoint)
{
	sprintf(&buff[0],  "PBRWPR,");

	/* lat, lon */
	latlonToString(pft_WayPoint->latitude, pft_WayPoint->longitude, &buff[7]);
	buff[29] = ',';

	/* ? */
	buff[30] = ',';

	/* name */
	ft_string2ftstring(&pft_WayPoint->name[0], &buff[31]);
	buff[48] = ',';
	
	/* altitude */
	sprintf(&buff[49], "%04d", pft_WayPoint->altitude);
	
	return flytec_ll_send(&buff[0], 53);
}

int ft_wayPointDel(const char *pName) /* NULL = delete all */
{
	u_char len;
	
	sprintf(&buff[0],  "PBRWPX,");
	
	if(pName == NULL)
	{ /* delete all */
		buff[7] = ',';
		len = 8;
	}
	else
	{ /* delete one */
		ft_string2ftstring(pName, &buff[7]);
		len = 24;
	}
	
	return flytec_ll_send(&buff[0], len);
}

/*******************************************************************************************
ROUTE SERVICES
*******************************************************************************************/
int ft_routeListReq()
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRRTS,");
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
	usleep(500*1000);
	
	return res;
}

int ft_routeListRec(ft_RouteType *pft_Route)
{
	u_char len;
	int res;
	
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRRTS", 6);
	}
	
	if(res == 0)
	{
		/* route number */
		buff[9] = '\0';
		pft_Route->routeNum = atoi(&buff[7]);
		
		/* toal sentences */
		buff[12] = '\0';
		pft_Route->totalSent = atoi(&buff[10]);
		
		/* actual sentence */
		buff[15] = '\0';
		pft_Route->actSent = atoi(&buff[13]);
		
		if(pft_Route->actSent == 0)
		{ /* this is the route name */
			ft_ftstring2string(pft_Route->name, &buff[16]);
		}
		else
		{ /* this is a waypoint */
			ft_ftstring2string(pft_Route->name, &buff[23]);
		}
	}
	
	return res;
}

int ft_routeSnd(ft_RouteType *pft_Route)
{
	u_char strStart;
	u_char len;
	
	sprintf(&buff[0],  "PBRRTR,%02d,%02d,%02d,", pft_Route->routeNum, pft_Route->totalSent,
				pft_Route->actSent);
				
	strStart = strlen(&buff[0]);

	if(pft_Route->actSent > 0)
	{
		buff[strStart] = ',';
		strStart++;
	}

	ft_string2ftstring(pft_Route->name, &buff[strStart]);
	len = strStart + 17;
	
	return flytec_ll_send(&buff[0], len);
}

int ft_routeDel(const char *pName) /* NULL = delete all */
{
	u_char strStart;
	u_char len;
	
	sprintf(&buff[0],  "PBRRTX,");
	
	strStart = strlen(&buff[0]);

	if(pName == NULL)
	{ /* delete all */
		buff[strStart] = ',';
		len = strStart + 1;
	}
	else
	{ /* delete one */
		ft_string2ftstring(pName, &buff[strStart]);
		len = strStart + 17;
	}
	
	return flytec_ll_send(&buff[0], len);
}

/*******************************************************************************************
TRACK SERVICES
*******************************************************************************************/
int ft_trackListReq()
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRTL,");
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
	
	return res;
}

int ft_trackListRec(TrackType *pTrack)
{
	u_char len;
	int res;
	
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRTL", 5);
	}
	
	if(res == 0)
	{
		/* nof tracks */
		buff[8] = '\0';
		pTrack->totalNum = atoi(&buff[6]);
		
		/* track number */
		buff[11] = '\0';
		pTrack->trackNum = pTrack->totalNum - atoi(&buff[9]) - 1;
		
		/* date */
		buff[14] = '\0';
		pTrack->date.day= atoi(&buff[12]);
		buff[17] = '\0';
		pTrack->date.month= atoi(&buff[15]);
		buff[20] = '\0';
		pTrack->date.year= 2000 + atoi(&buff[18]);
		
		/* start */
		buff[23] = '\0';
		pTrack->start.hour = atoi(&buff[21]);
		buff[26] = '\0';
		pTrack->start.min = atoi(&buff[24]);
		buff[29] = '\0';
		pTrack->start.sec = atoi(&buff[27]);
		
		/* duration */
		buff[32] = '\0';
		pTrack->duration.hour = atoi(&buff[30]);
		buff[35] = '\0';
		pTrack->duration.min = atoi(&buff[33]);
		buff[38] = '\0';
		pTrack->duration.sec = atoi(&buff[36]);
	}
	
	return res;
}

int ft_trackReq(u_int trackNum)
{
	int res;
	u_char len;
	
	sprintf(&buff[0],  "PBRTR,%02d", trackNum);
	len = strlen(&buff[0]);
	
	res = flytec_ll_send(&buff[0], len);
/*	usleep(500*1000); */
	
	return res;
}

int ft_trackRec(u_char *pLine, u_char *pSize)
{
	int res;

	res = flytec_ll_recieve(TrackResp, pLine, pSize);
	
	return res;
}

/*******************************************************************************************
MEMORY SERVICES
*******************************************************************************************/
int ft_memoryRead(u_int addr, u_char *pPage)
{
	int res;
	u_char len;
	u_char byteNr;
	u_char start;
	
	/* valid address? */
	if((addr + ft_PageSize) > ft_MemSize)
	{
		return -1;
	}

	/* request */
	sprintf(&buff[0],  "PBRMEMR,%04X", addr);
	len = strlen(&buff[0]);
	res = flytec_ll_send(&buff[0], len);

	/* answer */
	res = flytec_ll_recieve(ProductResp, &buff[0], &len);
	
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRMEMR", 7);
	}
	
	if(res == 0)
	{
		start = 13;
		
		for(byteNr=0; byteNr<PageSize; byteNr++)
		{
			buff[start+2] = '\0';
			pPage[byteNr] = strtol(&buff[start], (char **)NULL, 16);
			start += 3;
		}
	}

	return res;
}

int ft_memoryWrite(u_int addr, u_char *pPage)
{
	int res;
	u_char byteNr;
	u_char buffSize;
	
	/* valid address? */
	if((addr + ft_PageSize) > ft_MemSize)
	{
		return -1;
	}
	
	sprintf(&buff[0],  "PBRMEMW,%04X,%d", addr, ft_PageSize);
	buffSize = 14;

	for(byteNr=0; byteNr<ft_PageSize; byteNr++)
	{
		sprintf(&buff[buffSize], ",%02X", pPage[byteNr]);
		buffSize += 3;
	}
	
	res = flytec_ll_send(&buff[0], buffSize);

	/* read the answer */
	res = flytec_ll_recieve(ProductResp, &buff[0], &buffSize);
		
	if(res == 0)
	{
		res = memcmp(&buff[0], "PBRMEMR", 7);
	}
	
	return res;
}

/*******************************************************************************************
HELPERS
*******************************************************************************************/
/* Format: "dddd.mmm,[N|S],ddddd.mmm,[E|W]" */
void latlonToString(double lat, double lon, u_char *pBuff)
{
	double value;

	/* latitude */
	if(lat >= 0)
	{
		pBuff[9] = 'N';
		value = lat;
	}
	else
	{
		pBuff[9] = 'S';
		value = -lat;
	}
	
	pBuff[8] = ',';
	degToString(value, &pBuff[0], 8);
	pBuff[10] = ',';
	
	/* longitude */
	if(lon >= 0)
	{
		pBuff[21] = 'E';
		value = lon;
	}
	else
	{
		pBuff[21] = 'W';
		value = -lon;
	}
	
	pBuff[20] = ',';
	degToString(value, &pBuff[11], 9);
}

void degToString(double value, u_char* pbuff, u_char size)
{
	#define STR_SIZE 12
	u_char strValue[STR_SIZE];
	u_char strLen;
	double intValue;

	/* dec to min */
	intValue = floor(value);
	value = intValue * 100 + (value - intValue) * 60;
	
	if(size == 8)
	{
		snprintf(&strValue[0], STR_SIZE, "%4.3f", value);
	}
	else
	{
		snprintf(&strValue[0], STR_SIZE ,"%5.3f", value);
	}
	strLen = strlen(&strValue[0]);
	
	memset(&pbuff[0], '0', size);
	memcpy(&pbuff[size-strLen], &strValue[0], strLen);
}

/* format: "dddd[d].mmm,[N|S|E|W]" */
double stringToDeg(u_char *pstr, u_char size)
{
	double value;
	double intValue;

	pstr[size-2] = '\0';
	value = atof(pstr) / 100;
	
		/* dec to min */
	intValue = floor(value);
	value = intValue + (value - intValue) * 100 / 60;
	
	if((pstr[size] == 'W') || (pstr[size] == 'S'))
	{
		value *= (-1); /* negate values below equator and west*/
	}
	
	return value;
}

void ft_string2ftstring(const char *pstr, char *pftstr)
{
	u_char strLen;
	
	memset(&pftstr[0], ' ', FT_STRING_SIZE);
	strLen = strlen(&pstr[0]);
	
	if(strLen > FT_STRING_SIZE)
	{
		strLen = FT_STRING_SIZE;
	}
	
	memcpy(&pftstr[0], &pstr[0], strLen);
}

void ft_ftstring2string(char *pstr, const char *pftstr)
{
	u_char strLen;
	
	for(strLen=FT_STRING_SIZE;strLen>0;strLen--)
	{
		if(pftstr[strLen-1] != ' ')
		{
			break;
		}
	}
	
	memcpy(&pstr[0], &pftstr[0], strLen);
	pstr[strLen] = '\0';
}

/* Copyright (c):  2004 by Alex Graf
	This file is distributed under the terms of the General Public
	Licence. See the file gpl.txt for the Licence or visit gnu.org 
	for more information.
*/

#ifndef _flytec_al_h_
#define _flytec_al_h_

#include <sys/types.h>

#define FT_STRING_SIZE 17

#define PILOT_NAME_POS 0
#define LCD_CONTRAST_POS 32
#define QNH_CORR_A1_POS 54
/*#define SPEED_GAIN_PRESS_POS 72 */
#define RESP_DELAY_POS 73
#define BASE_VOL_BEEP_POS 74
/*#define TEC_POS 75 */
#define VARIO_OFFSET_POS 76
#define ACOUSTIC_SINK_POS 77
#define TMP_CORR_POS 78
/*#define POLARE_SINK_1_0_POS 85 */
/*#define POLARE_SINK_1_1_POS 86 */
/*#define POLARE_SPEED_1_0_POS 87 */
/*#define POLARE_SPEED_1_1_POS 88 */
#define REC_STOP_MODE_POS 89
#define SPEED_DISPLAY_POS 90
/*#define POLARE_ALT_1_POS 91 */
#define UTC_OFFSET_POS 92
#define STALL_SPEED_POS 94
#define STALL_ALT_POS 95
#define REC_INTERVAL_POS 97
#define ACOUSTIC_I_POS 100
#define UP_BASE_FRQ_POS 102
#define DOWN_BASE_FRQ_POS 104
#define FRQ_MODULATION_POS 110
#define SINC_ACOUSTIC_EN_POS 119
#define VARIOMODE_POS 120
#define I_TIME_POS 121
/*#define POLARE_SINK_2_0_POS 122 */
/*#define POLARE_SINK_2_1_POS 123 */
/*#define POLARE_SPEED_2_0_POS 124 */
/*#define POLARE_SPEED_2_1_POS 125 */
/*#define POLARE_ALT_2_POS 126 */
#define SPEED_GAIN_WHEEL_POS 128
#define UNITS_POS 141
#define GRID_SYS_POS 142
#define QNH_CORR_A2_POS 147
/*#define PRESS_SPEED_USAGE_POS 149 */
#define USER_SET_PAGE_POS 156
#define DAY_RISING_AV_POS 157
#define ACOUSTIC_PITCH_POS 158
#define SW_VERSION_POS 159
#define LAST_THERM_THERS_POS 185
#define COMPANY_POS 186
#define SPEED_SCALING_POS 188
#define GLYDER_TYPE_POS 192
#define GLYDER_ID_POS 224
#define GEO_ID_POS 316
#define USERFIELD_0_POS 320
#define USERFIELD_1_POS 330
#define USERFIELD_2_POS 340
#define BEST_LD_POS 350
#define BATT_TYPE_POS 351
#define SMS_REC_NR_POS 353
#define SMS_COMP_MODE_POS 378
#define SMS_CYCL_RATE_POS 379
#define SMS_EMERGENCY_POS 380

static const u_char ft_PageSize = 8;
static const u_int ft_MemSize = 347;

/* Primitive Datatypes */
typedef char FlytecStringType[FT_STRING_SIZE]; /* '\0' always terminated */

typedef struct FlytecDate
{
	u_char day;
	u_char month;
	u_int year;
}FlytecDateType;

typedef struct FlytecTime
{
	u_int hour;
	u_int min;
	u_int sec;
}FlytecTimeType;

/* Req/Answ Datatypes */
typedef struct DeviceInfo
{
	char deviceIdent[5];
	FlytecStringType pilotName;
	u_int serialNr;
	char swVersion[5];
}DeviceInfoType;

typedef struct ft_WayPoint
{
	double latitude;
	double longitude;
	int altitude;
	FlytecStringType name;
}ft_WayPointType;

typedef struct ft_Route
{
	u_int routeNum; /* 0 = FAI route */
	u_int totalSent;
	u_int actSent; /* 0 = ft_Route Name */
	FlytecStringType name;
}ft_RouteType;

typedef struct Track
{
	u_int totalNum;
	u_int trackNum;
	FlytecDateType date;
	FlytecTimeType start;
	FlytecTimeType duration;
}TrackType;

typedef struct CTR
{
	u_int totalSent;
	u_int actSent; /* 0 = first,  1 = second, >1 = member */
	union
	{
		struct
		{
			FlytecStringType name;
			u_int warnDist;
		}first;
		struct
		{
			FlytecStringType remark;
		}second;
		struct
		{
			char type; /* P = Point, C = Circle, X = Center, T = Segment begin, Z = Segment end */
			double latitude;
			double longitude;
			uint radius;
			char direction; /* + = clockwise */
		}member;
	}sent;
}ft_CTRType;

/* services */
extern int ft_init(const char *pDevName);
extern int ft_exit();
extern int ft_deviceInfoRead(DeviceInfoType *pDeviceInfo);

extern int ft_memoryRead(u_int addr, u_char *pPage);
extern int ft_memoryWrite(u_int addr, u_char *pPage);
extern int ft_updateConfiguration();

 /* max 500 CTRs with up to 100 waypoints (depends on memory)*/
extern int ft_ctrListReq();
extern int ft_ctrListRec(ft_CTRType *pCTR);
extern int ft_ctrSnd(ft_CTRType *pCTR);
extern int ft_ctrDel(const char *pName);
extern int ft_ctrRecAck();

extern int ft_trackListReq();
extern int ft_trackListRec(TrackType *pTrack);
extern int ft_trackReq(u_int trackNum);
extern int ft_trackRec(u_char *pLine, u_char *pSize);

extern int ft_wayPointListReq();
extern int ft_wayPointListRec(ft_WayPointType *pft_WayPoint);
extern int ft_wayPointSnd(ft_WayPointType *pft_WayPoint);
extern int ft_wayPointDel(const char *pName); /* NULL = delete all */

/* max 20 routes with max 30 waypoints */
extern int ft_routeListReq();
extern int ft_routeListRec(ft_RouteType *pft_Route);
extern int ft_routeSnd(ft_RouteType *pft_Route);
extern int ft_routeDel(const char *pName); /* NULL = delete all */

extern void ft_string2ftstring(const char *pstr, char *pftstr);
extern void ft_ftstring2string(char *pstr, const char *pftstr);

#endif

/* trconfig.h
**
** Copyright (C) 1999  Thurman Gillespy IIII
** 1/17/99
*/

#ifndef TRCONFIG_H
#define TRCONFIG_H

#define kTRASH_ALLOC_VAL	0xA5	/* value to intialize new memory */
#define kTRASH_FREE_VAL		0xA3	/* trash free memory value */

/* user macros */
#define TRC_PRINT(p,x)		TRC__Print(p,x)
#define TRC_USER_DATA(p)	((USER_DATA_p_t)TRC__GetUserData(p))

/* user data */
/* this structure 'overrides' the incomplete TRC_USER_DATA_t typedef */
typedef struct user_s {
	unsigned long		magic;		/* 'USER' */
	/* user data here */
} USER_DATA_t, *USER_DATA_p_t;


#endif /* TRCONFIG_H */
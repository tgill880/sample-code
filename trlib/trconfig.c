/* trconfig.c
**
** implementation dependent routines for the TRACE library (trlib)
** change functions as needed for local environment
** see manual.txt file for additional information
**
** Standard C Library version
**
** Copyright (C) 1999 Thurman Gillespy III
** 1/17/99
*/

#include "trlib.h"
#include "trconfig.h"
/* add includes as needed */
#include <stdio.h>
#include <string.h>
#include <time.h>


/*********************************/
/*    TRACE memory management    */
/*********************************/

/*
** allocate size bytes, return pointer to allocated memory
*/
void * TRC__MemAlloc(size_t size)
{
	void	*ptr = NULL;
	
	ptr = malloc(size);
	if (ptr != NULL)
		TRC__TrashMemAlloc(ptr, size);
	
	return ptr;

} /* TRC__MemAlloc */

/*
** free memory associated with ptr
*/
void TRC__MemFree(void *ptr, long size)
{
	 if (ptr != NULL) {
	 	TRC__TrashMemFree(ptr, size);
	 	free(ptr);
	 }

} /* TRC__MemFree */

/*
** set value of newly allocated memory to kTRASH_ALLOC_VAL (config.h)
** called immediately after TRC__MemAlloc
*/
void TRC__TrashMemAlloc(void *ptr, size_t size)
{
	memset(ptr, kTRASH_ALLOC_VAL, size);
} /* TRC__TrashMemAlloc */

/*
** set value of memory to kTRASH_FREE_VAL
** called immediately before TRC__MemFree
** if size of memory not known, called with -1
*/
void TRC__TrashMemFree(void *ptr, long size)
{
	if (size > 0)
		memset(ptr, kTRASH_FREE_VAL, (size_t)size);
} /* TRC__TrashMemAlloc */


/****************************************/
/*    TRACE output facility routines    */
/****************************************/

void TRC__PrintStd(USER_DATA_p_t user_data, const char *str)
{
	printf("%s", str);
} /* TRC__Print */

void TRC__InitOutput(TRACE_t tr, void *obj)
{
	USER_DATA_p_t	usr = NULL;
	
	/* initialize user data here */
	usr = TRC_USER_DATA(tr);
	
	TRC_PRINT(tr, "\nBegin TRACE log.\n");
	TRC_PRINT(tr, "Version ");
	TRC_PRINT(tr, TRC__GetVersion(tr));
	TRC_PRINT(tr, "\n");
	if (TRC__BufferIsFull(tr))
		TRC_PRINT(tr, "\n");
} /* TRC_InitOutput */


void TRC__CloseOutput(TRACE_t tr)
{
	TRC_PRINT(tr, "\nClose TRACE log.\n");
	
} /* TRC_CloseOutput */

void TRC__AbortOutput(TRACE_t tr, const char *str)
{
	TRC_PRINT(tr, "\nAbort: error processing TRACE output.\n");
} /* TRC_AbortOutput */



/*********************************/
/*    Miscellaneous Functions    */
/********************************/

/*
** returns size in bytes of user struct
** used by trlib when allocating memory
*/
int TRC__SizeOfUserStruct(void)
{
	return sizeof(USER_DATA_t);
}

/* time support */
time_t TRC__GetTime(void)
{
	return clock();	/* time.h */
} /* TRC__GetTime */

/*
** abort function for TRACE init
** accepts: error code (INIT_ERR__t)
**       pointer not NULL
**       wrong size parameter
**       out of memory
*/
void TRC__AbortInit(INIT_ERR__t err)
{
	switch (err) {
		case INITERR_POINTER_NOT_NULL:
			printf("TRACE init error: TRACE object pointer not NULL.\n");
			printf("usage:\n");
			printf("   TRACE_t tr = NULL;\n");
			printf("   TRACE_INIT(&tr, 1024);\n");
			break;
		case INITERR_INVALID_SIZE:
			printf("TRACE init error: improper size paramter.\n");
			printf("usage:\n");
			printf("   TRACE_INIT(&tr, size);\n");
			printf("   where size >= %d and size <= %d\n", 512, 30000);
			printf("   size = -1: default buffer size\n");
			break;
		case INITERR_OUT_OF_MEMORY:
			printf("TRACE init error: out of memory.\n");
			break;
	}
	
	exit(0);
} /* TRC__AbortInit */
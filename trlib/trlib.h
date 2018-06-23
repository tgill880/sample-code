/* trlib.h
**
** public interface for the TRACE library (trlib)
**
** Copyright (C) 1999 Thurman Gillespy III
** 2/2/99
*/

#ifndef TRLIB_H
#define TRLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>	/* size_t */
#include <time.h>		/* time_t */

/* new line character(s): unix \n, Mac \r, Windows ? */
/* must be double quoted */
#define NEWLINE	"\r"

/* incomplete (hidden) pointers to the TRACE object */
/* and user data */
typedef struct trace_s		*TRACE_t;
typedef struct user_s		*TRC_USER_DATA_t;


/* typedefs for function pointers */
typedef void TRC_INIT_PROC_t(TRACE_t, void *);
typedef void TRC_PRINT_PROC_t(TRC_USER_DATA_t, const char *);
typedef void TRC_CLOSE_PROC_t(TRACE_t);
typedef void TRC_ABORT_PROC_t(TRACE_t, const char *);

typedef TRC_INIT_PROC_t		*TRC_INIT_PROC_p_t;
typedef TRC_PRINT_PROC_t	*TRC_PRINT_PROC_p_t;
typedef TRC_CLOSE_PROC_t	*TRC_CLOSE_PROC_p_t;
typedef TRC_ABORT_PROC_t	*TRC_ABORT_PROC_p_t;

/* to deactivate the tracelib, comment out this #define */
/* all tracelib macros will then expand to nothing */
#define _TRLIB_

/* public interface for trlib (all macros) */

#ifdef _TRLIB_	/* trlib is enabled */

#define _TRACE(p,x) char *__tT_ = TRC__LogFunc(p,#x, x)
#define TRACE(p,x)					TRC__CString(p,#x)
#define TRACE_INIT(p,x)				TRC__Init(p,x)
#define TRACE_INIT_ID(p,x,i)		TRC__InitID(p,x,i)
#define TRACE_RESET(p)				TRC__Reset(p)
#define TRACE_FREE(p)				TRC__Free(p)
#define TRACE_DUMP(p)				TRC__Dump(p,NULL)
#define TRACE_DUMP_OBJ(p,x)		TRC__Dump(p,x)
#define TRACE_START(p)				TRC__Start(p)
#define TRACE_START_RESET(p)		TRC__StartReset(p)
#define TRACE_CHAR(p,x)				TRC__Char(p,x)
#define TRACE_INTCHAR(p,x)			TRC__SINTCHR(p,x)
#define TRACE_UINTCHAR(p,x)		TRC__UINTCHR(p,x)
#define TRACE_INT(p,x)				TRC__SINT(p,x)
#define TRACE_UINT(p,x)				TRC__UINT(p,x)
#define TRACE_SHORT(p,x)			TRC__SINTSHT(p,x)
#define TRACE_USHORT(p,x)			TRC__UINTSHT(p,x)
#define TRACE_LONG(p,x)				TRC__SINTLNG(p,x)
#define TRACE_ULONG(p,x)			TRC__UINTLNG(p,x)
#define TRACE_ADDRESS(p,x)			TRC__ADDRESS(p,x)
#define TRACE_TIMESTAMP(p)			TRC__TIMESTAMP(p)
/*#define TRACE_ELAPSEDTIME(p)		TRC__ELAPSEDTIME(p)*/
#define TRACE_CSTRING(p,x)			TRC__CString(p,x)
#define TRACE_PasSTRING(p,x)		TRC__PasString(p,x)
#define TRACE_FLOAT(p,x)			TRC__FLOAT(p,x)
#define TRACE_DOUBLE(p,x)			TRC__DOUBLE(p,x)
/*#define TRACE_ARRAY(p,x,t)		TRC__ARRAY(p,x,t)k*/

#define TRACE_VAR_CHAR(p,x)		TRC__VarChar(p,#x,x)
#define TRACE_VAR_INT(p,x)			TRC__VarSINT(p,#x,x)
#define TRACE_VAR_UINT(p,x)		TRC__VarUINT(p,#x,x)
#define TRACE_VAR_INTCHAR(p,x)	TRC__VarSINTCHAR(p,#x,x)
#define TRACE_VAR_UINTCHAR(p,x)	TRC__VarUINTCHAR(p,#x,x)
#define TRACE_VAR_SHORT(p,x)		TRC__VarSINTSHT(p,#x,x)
#define TRACE_VAR_USHORT(p,x)		TRC__VarUINTSHT(p,#x,x)
#define TRACE_VAR_LONG(p,x)		TRC__VarSINTLNG(p,#x,x)
#define TRACE_VAR_ULONG(p,x)		TRC__VarUINTLNG(p,#x,x)
#define TRACE_VAR_ADDRESS(p,x)	TRC__VarADDRESS(p,#x,x)
#define TRACE_VAR_CSTRING(p,x)	TRC__VarCString(p,#x,x)
#define TRACE_VAR_PasSTRING(p,x)	TRC__VarPasString(p,#x,x)
#define TRACE_VAR_FLOAT(p,x)		TRC__VarFLOAT(p,#x,x)
#define TRACE_VAR_DOUBLE(p,x)		TRC__VarDOUBLE(p,#x,x)
/*#define TRACE_VAR_ARRAY(p,x,t)	TRC__VarARRAY(p,#x,x,t)*/

#define TRACE_CSTR_CHAR(p,s,x)		TRC__VarChar(p,s,x)
#define TRACE_CSTR_INT(p,s,x)			TRC__VarSINT(p,s,x)
#define TRACE_CSTR_UINT(p,s,x)		TRC__VarUINT(p,s,x)
#define TRACE_CSTR_INTCHAR(p,s,x)	TRC__VarSINTCHAR(p,s,x)
#define TRACE_CSTR_UINTCHAR(p,s,x)	TRC__VarUINTCHAR(p,s,x)
#define TRACE_CSTR_SHORT(p,s,x)		TRC__VarSINTSHT(p,s,x)
#define TRACE_CSTR_USHORT(p,s,x)		TRC__VarUINTSHT(p,s,x)
#define TRACE_CSTR_LONG(p,s,x)		TRC__VarSINTLNG(p,s,x)
#define TRACE_CSTR_ULONG(p,s,x)		TRC__VarUINTLNG(p,s,x)
#define TRACE_CSTR_ADDRESS(p,s,x)	TRC__VarADDRESS(p,s,x)
#define TRACE_CSTR_CSTRING(p,s,x)	TRC__VarCString(p,s,x)
#define TRACE_CSTR_PasSTRING(p,s,x) TRC__VarPasString(p,s,x)
#define TRACE_CSTR_FLOAT(p,s,x)		TRC__VarFLOAT(p,s,x)
#define TRACE_CSTR_DOUBLE(p,s,x)		TRC__VarDOUBLE(p,s,x)

/* data fields in the TRACE object */
/*#define TRACE_GET_ID(p)					TRC__GetID(p)*/
#define TRACE_GET_VERSION(p)				TRC__GetVersion((p))
#define TRACE_SET_DUMP_ON_FREE(p)		TRC__SetDumpOnFree((p))
#define TRACE_SET_DUMP_ON_START(p)		TRC__SetDumpOnStart((p))
#define TRACE_SET_OUTPUT(p,a,b,c,d)		TRC__SetOutput((p),a,b,c,d)
#define TRACE_SET_STDOUT(p)				TRC__SetOutput((p),\
																		TRC__InitOutput,\
																		TRC__Print,\
																		TRC__CloseOutput,\
																		TRC__AbortOutput)

/* 
** the macros need these functions, but they should not
** be called directly
*/
char * TRC__LogFunc(TRACE_t, const char *name, const void *fxAddr);
void TRC__Init(TRACE_t *, long size);
void TRC__InitID(TRACE_t *, long size, int id);
void TRC__Reset(TRACE_t);
void TRC__Free(TRACE_t *);
void TRC__Dump(TRACE_t, void *obj);
void TRC__Start(TRACE_t);
void TRC__StartReset(TRACE_t);
void TRC__Char(TRACE_t, char val);
void TRC__SINTCHR(TRACE_t, signed char val);
void TRC__UINTCHR(TRACE_t, unsigned char val);
void TRC__SINT(TRACE_t, int val);
void TRC__UINT(TRACE_t, unsigned int val);
void TRC__SINTSHT(TRACE_t, short val);
void TRC__UINTSHT(TRACE_t, unsigned short val);
void TRC__SINTLNG(TRACE_t, long val);
void TRC__UINTLNG(TRACE_t, unsigned long val);
void TRC__ADDRESS(TRACE_t, void *ptr);
void TRC__TIMESTAMP(TRACE_t);
/*void TRC__ELAPSEDTIME(TRACE_t);*/
void TRC__CString(TRACE_t, const char *str);
void TRC__PasString(TRACE_t, const unsigned char *);
void TRC__FLOAT(TRACE_t, float val);
void TRC__DOUBLE(TRACE_t, double val);
/*void TRC__ARRAY(TRACE_t, void *arr, char *type);*/

void TRC__VarChar(TRACE_t, const char *name, char val);
void TRC__VarSINTCHR(TRACE_t tr, const char *name, signed char val);
void TRC__VarUINTCHR(TRACE_t tr, const char *name, unsigned char val);
void TRC__VarSINT(TRACE_t, const char *name, int val);
void TRC__VarUINT(TRACE_t, const char *name, unsigned int val);
void TRC__VarSINTSHT(TRACE_t, const char *name, short val);
void TRC__VarUINTSHT(TRACE_t, const char *name, unsigned short val);
void TRC__VarSINTLNG(TRACE_t, const char *name, long val);
void TRC__VarUINTLNG(TRACE_t, const char *name, unsigned long val);
void TRC__VarADDRESS(TRACE_t, const char *name, void *ptr);
void TRC__VarCString(TRACE_t, const char *name, const char *str);
void TRC__VarPasString(TRACE_t, const char *name, const unsigned char *str);
void TRC__VarFLOAT(TRACE_t, const char *name, float val);
void TRC__VarDOUBLE(TRACE_t, const char *name, double val);
/*void TRC__VarARRAY(TRACE_t, const char *name, void *arr, char *type);*/

void TRC__SetOutput(	TRACE_t,\
							TRC_INIT_PROC_p_t,\
							TRC_PRINT_PROC_p_t,\
							TRC_CLOSE_PROC_p_t,\
							TRC_ABORT_PROC_p_t );

#else		/* trlib is _disabled_ */

#define _TRACE(p,x)	char *__tT_ = NULL	/* this macro cannot be empty */
#define TRACE(p,x)
#define TRACE_INIT(p,x)
#define TRACE_INIT_ID(p,x,i)
#define TRACE_RESET(p)
#define TRACE_FREE(p)
#define TRACE_DUMP(p)
#define TRACE_DUMP_OBJ(p,x)
#define TRACE_START(p)
#define TRACE_CHAR(p,x)
#define TRACE_INTCHAR(p,x)
#define TRACE_UINTCHAR(p,x)
#define TRACE_INT(p,x)
#define TRACE_UINT(p,x)
#define TRACE_SHORT(p,x)
#define TRACE_USHORT(p,x)
#define TRACE_LONG(p,x)
#define TRACE_ULONG(p,x)
#define TRACE_ADDRESS(p,x)
#define TRACE_TIMESTAMP(p)
/*#define TRACE_ELAPSEDTIME(p)*/
#define TRACE_CSTRING(p,x)
#define TRACE_PasSTRING(p,x)
#define TRACE_FLOAT(p,x)
#define TRACE_DOUBLE(p,x)
/*#define TRACE_ARRAY(p,x,t)		TRC__ARRAY(p,x,t)k*/

#define TRACE_VAR_CHAR(p,x)
#define TRACE_VAR_INT(p,x)
#define TRACE_VAR_UINT(p,x)
#define TRACE_VAR_INTCHAR(p,x)
#define TRACE_VAR_UINTCHAR(p,x)
#define TRACE_VAR_SHORT(p,x)
#define TRACE_VAR_USHORT(p,x)
#define TRACE_VAR_LONG(p,x)
#define TRACE_VAR_ULONG(p,x)
#define TRACE_VAR_ADDRESS(p,x)
#define TRACE_VAR_CSTRING(p,x)
#define TRACE_VAR_PasSTRING(p,x)
#define TRACE_VAR_FLOAT(p,x)
#define TRACE_VAR_DOUBLE(p,x)
/*#define TRACE_VAR_ARRAY(p,x,t)*/

#define TRACE_CSTR_CHAR(p,s,x)
#define TRACE_CSTR_INT(p,s,x)
#define TRACE_CSTR_UINT(p,s,x)
#define TRACE_CSTR_INTCHAR(p,s,x)
#define TRACE_CSTR_UINTCHAR(p,s,x)
#define TRACE_CSTR_SHORT(p,s,x)
#define TRACE_CSTR_USHORT(p,s,x)
#define TRACE_CSTR_LONG(p,s,x)
#define TRACE_CSTR_ULONG(p,s,x)
#define TRACE_CSTR_ADDRESS(p,s,x)
#define TRACE_CSTR_CSTRING(p,s,x)
#define TRACE_CSTR_PasSTRING(p,s,x)
#define TRACE_CSTR_FLOAT(p,s,x)
#define TRACE_CSTR_DOUBLE(p,s,x)

/*#define TRACE_GET_ID(p)*/
#define TRACE_GET_VERSION(p)
#define TRACE_SET_DUMP_ON_FREE(p)
#define TRACE_SET_DUMP_ON_START(p)
#define TRACE_SET_OUTPUT(p,a,b,c,d)
#define TRACE_SET_STDOUT(p)

#endif /* _TRLIB_ */

/* typedefs and enums */

/* error constants for TRACE init failure */
/* passed to TRC__AbortInit */
typedef enum {
	INITERR_FIRST = 0,	/* debug value */
	INITERR_POINTER_NOT_NULL,
	INITERR_INVALID_SIZE,
	INITERR_OUT_OF_MEMORY,
	INITERR_LAST			/* debug value */
} INIT_ERR__t;

/* exported functions from trlib.c, not called directly by user */
/* size of user struct */
int TRC__SizeOfUserStruct(void);
/* wrapper function for print function of TRACE output facility */
void TRC__Print(TRACE_t tr, const char *str);

/* implementation dependent functions defined in trconfig.c */
/* not called directly by the user */
void * TRC__MemAlloc(size_t size);
void TRC__MemFree(void *ptr, long size);
void TRC__TrashMemAlloc(void *ptr, size_t size);
void TRC__TrashMemFree(void *ptr, long size);
void TRC__PrintStd(TRC_USER_DATA_t tr, const char *str);
void TRC__InitOutput(TRACE_t, void *obj);
void TRC__CloseOutput(TRACE_t);
void TRC__AbortOutput(TRACE_t, const char *);
time_t TRC__GetTime(void);
void TRC__AbortInit(INIT_ERR__t err);

/* trace object data fields */
char * TRC__GetVersion(TRACE_t tr);
/*int TRC__GetID(TRACE_t tr);*/
TRC_USER_DATA_t TRC__GetUserData(TRACE_t tr);

/* trace flags */
void TRC__SetDumpOnFree(TRACE_t);
void TRC__SetDumpOnStart(TRACE_t);
int  TRC__BufferIsFull(TRACE_t);

#ifdef __cplusplus
}
#endif
#endif /* TRLIB_H */
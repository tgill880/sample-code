/* trlibp.h
**
** private header for the TRACE library (trlib)
**
** Copyright (C) 1999 Thurman Gillespy III
** 1/28/99
*/

#ifndef TRLIBP_H
#define TRLIBP_H

#include <time.h>		/* time_t */
#include "trlib.h"

#ifndef false
#define false	0
#endif

#ifndef true
#define true	(!false)	/* 1 */
#endif

#define kTRACE_VERSION		"DR 4"
#define kTRACE_MAGIC			'TRCE'
#define kMIN_TRACE_BUFFER	512
#define kMAX_TRACE_BUFFER	100000
#define kDEF_TRACE_BUFFER	2048
#define kMAX_ERR_STRLEN		255
#define kMAX_TAG_STRLEN		255
#define kVER_BUF_SIZE		16
/* debug, comment out to remove assertions, debug code */
#define TRLIB_DEBUG

#ifdef TRLIB_DEBUG
/* for an explanation of this assert macro, see Koenig */
#define TRC_ASSERT(x)		(x) || TRC__Assert((#x), __FILE__, __LINE__)
#define kBUFVALID				0xBC	/* debug bytes at front and end of TRACE buffer */
#else
#define TRC_ASSERT(x)
#endif

/* macros */
#define CHAR_SIZE			((long)sizeof(char))
#define SHORT_SIZE		((long)sizeof(short))
#define INT_SIZE			((long)sizeof(int))
#define LONG_SIZE			((long)sizeof(long))
#define PTR_SIZE			((long)sizeof(void *))
#define INT_SIZE			((long)sizeof(int))
#define TIMET_SIZE		((long)sizeof(time_t))
#define FLOAT_SIZE		((long)sizeof(float))
#define DOUBLE_SIZE		((long)sizeof(double))

/* macros for accessing TRACE struct */
#define TRC_GET_MAGIC(p)			(p)->magic
#define TRC_GET_VERSION(p)			&((p)->version)[0]	/* C string */
#define TRC_GET_FLAGS(p)			(p)->flags
#define TRC_GET_ID(p)				(p)->id
#define TRC_GET_SIZE(p)				(p)->size
#define TRC_GET_CUR_POS(p)			(p)->cur_pos
#define TRC_GET_BASE(p)				(p)->base
#define TRC_GET_MARK(p)				(p)->cur_mark
#define TRC_GET_OUTPUT(p)			(p)->out_buf
#define TRC_GET_USER_DATA(p)		(p)->user
#define TRC_GET_BUFFER(p)			(p)->buffer

#define TRC_SET_MAGIC(p,x)			(p)->magic		= (x)
#define TRC_SET_VERSION(p,x)		strcpy(&((p)->version[0]), x)
#define TRC_SET_FLAGS(p,x)			(p)->flags		= (x)
#define TRC_SET_ID(p,x)				(p)->id			= (x)
#define TRC_SET_SIZE(p,x)			(p)->size		= (x)
#define TRC_SET_CUR_POS(p,x)		(p)->cur_pos	= (x)
#define TRC_SET_BASE(p,x)			(p)->base		= (x)
#define TRC_SET_TIMESTAMP(p,x)	(p)->init_time = (x)
#define TRC_SET_MARK(p,x)			(p)->cur_mark	= (x)
#define TRC_SET_OUTPUT(p,x)		(p)->out_buf	= (x)
#define TRC_SET_USER_DATA(p,x)	(p)->user		= (x)
#define TRC_SET_BUFFER(p,x)		(p)->buffer		= (x)

/* TRACE output object */
#define TRC_SET_INITOUT_PROC(p,x) (p)->initout_proc = x
#define TRC_SET_PRINT_PROC(p,x)	(p)->print_proc = x
#define TRC_SET_CLOSE_PROC(p,x)	(p)->close_proc = x
#define TRC_SET_ABORT_PROC(p,x)	(p)->abort_proc = x
#define TRC_INITOUT(p,x)			((p)->initout_proc)(p,x);
#define TRC_PRINT_PROC(p,x)		((p)->print_proc)(TRC_GET_USER_DATA(p),x)
#define TRC_ABORT(p,x)				((p)->abort_proc)(p,x)
#define TRC_CLOSEOUT(p)				((p)->close_proc)(p)

/* memory allocator and free*/
#define TRC_MEM_ALLOC(p,n,t)		(p) = (t)TRC__MemAlloc((size_t)(n))
	
#define TRC_MEM_FREE(p,n) 			TRC__MemFree((void *)(p),(n))

/* increment counter by one, wrap past buffer end if needed */
#define INCR_COUNTER(p, x) 		(x++), (x) %= (p)->size

/* for every counter increment:
**    wrap past buffer end if needed
**    make sure we don't overwrite the base counter
*/
#define INCR_COUNTER_BASECHECK(p, x) \
	{(x++); (x) %= (p)->size; if ((x) == (p)->base) AdvBase(p);}

/* advance offset by num bytes, wrap as needed */
/* do NOT worry about overwriting base */
#define ADV_COUNTER(p, x, num) 		(x) = ((x) + (num)) % (p)->size

/* advance x by num bytes; wrap as needed, check for base counter overwrite */
#define ADV_COUNTER_BASECHECK(p, x, num) \
{int t = num;while (t-- > 0) {(x++); (x) %= (p)->size; if ((x)==(p)->base)AdvBase(p);}}

/* number of bytes to end of TRACE buffer; count _includes_ current position */
#define BYTES_TO_END(pos, size)		((size) - (pos))

/* are there at least "n" bytes to the end of the buffer including pos? */
#define CHAR_BYTES_TO_END(pos, size)		((pos) > (size) - CHAR_SIZE)
#define SHORT_BYTES_TO_END(pos, size)		((pos) > (size) - SHORT_SIZE)
#define LONG_BYTES_TO_END(pos, size)		((pos) > (size) - LONG_SIZE)
#define INT_BYTES_TO_END(pos, size)			((pos) > (size) - INT_SIZE)
#define PTR_BYTES_TO_END(pos, size)			((pos) > (size) - PTR_SIZE)
#define TIMET_BYTES_TO_END(pos, size)		((pos) > (size) - TIMET_SIZE)
#define FLOAT_TO_END(pos, size)				((pos) > (size) - FLOAT_SIZE)
#define DOUBLE_TO_END(pos, size)				((pos) > (size) - DOUBLE_SIZE)

/* size of the entry value, add extra bytes at buffer end if needed */
#define VALUE_SIZE_ENDCHECK(pos,buf_size,val_size) \
	(buf_size) - (pos) < (val_size) ? (val_size) + (buf_size) - (pos) : (val_size)
	
/* read/write macros with TRACE object */
#define TRC_READ_TAG(p,x)			(TRACE_TAG__t)*((p)->buffer + (x))
#define TRC_READ_BYTE(p,x)			(TRC_BYTE_t)*((p)->buffer + (x))
#define PRINT_VALUE(ptr,x,type,q,nl)\
	{\
		unsigned char *p1 = (ptr)->buffer + (x), *p2;\
		int len = sizeof(type);\
		type	temp = (type)0;\
		p2 = (unsigned char *)&temp;\
		while (len-- > 0)\
			*p2++ = *p1++;\
		sprintf(out_buf, "%" #q nl, temp);\
	}
/* *((t *)((p)->buffer + (x))) */

#define TRC_WRITE_TAG(p,x,v)		*((p)->buffer + (x)) = (TRC_BYTE_t)(v)
#define TRC_WRITE_BYTE(p,x,v)		*((p)->buffer + (x)) = (TRC_BYTE_t)(v)
#define TRC_WRITE_VALUE(ptr,x,val,type) \
	{\
		int len = sizeof(type);\
		unsigned char	*p1 = (val),\
							*p2 = (ptr)->buffer + (x);\
		while (len-- > 0)\
			*p2++ = *p1++;\
	}

/* *((t *)((p)->buffer + (x))) = *((t *)(v)) */

/* read/write macros without TRACE object */
#define READ_BYTE(p,x)				*((p) + (x))
#define WRITE_BYTE(p,x,v)			*((p) + (x)) = (TRC_BYTE_t)(v)


/* typedefs */
typedef int					TRC_BOOL_t;
typedef unsigned long	TRC_MAGIC_t;
typedef unsigned long	TRC_FLAGS_t;
typedef long				TRC_COUNT_t;
typedef long				TRC_SIZE_t;
typedef int					TRC_INT_t;
typedef char				TRC_CHAR_t, *TRC_CHAR_p_t;
typedef unsigned char	TRC_BYTE_t;
typedef TRC_BYTE_t		*TRC_BUFFER_p_t;
typedef time_t				TIMESTAMP_t;

/* constants for TRACE tag entries */
/* all entries have high bit (7) set to reduce possibility */
/* of random values mistaken for valid tag */
typedef enum {
	TAG_FIRST = 128,	/* debugging */
	TAG_START,			/* trace base marker */
	TAG_ERR,				/* an error (followed by error byte code) */
	TAG_CHAR,			/* integers */
	TAG_SINTCHR, TAG_UINTCHR,
	TAG_SINTSHT, TAG_UINTSHT,
	TAG_SINT, TAG_UINT, 
	TAG_SINTLNG, TAG_UINTLNG,
	TAG_ADDRESS,		/* memory address */
	TAG_TIMESTAMP,		/* time stamp */
	TAG_FLOAT,			/* floating point */
	TAG_DOUBLE,
	TAG_CSTR,			/* C string (followed by size byte) */
	TAG_PASCSTR,		/* Pascal string */
	TAG_VAR,				/* variable name (C string) */
	TAG_ARRAY,			/* array (followed by type byte, size byte) */
	TAG_LAST				/* debugging */
} TRACE_TAG__t;

/* error constants for ERR TRACE tag */
typedef enum {
	ERR_FIRST = 0,		/* debug */
	ERR_STR_TOO_BIG,	/* C string too large */
	ERR_EMPTY_STR,		/* attempt to write empty string */
	ERR_INVALID_TAG,	/* invalid TRACE tag */
	ERR_BUF_OVERFLOW,	/* base and cur_pos not in synch */
	ERR_LAST				/* debug */
} TRACE_ERR__t;

/* the TRACE object */
typedef struct trace_s {
	TRC_MAGIC_t				magic;		/* kTRACE_MAGIC */
	TRC_CHAR_t				version[kVER_BUF_SIZE];/* version of trlib (C string, 15 bytes max) */
	TRC_INIT_PROC_p_t		initout_proc;	/* initialize TRACE output object proc */
	TRC_PRINT_PROC_p_t	print_proc;		/* print proc */
	TRC_CLOSE_PROC_p_t	close_proc;		/* close TRACE output proc */
	TRC_ABORT_PROC_p_t	abort_proc;		/* abort TRACE output proc */
	TRC_FLAGS_t				flags;		/* status flags */
	TRC_INT_t				id;			/* id code */
	TRC_SIZE_t				size;			/* number of bytes in buffer (does not include debug bytes) */
	TRC_COUNT_t				cur_pos;		/* current buffer position */
	TRC_COUNT_t				cur_mark;	/* position of most recent TRACE_MARK */
	TRC_COUNT_t				base;			/* base position */
	TIMESTAMP_t				init_time;	/* timestamp when initialized */
	TRC_CHAR_t				out_buf[300]; /* output buffer */
	TRC_USER_DATA_t		user;			/* user defined data (defined in trconfig.h) */
	TRC_BUFFER_p_t			buffer;		/* TRACE buffer */
} TRACE__t, *TRACE__p_t;

/* status flags */
enum {
	kIsLockedBit = 0,
	kIsValidBit,
	kBufferFullBit,
	kMustDumpAtFreeBit,
	kMustDumpAtStartBit,
	kIsListBit
};

#define kIsLockedFlag			(unsigned long)(1L << kIsLockedBit)
#define kIsValidFlag				(unsigned long)(1L << kIsValidBit)
#define kBufferFullFlag			(unsigned long)(1L << kBufferFullBit)
#define kDumpAtFreeFlag			(unsigned long)(1L << kMustDumpAtFreeBit)
#define kDumpAtStartFlag		(unsigned long)(1L << kMustDumpAtStartBit)
#define kIsListFlag				(unsigned long)(1L << kIsListBit)

/* flag access macros */
/* return true if bit is set */
#define TRC_GET_IS_LOCKED(p)				(((p)->flags & kIsLockedFlag) != 0)
#define TRC_GET_BUFFER_FULL(p)			(((p)->flags & kBufferFullFlag) != 0)
#define TRC_GET_IS_VALID(p)				(((p)->flags & kIsValidFlag) != 0)
#define TRC_GET_MUSTDUMP_FREE(p)			(((p)->flags & kDumpAtFreeFlag) != 0)
#define TRC_GET_MUSTDUMP_START(p)		(((p)->flags & kDumpAtStartFlag) != 0)
#define TRC_GET_ISLIST(p)					(((p)->flags & kIsListFlag) != 0)

/* if f is true, set bit, else clear */
#define TRC_SET_IS_LOCKED(p,f)		f ? ((p)->flags |= kIsLockedFlag):((p)->flags &= ~kIsLockedFlag)
#define TRC_SET_BUFFER_FULL(p,f)		f ? ((p)->flags |= kBufferFullFlag):((p)->flags &= ~kBufferFullFlag)
#define TRC_SET_IS_VALID(p,f)			f ? ((p)->flags |= kIsValidFlag):((p)->flags &= ~kIsValidFlag)
#define TRC_SET_MUSTDUMP_FREE(p,f)	f ? ((p)->flags |= kDumpAtFreeFlag):((p)->flags &= ~kDumpAtFreeFlag)
#define TRC_SET_MUSTDUMP_START(p,f)	f ? ((p)->flags |= kDumpAtStartFlag):((p)->flags &= ~kDumpAtStartFlag)
#define TRC_SET_ISLIST(p,f)			f ? ((p)->flags |= kIsListFlag):((p)->flags &= ~kIsListFlag)

/* private prototypes for trlib */
void			TRC__SetError(TRACE__p_t tr, TRACE_ERR__t err);
TRC_INT_t	TRC__NextTag(TRACE__p_t tr, long pos);
TRC_BOOL_t	TRC__IsValidTag(TRACE_TAG__t tag);
TRC_BOOL_t	TRC__IsValidTRACE(TRACE__p_t tr);
TRC_BOOL_t	TRC__IsValidBuffer(TRACE__p_t tr);
TRC_BOOL_t	TRC__IsValidErrorCode(TRACE_ERR__t err);
TRC_BOOL_t	TRC__IsValidBaseTag(TRACE__p_t tr);
TRC_BOOL_t	TRC__IsValidCounter(TRACE__p_t tr, TRC_COUNT_t x);
TRC_BOOL_t	TRC__Assert(const char *msg, const char *file, int line);
void 			AdvBase(TRACE__p_t tr);

#endif /* TRLIBP_H */
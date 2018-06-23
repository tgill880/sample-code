/* trlib.c
**
** TRACE core library
**
** Copyright (C) 1999 Thurman Gillespy III
** 1/18/99
*/

#include <stdlib.h>
#include <string.h>		/* memset */
#include "trlibp.h"

/* local static prototypes */
static TRC_BOOL_t IsValidSizeParameter(TRC_SIZE_t size);

/*
** initialize the TRACE structure, buffer and user data
** accepts: pointer to TRACE object (MUST BE NULL)
**          size of TRACE buffer to allocate (-1: use default)
** returns: void
** TRC__AbortInit called if error
**    pointer not NULL
**    wrong size parameter
**    out of memory
*/
void TRC__Init(TRACE__p_t *tr, TRC_SIZE_t size)
{
	TRC_BUFFER_p_t		buf = NULL;
	TRC_USER_DATA_t	usr = NULL;	/* hidden data type */
	
	/* tr must be NULL on entry */
	if (*tr != NULL) {
		TRC__AbortInit(INITERR_POINTER_NOT_NULL);
		exit(0);
	}
	
	/* check for valid size parmeter */
	if ( !IsValidSizeParameter(size)) {
		TRC__AbortInit(INITERR_INVALID_SIZE);
		exit(0);
	}
	
	/* size = -1: use default buffer size */
	if (size < 0)
		size = kDEF_TRACE_BUFFER;
	
	/* allocate memory for the TRACE object */
	TRC_MEM_ALLOC(*tr, sizeof(TRACE__t), TRACE__p_t);
	if (*tr == NULL) {
		TRC__AbortInit(INITERR_OUT_OF_MEMORY);
		exit(0);
	}
	
	/* memory for the TRACE buffer */
#ifdef TRLIB_DEBUG
	size += 4;	/* debug bytes */
#endif /* TRLIB_DEBUG */
	
	TRC_MEM_ALLOC(buf, size, TRC_BUFFER_p_t);
	if (buf == NULL)
		if (*tr == NULL) {
		TRC__AbortInit(INITERR_OUT_OF_MEMORY);
		exit(0);
	}
	
#ifdef TRLIB_DEBUG
	/* first two and last two bytes of buffer set to kBUFVALID */
	buf[0] = buf[1] = buf[size -2] = buf[size - 1] = kBUFVALID;
	/* bugger the buffer pointer and size parameter so the */
	/* debug bytes are only seen by the assert macros */
	TRC_SET_BUFFER(*tr, buf + 2);
	size -= 4;
#else
	TRC_SET_BUFFER(*tr, buf);
#endif /* TRLIB_DEBUG */

	/* user data */
	/* user struct defined in trconfig.h */
	TRC_MEM_ALLOC(usr, TRC__SizeOfUserStruct(), TRC_USER_DATA_t);
	if (usr == NULL) {
		TRC__AbortInit(INITERR_OUT_OF_MEMORY);
		exit(0);
	}
	
	/* initial values - begin with TRACE object in valid state */
	TRC_SET_MAGIC(*tr, kTRACE_MAGIC);
	TRC_SET_VERSION(*tr, kTRACE_VERSION);
	TRC_SET_FLAGS(*tr, 0L);
	TRC_SET_ID(*tr, 0);
	TRC_SET_SIZE(*tr, size);
	TRC_SET_CUR_POS(*tr, 0);
	TRC_SET_MARK(*tr, -1);
	TRC_SET_BASE(*tr, 0);
	TRC_SET_TIMESTAMP(*tr, TRC__GetTime());
	TRC_SET_USER_DATA(*tr, usr);
	/* flags */
	TRC_SET_IS_LOCKED(*tr, false);
	TRC_SET_IS_VALID(*tr, true);
	TRC_SET_BUFFER_FULL(*tr, false);
	TRC_SET_MUSTDUMP_FREE(*tr, false);
	TRC_SET_MUSTDUMP_START(*tr, false);
	/* standard output procs */
	TRC_SET_INITOUT_PROC(*tr, TRC__InitOutput);
	TRC_SET_PRINT_PROC(*tr, TRC__PrintStd);
	TRC_SET_CLOSE_PROC(*tr, TRC__CloseOutput);
	TRC_SET_ABORT_PROC(*tr, TRC__AbortOutput);
	
	/* write the first value to the buffer */
	TRC__Start(*tr);
	
} /* TRC__Init */



/* wrapper function that adds id after tr allocated */
void TRC__InitID(TRACE__p_t *tr, TRC_SIZE_t size, TRC_INT_t id)
{
	TRC__Init(tr, size);
	TRC_SET_ID(*tr, id);
} /* TRC__InitID */

/*
** reset the TRACE struct, restore to initial condition
** does not erase current contents of buffer
*/
void TRC__Reset(TRACE__p_t tr)
{
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	
	TRC_SET_BASE(tr, 0);
	TRC_SET_CUR_POS(tr, 0);
	TRC_SET_MARK(tr, -1);
	
	TRC__Start(tr);
	/* reset the 'buffer wrap' flag */
	TRC_SET_BUFFER_FULL(tr, false);
} /* TRC__Reset */


/*
** free the memory allocated for the TRACE struct
** accepts: address of TRACE object pointer
**          pointer is set to NULL after memory freed
** returns: void
*/
void TRC__Free(TRACE__p_t *tr)
{
	TRC_ASSERT(TRC__IsValidTRACE(*tr));
	TRC_ASSERT(TRC__IsValidBuffer(*tr));
	if (!TRC__IsValidTRACE(*tr))
		return;
	
	/* dump TRACE if TRACE_SET_BUFFER_ON_FREE set */
	if (TRC_GET_MUSTDUMP_FREE(*tr))
		TRC__Dump(*tr, NULL);
	
	/* free the buffer */
#ifdef TRLIB_DEBUG
	TRC_MEM_FREE(TRC_GET_BUFFER(*tr) - 2, TRC_GET_SIZE(*tr) + 4);
#else
	TRC_MEM_FREE(TRC_GET_BUFFER(*tr), TRC_GET_SIZE(*tr));
#endif /* TRLIB_DEBUG
	
	/* free TRACE */
	TRC_MEM_FREE(*tr, sizeof(TRACE__t));
	*tr = NULL;
} /* TRC__Free */


/*
** function logger
** called from _TRACE(tr, foo)
*/
char * TRC__LogFunc(TRACE__p_t tr, const char *name, const void *fxAddr)
{
	#pragma unused(fxAddr)
	
	TRC__CString(tr, name);
	return NULL;
}

/*
** advance the base counter to prevent impending overwrite
** only called within macros
*/
void AdvBase(TRACE__p_t tr)
{
	TRC_INT_t		num_bytes= 0;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
	
	/* number of bytes to next tag */
	num_bytes = TRC__NextTag(tr,  TRC_GET_BASE(tr));
	TRC_ASSERT(num_bytes > 0);
	
	/* advance the base counter */
	ADV_COUNTER(tr, tr->base, num_bytes);
	/* set the 'buffer wrap' flag - we have wrapped past the buffer end */
	TRC_SET_BUFFER_FULL(tr, true);
} /* AdvBase */

/*
** determine the number of bytes to the next TRACE tag
** accepts: pointer to TRACE struct
**          position of current tag
** returns: number of bytes to next tag
*/
TRC_INT_t TRC__NextTag(TRACE__p_t tr, long pos)
{
	TRC_SIZE_t			size		= 0;
	TRC_INT_t			num_bytes= 0;
	TRACE_TAG__t		next_tag 		= TAG_FIRST;	
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return 0;
	TRC_ASSERT(TRC__IsValidCounter(tr, pos));
	
	size = TRC_GET_SIZE(tr);
	
	/* get current tag */
	next_tag = TRC_READ_TAG(tr, pos);
	TRC_ASSERT(TRC__IsValidTag(next_tag));
	/* size of the tag itself */
	num_bytes = 1;
	/* advance the counter */
	INCR_COUNTER(tr, pos);
	
	/* add the size of the entry, allowing for extra bytes */
	/* at the end of the buffer */
	switch (next_tag ) {
		/* 1 byte entries (nothing to add) */
		case TAG_START:
			break;
		/* 2 byte entries */
		case TAG_ERR:
		case TAG_SINTCHR:
		case TAG_UINTCHR:
		case TAG_CHAR:
			num_bytes += 1;
			break;
		/* int - 2 or 4 bytes + tag */
		case TAG_SINT:
		case TAG_UINT:
			/* add size of the value, allowing for extra bytes at end of buffer */
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, INT_SIZE);
			break;
		case TAG_SINTSHT:
		case TAG_UINTSHT:
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, SHORT_SIZE);
			break;
		case TAG_SINTLNG:
		case TAG_UINTLNG:
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, LONG_SIZE);
			break;
		case TAG_ADDRESS:
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, PTR_SIZE);
			break;
		case TAG_TIMESTAMP:
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, TIMET_SIZE);
			break;
		case TAG_FLOAT:
			/* add size of the value, allowing for extra bytes at end of buffer */
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, FLOAT_SIZE);
			break;
		case TAG_DOUBLE:
			/* add size of the value, allowing for extra bytes at end of buffer */
			num_bytes += VALUE_SIZE_ENDCHECK(pos, size, DOUBLE_SIZE);
			break;
		/* variable size tags */
		case TAG_CSTR:	/* C strings are stored in Pascal format */
		case TAG_PASCSTR:
		case TAG_VAR:
			/* size of entry is tag + size byte + number of bytes in string */
			/* pos now points to the size byte */
			num_bytes += TRC_READ_BYTE(tr, pos) + 1;	/* add size byte */
			break;
		case TAG_ARRAY:
			num_bytes += TRC_READ_BYTE(tr, pos) + 2;
			break;
		default:
			num_bytes = 0;
			break;
	} /* switch tag */
	TRC_ASSERT(num_bytes > 0);
	
	return num_bytes;
	
} /* TRC__NextTag */



/*************************************************/
/*    public access functions for TRACE fields   */
/*************************************************/

/* version of the trlib */
TRC_CHAR_p_t TRC__GetVersion(TRACE__p_t tr)
{
	return TRC_GET_VERSION(tr);
} /* TRC__GetVersion */

TRC_USER_DATA_t TRC__GetUserData(TRACE__p_t tr)
{
	return TRC_GET_USER_DATA(tr);
}

/* do TRACE dump when TRACE object freed */
void TRC__SetDumpOnFree(TRACE__p_t tr)
{
	TRC_SET_MUSTDUMP_FREE(tr, true);
} /* TRC__SetDumpOnFree */

/* do TRACE dump next TRACE_START call */
void TRC__SetDumpOnStart(TRACE__p_t tr)
{
	TRC_SET_MUSTDUMP_START(tr, true);
} /* TRC__SetDumpOnFree */

/* has the buffer wrapped? */
TRC_BOOL_t TRC__BufferIsFull(TRACE__p_t tr)
{
	return TRC_GET_BUFFER_FULL(tr);
} /* TRC__BufferIsFull */

/* id of this TRACE object
TRC_INT_t TRC__GetID(TRACE__p_t tr)
{
	return TRC_GET_ID(tr);
}  TRC__GetID */


/**************************************************************/
/*    utility functions for testing validity of TRACE object  */
/**************************************************************/

/* these functions may be called outside of assert macro */

/*
** check for validity of TRACE object
*/
TRC_BOOL_t TRC__IsValidTRACE(TRACE__p_t tr)
{
	return (tr != NULL && TRC_GET_MAGIC(tr) == kTRACE_MAGIC);
} /* TRC__IsValidTRACE */

/* check validity of buffer size parameter when initializing trlib */
static TRC_BOOL_t IsValidSizeParameter(TRC_SIZE_t size)
{
	return ( (size >= kMIN_TRACE_BUFFER && size <= kMAX_TRACE_BUFFER) || size == -1 );
} /* IsValidSizeParameter */


/* these functions are always called within an assert macro */
#ifdef TRLIB_DEBUG

/*
** check validity of TRACE buffer
*/
TRC_BOOL_t TRC__IsValidBuffer(TRACE__p_t tr)
{
	TRC_BUFFER_p_t		buf = NULL;
	TRC_SIZE_t			size = 0;
	
	buf = TRC_GET_BUFFER(tr);
	size = TRC_GET_SIZE(tr);	/* size of original request */
	
	return (buf != NULL && buf[-2] == kBUFVALID && buf[-1] == kBUFVALID &&
				buf[size] == kBUFVALID && buf[size + 1] == kBUFVALID);
} /* TRC__IsValidBuffer */

/*
** valid TRACE tag?
*/
TRC_BOOL_t TRC__IsValidTag(TRACE_TAG__t tag)
{
	return (tag > TAG_FIRST && tag < TAG_LAST);
} /* TRC__IsValidTag */

/*
** valid TRACE error code?
*/
TRC_BOOL_t TRC__IsValidErrorCode(TRACE_ERR__t err)
{
	return (err > ERR_FIRST && err < ERR_LAST);
} /* TRC__IsValidErrorCode */

/*
** does current base counter point to a valid TRACE tag?
*/
TRC_BOOL_t TRC__IsValidBaseTag(TRACE__p_t tr)
{
	return TRC__IsValidTag( (TRACE_TAG__t)*(TRC_GET_BUFFER(tr) + TRC_GET_BASE(tr)) );
} /* TRC__IsValidBaseTag */

/*
** check for valid cur_pos and base counter
** valid range is 0 to size - 1
** note: cur_mark can equal -1
*/
TRC_BOOL_t TRC__IsValidCounter(TRACE__p_t tr, TRC_COUNT_t pos)
{
	return ( pos >= 0 && pos < TRC_GET_SIZE(tr) );
}

#endif /* TRLIB_DEBUG */

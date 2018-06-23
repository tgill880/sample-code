/* trin.c
**
** TRACE input facility
**
** Copyright (C) 1999 Thurman Gillespy III
** 1/28/99
*/

#include <string.h>		/* strlen */
#include "trlibp.h"

/* local static prototypes */
static void SetEntryFixed(TRACE__p_t tr, TRACE_TAG__t tag, void *val);
static void SetEntryString(TRACE__p_t tr, const char *str, TRACE_TAG__t tag);
static TRC_INT_t IsValidFixedTRACEtag(TRACE_TAG__t tag);

/*
** begin a trace path
*/
void TRC__Start(TRACE__p_t tr)
{
	TRC_COUNT_t		cur_pos  = 0;
	TRC_COUNT_t		cur_mark = 0;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	
	/* dump TRACE if TRACE_SET_BUFFER_ON_START set */
	if (TRC_GET_MUSTDUMP_START(tr)) {
		TRC_SET_MUSTDUMP_START(tr, false);
		TRC__Dump(tr, NULL);
	}
	
	cur_pos  = TRC_GET_CUR_POS(tr);
	cur_mark = TRC_GET_MARK(tr);
	
	/* if the last TRACE_TAG_t == TAG_START, do nothing */
	if (cur_mark != -1) {
		INCR_COUNTER_BASECHECK(tr, cur_mark);
		if (cur_mark == cur_pos)
			return;
	}
	
	/* write the tag, increment cur_pos */
	TRC_WRITE_TAG(tr, cur_pos, TAG_START);
	TRC_SET_MARK(tr, cur_pos);		/* reset mark to cur_pos */
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	TRC_SET_CUR_POS(tr, cur_pos);
	
} /* TRC__Start */

/*
** erase the most recent trace path
** reset cur_pos to cur_mark
*/
void TRC__StartReset(TRACE__p_t tr)
{
	TRC_COUNT_t		cur_pos  = 0;
	TRC_COUNT_t		cur_mark = 0;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	
	cur_mark = TRC_GET_MARK(tr);	/* last TAG_START */
	cur_pos = cur_mark;
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	TRC_SET_CUR_POS(tr, cur_pos);

} /* TRC__StartReset */

/*
** write fixed size variable types to TRACE buffer
*/
static void SetEntryFixed(TRACE__p_t tr, TRACE_TAG__t tag, void *val)
{
	TRC_COUNT_t		cur_pos  = 0;
	TRC_COUNT_t		temp		= 0;
	TRC_SIZE_t		size		= 0;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	TRC_ASSERT(IsValidFixedTRACEtag(tag));
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
	
	cur_pos  = TRC_GET_CUR_POS(tr);
	size		= TRC_GET_SIZE(tr);
	
	/* write the TRACE tag, increment cur_pos */
	TRC_WRITE_TAG(tr, cur_pos, tag);
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	
	switch (tag) {
		/* 1 byte values */
		case TAG_CHAR:
			TRC_WRITE_VALUE(tr, cur_pos, val, char);
			INCR_COUNTER_BASECHECK(tr, cur_pos);
			break;
		case TAG_SINTCHR:
			TRC_WRITE_VALUE(tr, cur_pos, val, signed char);
			INCR_COUNTER_BASECHECK(tr, cur_pos);
			break;
		case TAG_UINTCHR:
			TRC_WRITE_VALUE(tr, cur_pos, val, unsigned char);
			INCR_COUNTER_BASECHECK(tr, cur_pos);
			break;
		/* int - 2 or 4 bytes */
		case TAG_SINT:
		case TAG_UINT:
			/* make sure we have at least "int" bytes to buffer end */
			if (INT_BYTES_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			/* temp points to location where value is written */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, INT_SIZE);
			
			TRC_WRITE_VALUE(tr, temp, val, int);
			
			break;
		/* short int */
		case TAG_SINTSHT:
		case TAG_UINTSHT:
			/* make sure we have at least two bytes to buffer end */
			if (SHORT_BYTES_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			/* temp points to location where value is written */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, SHORT_SIZE);
		
			if (tag == TAG_SINTSHT) {
				TRC_WRITE_VALUE(tr, temp, val, short);
			}
			else {
				TRC_WRITE_VALUE(tr, temp, val, unsigned short);
			}
			
			break;
		/* long int */
		case TAG_SINTLNG:
		case TAG_UINTLNG:
			/* make sure we have at least "long" bytes to buffer end */
			if (LONG_BYTES_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			/* temp points to location where value is written */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, LONG_SIZE);
			
			if (tag == TAG_SINTLNG) {
				TRC_WRITE_VALUE(tr, temp, val, long);
			}
			else {
				TRC_WRITE_VALUE(tr, temp, val, unsigned long);
			}
					
			break;
		case TAG_ADDRESS:
			/* make sure we have at least "void *" bytes to buffer end */
			if (PTR_BYTES_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			/* temp points to location where value is written */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, PTR_SIZE);
			
			TRC_WRITE_VALUE(tr, temp, val, void *);
			
			break;
		case TAG_TIMESTAMP:
			/* make sure we have at least "time_t" bytes to buffer end */
			if (TIMET_BYTES_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			/* temp points to location where value is written */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, TIMET_SIZE);
			
			TRC_WRITE_VALUE(tr, temp, val, TIMESTAMP_t);
			
			break;
		case TAG_FLOAT:
			if (FLOAT_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, FLOAT_SIZE);
			
			TRC_WRITE_VALUE(tr, temp, val, float);
			
			break;
		case TAG_DOUBLE:
			if (DOUBLE_TO_END(cur_pos, size))
				ADV_COUNTER_BASECHECK(tr, cur_pos, BYTES_TO_END(cur_pos, size));
			
			/* advance cur_pos before write to prevent base counter overwrite */
			temp = cur_pos;
			ADV_COUNTER_BASECHECK(tr, cur_pos, DOUBLE_SIZE);
			
			TRC_WRITE_VALUE(tr, temp, val, double);
			
			break;
		default:
			break;
	} /* end switch */
	
	/* did we overwrite the base counter? */
	TRC_ASSERT(TRC__IsValidBaseTag(tr)); 
	
	/* update TRACE */
	TRC_SET_CUR_POS(tr, cur_pos);
	
} /* SetEntryFixed */

/* public wrappers for fixed size values */

void TRC__Char(TRACE__p_t tr, char val)
{
	SetEntryFixed(tr, TAG_CHAR, (void *)&val);
}

void TRC__SINT(TRACE__p_t tr, int val)
{
	SetEntryFixed(tr, TAG_SINT, (void *)&val);
}

void TRC__UINT(TRACE__p_t tr, unsigned int val)
{
	SetEntryFixed(tr, TAG_UINT, (void *)&val);
}

void TRC__SINTCHR(TRACE__p_t tr, signed char val)
{
	SetEntryFixed(tr, TAG_SINTCHR, (void *)&val);
}

void TRC__UINTCHR(TRACE__p_t tr, unsigned char val)
{
	SetEntryFixed(tr, TAG_UINTCHR, (void *)&val);
}
	
void TRC__SINTSHT(TRACE__p_t tr, short val)
{
	SetEntryFixed(tr, TAG_SINTSHT, (void *)&val);
}

void TRC__UINTSHT(TRACE__p_t tr, unsigned short val)
{
	SetEntryFixed(tr, TAG_UINTSHT, (void *)&val);
}

void TRC__SINTLNG(TRACE__p_t tr, long val)
{
	SetEntryFixed(tr, TAG_SINTLNG, (void *)&val);
}

void TRC__UINTLNG(TRACE__p_t tr, unsigned long val)
{
	SetEntryFixed(tr, TAG_UINTLNG, (void *)&val);
}

void TRC__ADDRESS(TRACE__p_t tr, void *ptr)
{
	SetEntryFixed(tr, TAG_ADDRESS, ptr);
}

/* treat TIMESTAMP like a Var call */
void TRC__TIMESTAMP(TRACE__p_t tr)
{
	TIMESTAMP_t	time = TRC__GetTime();
	
	SetEntryString(tr, "timestamp", TAG_VAR);
	SetEntryFixed(tr, TAG_TIMESTAMP, (void *)&time);
}

void TRC__FLOAT(TRACE__p_t tr, float val)
{
	SetEntryFixed(tr, TAG_FLOAT, (void *)&val);
}

void TRC__DOUBLE(TRACE__p_t tr, double val)
{
	SetEntryFixed(tr, TAG_DOUBLE, (void *)&val);
}

/* variable versions */

void TRC__VarChar(TRACE__p_t tr, const char *name, char val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_CHAR, (void *)&val);
}

void TRC__VarSINT(TRACE__p_t tr, const char *name, int val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_SINT, (void *)&val);
}

void TRC__VarUINT(TRACE__p_t tr, const char *name, unsigned int val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_UINT, (void *)&val);
}

void TRC__VarSINTCHR(TRACE__p_t tr, const char *name, signed char val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_SINTCHR, (void *)&val);
}

void TRC__VarUINTCHR(TRACE__p_t tr, const char *name, unsigned char val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_UINTCHR, (void *)&val);
}
	
void TRC__VarSINTSHT(TRACE__p_t tr,const  char *name, short val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_SINTSHT, (void *)&val);
}

void TRC__VarUINTSHT(TRACE__p_t tr, const char *name, unsigned short val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_UINTSHT, (void *)&val);
}

void TRC__VarSINTLNG(TRACE__p_t tr, const char *name, long val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_SINTLNG, (void *)&val);
}

void TRC__VarUINTLNG(TRACE__p_t tr, const char *name, unsigned long val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_UINTLNG, (void *)&val);
}

void TRC__VarADDRESS(TRACE__p_t tr, const char *name, void *ptr)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_ADDRESS, ptr);
}

void TRC__VarFLOAT(TRACE__p_t tr, const char *name, float val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_FLOAT, (void *)&val);
}

void TRC__VarDOUBLE(TRACE__p_t tr, const char *name, double val)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryFixed(tr, TAG_DOUBLE, (void *)&val);
}

/*
** write C or Pascal string str to the TRACE buffer
** string actually stored in Pasal string format
** string truncated to 255 bytes if needed
*/
static void SetEntryString(TRACE__p_t tr, const char *str, TRACE_TAG__t tag)
{
	TRC_BUFFER_p_t	buf		= NULL;
	TRC_COUNT_t		cur_pos  = 0;
	TRC_COUNT_t		base		= 0;
	TRC_INT_t		len		= 0, i;
	TRC_BOOL_t		truncate_flag = false;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
	
	cur_pos  = TRC_GET_CUR_POS(tr);
	base		= TRC_GET_BASE(tr);
	buf		= TRC_GET_BUFFER(tr);
	
	/* if the string is empty, write an error code, then exit */
	if (str[0] == '\0') {
		TRC__SetError(tr, ERR_EMPTY_STR);
		return;
	}
	
	/* write the string tag, advance cur_pos */
	TRC_WRITE_TAG(tr, cur_pos, tag);
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	
	/* check length */
	if (tag == TAG_CSTR || tag == TAG_VAR) {
		len = (long)strlen(str);
		if (len > kMAX_TAG_STRLEN - 1) {
			/* truncate C string */
			len = kMAX_TAG_STRLEN;
			truncate_flag = true;
		} /* end if */
	} /* end if */
	else {
		len = (TRC_BYTE_t)str[0];
		str++;
	}
		
	/* copy the string to the TRACE buffer */
	/* write the length byte */
	WRITE_BYTE(buf, cur_pos, len);
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	/* write the rest of the string */
	for (i = 0; i < len; i++) {
		WRITE_BYTE(buf, cur_pos, *str++);
		INCR_COUNTER_BASECHECK(tr, cur_pos);
	}
	
	/* update the cur_pos in TRACE */
	TRC_SET_CUR_POS(tr, cur_pos);
	
	if (truncate_flag)
		TRC__SetError(tr, ERR_STR_TOO_BIG);
	
	/* make sure we didn't overwrite the base counter */
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
	
} /* SetEntryString */


void TRC__CString(TRACE__p_t tr, const char *str)
{
	SetEntryString(tr, str, TAG_CSTR);
} /* TRC__CString */


/*
** write Pascal string str to the TRACE buffer
*/
void TRC__PasString(TRACE__p_t tr, const unsigned char *str)
{
	SetEntryString(tr, (const char *)str, TAG_PASCSTR);
} /* TRC__PasString */

/* variable versions */
void TRC__VarCString(TRACE__p_t tr, const char *name, const char *str)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryString(tr, str, TAG_CSTR);
} /* TRC__VarCString */

void TRC__VarPasString(TRACE__p_t tr, const char *name, const unsigned char *str)
{
	SetEntryString(tr, name, TAG_VAR);
	SetEntryString(tr, (char *)str, TAG_PASCSTR);
} /* TRC__VarPasString */

/*
** set the TRACE error tag and code
*/
void TRC__SetError(TRACE__p_t tr, TRACE_ERR__t err)
{
	TRC_COUNT_t		cur_pos  = NULL;
	TRC_COUNT_t		base		= NULL;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
	TRC_ASSERT(TRC__IsValidErrorCode(err));
	
	cur_pos  = TRC_GET_CUR_POS(tr);
	
	/* write the error tag, increment cur_pos */
	TRC_WRITE_TAG(tr, cur_pos, TAG_ERR);
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	/* write the error code, increment cur_pos */
	TRC_WRITE_BYTE(tr, cur_pos, err);
	INCR_COUNTER_BASECHECK(tr, cur_pos);
	/* update TRACE */
	TRC_SET_CUR_POS(tr, cur_pos);
	
	TRC_ASSERT(TRC__IsValidBaseTag(tr));
} /* SetEntryErr */

/*
** valid fixed size TRACE tag?
*/
static TRC_BOOL_t IsValidFixedTRACEtag(TRACE_TAG__t int_tag)
{
	return (int_tag >= TAG_CHAR && int_tag <= TAG_DOUBLE);
}
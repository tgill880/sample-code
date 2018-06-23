/* trout.c
**
** TRACE output facility
**
** Copyright (C) 1999 Thurman Gillespy III
** 1/17/99
*/

#include "trlibp.h"
#include <stdio.h>
#include <string.h> /* strcpy in ErrToString */

/* string constants for error messages */
#define kSTRING_TOO_BIG		"C string too large - truncated to 255 charaters."
#define kEMPTY_STRING		"Empty string."
#define kINVALID_TAG			"Invalid TRACE tag."
#define kBUFFER_OVERFLOW	"Buffer overflow."

/* local static prototypes */
static TRC_BOOL_t DumpEntry(TRACE__p_t tr, TRC_COUNT_t base);
static TRC_INT_t ErrToString(char *str, TRACE_ERR__t err);


/*
** dump the TRACE buffer
** start at base, continue to cur_pos
** dat is optional user data
*/
void TRC__Dump(TRACE__p_t tr, void *dat)
{
	TRC_COUNT_t		base		= 0;
	TRC_COUNT_t		cur_pos  = 0;
	TRC_SIZE_t		size		= 0;
	TRC_INT_t		num_bytes = 0,
						total_bytes = 0,
						count		= 0;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return;
	
	cur_pos  = TRC_GET_CUR_POS(tr);
	base		= TRC_GET_BASE(tr);
	size		= TRC_GET_SIZE(tr);
	
	/* call the init out proc in tr */
	TRC_INITOUT((TRACE_t)tr, dat);	/* user data can be set here */
	
	count = 0, total_bytes = 0;
	while (base != cur_pos) {
		/* write the current tag entry to the output buffer */
		/* abort if error */
		if (!DumpEntry(tr, base))
			break;
		/* print the string in the output buffer */
		TRC__Print(tr, TRC_GET_OUTPUT(tr));
		
		/* advance to next tag */
		num_bytes = TRC__NextTag(tr, base);
		ADV_COUNTER(tr, base, num_bytes);
		/* make sure we're not in an endless loop */
		count++, total_bytes += num_bytes;
		if (count > size || total_bytes > size) {
			TRC_ABORT(tr, "endless buffer loop");
			break;
		}
	} /* end while */
	
	/* call the close output proc */
	TRC_CLOSEOUT(tr);
	
} /* TRC__Dump */

/*
** return the entry at position 'base' in the TRACE buffer
** entry string is copied to the string buffer in 'out'
*/
static TRC_BOOL_t DumpEntry(TRACE__p_t tr, TRC_COUNT_t base)
{
	TRC_SIZE_t		size	= 0;
	TRACE_TAG__t	tag	= TAG_FIRST;
	char				*out_buf = NULL;
	
	TRC_ASSERT(TRC__IsValidTRACE(tr));
	TRC_ASSERT(TRC__IsValidBuffer(tr));
	if (!TRC__IsValidTRACE(tr))
		return false;
	
	size		= TRC_GET_SIZE(tr);
	out_buf	= TRC_GET_OUTPUT(tr);
	TRC_ASSERT(out_buf != NULL);
	
	/* get the current tag */
	tag = TRC_READ_TAG(tr, base);
	TRC_ASSERT(TRC__IsValidTag(tag));
	if (!TRC__IsValidTag(tag)) {
		TRC_ABORT(tr, "invalid tag");
		return false;
	}
	
	/* increment to next byte after tag */
	INCR_COUNTER(tr, base);
	
	/* handle fixed values > 1 byte too close to buffer end */
	/* by advancing base to start of buffer */
	switch (tag) {
		/* short int */
		case TAG_SINTSHT:
		case TAG_UINTSHT:
			if (SHORT_BYTES_TO_END(base, size))
				base =0;
			break;
		/* int */
		case TAG_SINT:
		case TAG_UINT:
			if (INT_BYTES_TO_END(base, size))
				base = 0;
			break;
		/* long int */
		case TAG_SINTLNG:
		case TAG_UINTLNG:
			if (LONG_BYTES_TO_END(base, size))
				base = 0;
			break;
		case TAG_ADDRESS:
			if (PTR_BYTES_TO_END(base, size))
				base = 0;
			break;
		case TAG_TIMESTAMP:
			if (TIMET_BYTES_TO_END(base, size))
				base = 0;
			break;
		case TAG_FLOAT:
			if (FLOAT_TO_END(base, size))
				base = 0;
			break;
		case TAG_DOUBLE:
			if (DOUBLE_TO_END(base, size))
				base = 0;
			break;
	} /* end switch */
	
	/* print the value or message associated with this tag */
	switch (tag ) {
		case TAG_START:
			strcpy(out_buf, NEWLINE);	/* strcpy, not strcat! */
			strcat(out_buf, "Begin TRACE path.");
			strcat(out_buf, NEWLINE);
			break;
		case TAG_ERR: {
			char	str_err[kMAX_ERR_STRLEN + 1];
			TRACE_ERR__t	err;
			err = (TRACE_ERR__t)TRC_READ_BYTE(tr, base);
			ErrToString(str_err, err);
			strcpy(out_buf, "Error: ");
			strcat(out_buf, str_err);
			strcat(out_buf, NEWLINE);
			break;
			}
		case TAG_CHAR:
			PRINT_VALUE(tr, base, char, c, NEWLINE);
			break;
		case TAG_SINT:
			PRINT_VALUE(tr, base, int, d, NEWLINE);
			break;
		case TAG_UINT:
			PRINT_VALUE(tr, base, unsigned int, u, NEWLINE);
			break;
		case TAG_SINTCHR:
			PRINT_VALUE(tr, base, signed char, hd, NEWLINE);
			break;
		case TAG_UINTCHR:
			PRINT_VALUE(tr, base, unsigned char, hu, NEWLINE);
			break;
		case TAG_SINTSHT:
			PRINT_VALUE(tr, base, short, hd, NEWLINE);
			break;
		case TAG_UINTSHT:
			PRINT_VALUE(tr, base, unsigned short, hu, NEWLINE);
			break;
		case TAG_SINTLNG:
			PRINT_VALUE(tr, base, long, ld, NEWLINE);
			break;
		case TAG_UINTLNG:
			PRINT_VALUE(tr, base, unsigned long, lu, NEWLINE);
			break;
		case TAG_ADDRESS:
			PRINT_VALUE(tr, base, void *, p, NEWLINE);
			break;
		case TAG_TIMESTAMP:
			PRINT_VALUE(tr, base, time_t, lu, NEWLINE);
			break;
		case TAG_FLOAT:
			PRINT_VALUE(tr, base, float, f, NEWLINE);
			break;
		case TAG_DOUBLE:
			PRINT_VALUE(tr, base, double, lf, NEWLINE);
			break;
		case TAG_CSTR:
		case TAG_PASCSTR:
		case TAG_VAR: {
			char			str_buf[256];
			TRC_INT_t	len, i;
			/* copy the Pascal string to str_buf, convert to C string */
			len = TRC_READ_BYTE(tr, base);
			INCR_COUNTER(tr, base);
			for (i = 0; i < len; i++) {
				str_buf[i] = (char)TRC_READ_BYTE(tr, base);
				INCR_COUNTER(tr, base);
			}
			/* write the null terminator */
			str_buf[i] = '\0';
			/* print the string */
			strcpy(out_buf, str_buf);
			/* print CR/LF for C, Pascal strings, but not TAG_VAR */
			if (tag == TAG_VAR)
				strcat(out_buf, ": ");
			else
				strcat(out_buf, NEWLINE);
			break;
		}
		case TAG_ARRAY:
			break;
		default:
			break;
	} /* switch tag */
	
	return true;
	
} /* DumpEntry */


/*
** print the string in str
** general print call that selects proper print proc
*/
void TRC__Print(TRACE_t tr, const char *str)
{

	TRC_PRINT_PROC(tr, str); /* trlibp.h */
	
} /* TRC__Print */


/*
** convert a TRACE error code to a string message
** accepts: TRACE error code
**          string buffer (kMAX_ERR_STRLEN in size)
**	returns: 0: no error
**         -1: invalid error code
**          error message in the string buffer (empty string if error)
*/
static TRC_INT_t ErrToString(char *str, TRACE_ERR__t err)
{
	TRC_ASSERT(TRC__IsValidErrorCode(err));
	switch (err) {
		case ERR_STR_TOO_BIG:
			strcpy(str, kSTRING_TOO_BIG);
			break;
		case ERR_EMPTY_STR:
			strcpy(str, kEMPTY_STRING);
			break;
		case ERR_INVALID_TAG:
			strcpy(str, kINVALID_TAG);
			break;
		case ERR_BUF_OVERFLOW:
			strcpy(str, kBUFFER_OVERFLOW);
			break;
		default:
			strcpy(str, "Invalid error code!");
			return -1;
			break;
	}
	
	return 0;
} /* ErrToString */


/*
** set the core routines for the trlib output facility
** allows ability to override default behaviour
*/
void TRC__SetOutput(TRACE_t tr, TRC_INIT_PROC_p_t initproc, TRC_PRINT_PROC_p_t printproc,
							TRC_CLOSE_PROC_p_t closeproc, TRC_ABORT_PROC_p_t abortproc)
{
	TRC_SET_INITOUT_PROC((TRACE__p_t)tr, initproc);
	TRC_SET_PRINT_PROC((TRACE__p_t)tr, printproc);
	TRC_SET_CLOSE_PROC((TRACE__p_t)tr, closeproc);
	TRC_SET_ABORT_PROC((TRACE__p_t)tr, abortproc);

}  /* TRC__SetOutput */


/* trdebug.c
**
** debugging utilities for the TRACE library (trlib)
**
** Standard C version
**
** Copyright (C) 1998 Thurman Gillespy III
** 1/8/99
*/

#include "trlibp.h"
#include <stdio.h>

#ifdef TRLIB_DEBUG
/* assertion support */
int TRC__Assert(const char *msg, const char *file, int line)
{
	
	printf(NEWLINE "trlib assertion failure!" NEWLINE);
	printf("failure:\t%s" NEWLINE, msg);
	printf("file:\t\t%s" NEWLINE, file);
	printf("line:\t\t%d" NEWLINE, line);
	exit(0);
	
	return 1;	/* needed for assertion macro (see Koenig) */
} /* TRC__Assert */

#endif /* TRLIB_DEBUG */
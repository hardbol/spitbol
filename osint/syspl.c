/*
Copyright 1987-2012 Robert B. K. Dewar and Mark Emmer.

This file is part of Macro SPITBOL.

    Macro SPITBOL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Macro SPITBOL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Macro SPITBOL.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
/	File:  SYSPL.C		Version:  01.01
/	---------------------------------------
/
/	zyspl - interface polling from SPITBOL
/
/	zyspl is called before statement execution to allow the interface
/         to regain control if desired.
/	Parameters:
/	    WA - reason for call
/		     =0  periodic polling
/		     =1  breakpoint hit
/		     =2  completion of statement stepping
/     WB - current statement number
/           XL - SCBLK of result if WA = 3.
/	Normal Return
/	    WA - number of statements to elapse before calling SYSPL again.
/	Exits:
/	    1 - set breakpoint
/	    2 - single step
/	    3 - evaluate expression
/       normal exit - no special action
/
/  Version history:
/
/
*/

#include "port.h"

#if POLLING & (LINUX | WINNT)
#if WINNT
int	pollevent Params((void));
#endif
#if LINUX
#define pollevent()
#endif          /* LINUX */
extern  rearmbrk Params((void));
extern	int	brkpnd;
#define stmtDelay PollCount
#endif


zyspl()
{
#if POLLING

    /* Make simple polling case the fastest by avoiding switch statement */
    if (WA(word) == 0) {
#if !ENGINE
        pollevent();
#endif					/* !ENGINE */
        SET_WA(stmtDelay);	/* Poll finished or Continue */
#if !ENGINE & (WINNT | LINUX)
        if (brkpnd) {
            brkpnd = 0;		/* User interrupt */
            rearmbrk();		/* allow breaks again */
            return EXIT_1;
        }
#endif
    }
#else					/* POLLING */
    SET_WA((word)MAXPOSWORD);			/* Effectively shut off polling */
#endif					/* POLLING */
    return NORMAL_RETURN;
}


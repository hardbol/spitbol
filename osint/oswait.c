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
/   File:  OSWAIT.C     Version:  01.02
/	---------------------------------------
/
/	Contents:	Function oswait
*/


/*
/   oswait( pid )
/
/   oswait() waits for the termination of the process with id pid.
/
/   Parameters:
/	pid	prcoess id
/   Returns:
/   nothing
/
/   V1.01 MBE 07-29-91  <withdrawn>.
/   V1.02 MBE 12-31-96  Modify for WinNT.
/
*/

#include "port.h"
#if PIPES

#if LINUX
#include <signal.h>
#endif                  /* LINUX */

#if WINNT
#include <process.h>
#if _MSC_VER
extern int wait(int *status);
#endif
#endif

void oswait( pid )
int	pid;
{
    int	deadpid, status;
    struct  chfcb   *chptr;
#if LINUX
    SigType (*hstat)Params((int)),
            (*istat)Params((int)),
            (*qstat)Params((int));

    istat	= signal( SIGINT, SIG_IGN );
    qstat	= signal( SIGQUIT ,SIG_IGN );
    hstat	= signal( SIGHUP, SIG_IGN );
#endif

    while ( (deadpid = wait( &status )) != pid  &&  deadpid != -1 )
    {
        for ( chptr = GET_MIN_VALUE(R_FCB,struct chfcb *); chptr != 0;
                chptr = MK_MP(chptr->nxt, struct chfcb *) )
        {
            if ( deadpid == MK_MP(MK_MP(chptr->fcp, struct fcblk *)->iob,
                                  struct ioblk *)->pid )
            {
                MK_MP(MK_MP(chptr->fcp, struct fcblk *)->iob,
                      struct ioblk *)->flg2 |= IO_DED;
                break;
            }
        }
    }

#if UNIX
    signal( SIGINT,istat );
    signal( SIGQUIT,qstat );
    signal( SIGHUP,hstat );
#endif                  /* LINUX */
}
#endif					/* PIPES */

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
/	File:  RDENV.C		Version:  01.02
/	---------------------------------------
/
/	Contents:	Function rdenv
*/

/*
/   rdenv( varname, result )
/
/   rdenv() reads the environment variable named "varname", and if it can
/   be read, puts its value in "result.
/
/   Parameters:
/	varname	pointer to character string containing variable name
/	result	pointer to character string to receive result
/   Returns:
/	0 if successful / -1 on failure
/
/	v1.02 02-Jan-91 Changed rdenv to use cpys2sc instead of mystrncpy.
/					Add private getenv().
*/

#include "port.h"

/*
/   Find environment variable vq of length vn.  Return
/   pointer to value (just past '='), or 0 if not found.
*/
char *findenv( vq, vn )
char *vq;
int  vn;
{
#if WINNT | LINUX
    char savech;
    char *p;

    savech = make_c_str(&vq[vn]);
    p = (char *)getenv(vq);			/* use library lookup routine */
    unmake_c_str(&vq[vn], savech);
    return p;
#endif

}

rdenv( varname, result )
register struct scblk *varname, *result;
{
    register char *p;


    if ( (p = findenv(varname->str, varname->len)) == 0 )
        return -1;

    cpys2sc(p, result, TSCBLK_LENGTH);

    return 0;
}

/* make a string into a C string by changing the last character to null,
 * returning the old character at that location.
 * If the old character was already null, no change is made, so that
 * this works if passed a read-only C-string.
 */
char make_c_str(p)
char *p;
{
    char rtn;

    rtn = *p;
    if (rtn)
        *p = 0;
    return rtn;
}


/* Intel compiler bug? */
void unmake_c_str(p, savech)
char *p;
char savech;
{
    if (savech)
        *p = savech;
}

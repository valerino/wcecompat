/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

char* messages[] = {
/*0           */    "No error",
/*EPERM       */    "Operation not permitted",
/*ENOENT      */    "No such file or directory",
/*ESRCH       */    "No such process",
/*EINTR       */    "Interrupted system call",
/*EIO         */    "I/O error",
/*ENXIO       */    "No such device or address",
/*E2BIG       */    "Arg list too long",
/*ENOEXEC     */    "Exec format error",
/*EBADF       */    "Bad file descriptor",
/*ECHILD      */    "No child processes",
/*EAGAIN      */    "Resource temporarily unavailable",
/*ENOMEM      */    "Not enough memory",
/*EACCES      */    "Permission denied",
/*EFAULT      */    "Bad address",
/*15          */    "Unknown error",						// ENOTBLK "Block device required"
/*EBUSY       */    "Device or resource busy",
/*EEXIST      */    "File exists",
/*EXDEV       */    "Improper link",						//         "Cross-device link"
/*ENODEV      */    "No such device",
/*ENOTDIR     */    "Not a directory",
/*EISDIR      */    "Is a directory",
/*EINVAL      */    "Invalid argument",
/*ENFILE      */    "Too many open files in system",
/*EMFILE      */    "Too many open files",
/*ENOTTY      */    "Inappropriate I/O control operation",	//         "Not a character device"
/*26          */    "Unknown error",						// ETXTBSY "Text file busy"
/*EFBIG       */    "File too large",
/*ENOSPC      */    "No space left on device",
/*ESPIPE      */    "Invalid seek",							//         "Illegal seek"
/*EROFS       */    "Read-only file system",
/*EMLINK      */    "Too many links",
/*EPIPE       */    "Broken pipe",
/*EDOM        */    "Domain error",							//         "Math arg out of domain of func"
/*ERANGE      */    "Result too large",						//         "Math result out of range"
/*35          */    "Unknown error",						// ENOMSG  "No message of desired type"
/*EDEADLK     */    "Resource deadlock avoided",			// EIDRM   "Identifier removed"
/*37          */    "Unknown error",						// ECHRNG  "Channel number out of range"
/*ENAMETOOLONG*/    "Filename too long",
/*ENOLCK      */    "No locks available",
/*ENOSYS      */    "Function not implemented",
/*ENOTEMPTY   */    "Directory not empty",
/*EILSEQ      */    "Illegal byte sequence"
};
const int NUM_MESSAGES = sizeof(messages)/sizeof(messages[0]);

int vasprintf (char **strp, const char *fmt, va_list ap)
{
     /* HACK: vsnprintf in the WinCE API behaves like
      * the one in glibc 2.0 and doesn't return the number of characters
      * it needed to copy the string.
      * cf http://msdn.microsoft.com/en-us/library/1kt27hek.aspx
      * and cf the man page of vsnprintf
      *
      Guess we need no more than 50 bytes. */
     int n, size = 50;
     char *res, *np;
 
     if ((res = (char *) malloc (size)) == NULL)
         return -1;
 
     while (1)
     {
         n = _vsnprintf (res, size, fmt, ap);
 
         /* If that worked, return the string. */
         if (n > -1 && n < size)
         {
             *strp = res;
             return n;
         }
 
         /* Else try again with more space. */
         size *= 2;  /* twice the old size */
 
         if ((np = (char *) realloc (res, size)) == NULL)
         {
             free(res);
             return -1;
         }
         else
         {
             res = np;
         }
 
     }
}

char* strerror(int errnum)
{
	if (errnum < NUM_MESSAGES)
		return messages[errnum];
	return "Unknown error";
}

char *strdup(const char *Src)
{
	char* buffer;
	buffer = (char*)malloc(strlen(Src)+1);
	if(buffer)
		strcpy(buffer, Src);
	return buffer;
}

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t strlcat(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;

	if (n == 0)
		return(strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(s - src);	/* count does not include NUL */
}

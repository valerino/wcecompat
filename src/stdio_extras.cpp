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


#include <stdio.h>
#include <winsock2.h>
#include <errno.h>
#include <ceconfig.h>

FILE *tmpfile( void )
{
  WCHAR path [MAX_PATH];
  WCHAR wtmpfile [MAX_PATH];
  char atmpfile [MAX_PATH];
  FILE* f;

  GetTempPath(MAX_PATH,path);
  if (GetTempFileNameW(path,L"tmp",0,wtmpfile) == 0)
    return NULL;
  wcstombs(atmpfile,wtmpfile,MAX_PATH);
  f = fopen (atmpfile,"w+b");
  return f;
}

void rewind (FILE* file)
{
  fseek (file,0,SEEK_SET);
}

void perror(const char *prefix)
{
	if (prefix == NULL || *prefix == 0)
		fprintf(stderr, "errno=%d\n", errno);
	else
		fprintf(stderr, "%s: errno=%d\n", prefix, errno);
}

#if _WIN32_WCE < 0x500 || !defined(COREDLL_CORESTRA)
// int setvbuf(FILE* /*stream*/, char* /*buffer*/, int /*mode*/, size_t /*size*/)
// {
// 	// TODO: implement buffering
// 	return 0;
// }
#endif
int setbuf (FILE* stream, char* buffer)
{
	/// unimplemented, use vbuf. just for compatibility.
	return 0;
}


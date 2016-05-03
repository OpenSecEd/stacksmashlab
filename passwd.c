/*
 * Copyright (c) 2014, Daniel Bosk <daniel.bosk@miun.se>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  - Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef _GNU_SOURCE
#include <crypt.h>
#endif

int
test_passwd( const char *passwd )
{
	char pwd[256];
	size_t len = 0;
	FILE *file;
	
	/* use setuid(2) here to acquire priviledges */
	file = fopen( "master.passwd", "r" );
	
	if ( file == NULL )
		err( -1, "Could not check password" );

	len = fread( pwd, 1, 256, file );
	if ( len < 1 )
		return 0;
	pwd[len] = 0;

	if ( ! strcmp( pwd, crypt( passwd, "aa" ) ) )
		return 0;

	fclose( file );
	/* use setuid(2) to restore priviledges */

	return -1;
}

int
write_passwd( const char *passwd )
{
	char *pwdstr = crypt( passwd, "aa" );
	size_t len;
	FILE *file;
	
	/* use setuid(2) here to acquire priviledges */
	file = fopen( "master.passwd", "w" );

	if ( file == NULL )
		err( -1, "Could not write password" );

	len = fwrite( pwdstr, strlen( pwdstr ), 1, file );
	if ( len < 1 )
		err( -1, "Could not write password" );

	fclose( file );
	/* use setuid(2) to restore priviledges */

	return 0;
}

int
main( void )
{
	char passwd[128] = { 0 };
	char newpass[128] = { 0 };

	printf( "Enter old password: " );
	scanf( "%s", passwd );

	if ( test_passwd( passwd ) < 0 ) {
		//errx( -1, "Old password is wrong" );
		printf( "Old password is wrong" );
		return -1;
	}

	printf( "Enter new password: " );
	scanf( "%s", passwd );

	printf( "Reenter new password: " );
	scanf( "%s", newpass );

	if ( strcmp( passwd, newpass ) ) {
		errx( -1, "Passwords do not match" );
	}
	else if ( write_passwd( passwd ) < 0 ) {
		err( -1, "Could not write password" );
	}

	return 0;
}

/*
 * Copyright 2003 
 *	Richard Welty <rwelty@averillpark.net> ("The Author").
 *      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Richard Welty.
 * 4. The Author's name may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' BY THE AUTHOR AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define boolean int

main( argc, argv)
     int argc;
     char * argv[];
{
  boolean start_of_line = FALSE;
  boolean in_header = TRUE;

  int c;

  char base_command[]
  = "/usr/bin/gpg --armor --batch --no-tty --no-secmem-warning --encrypt --recipient ";
  char *command;

  if( argc != 2){
    fprintf( stderr, "Missing or extraneous arguments\n");
    fprintf( stderr, "Sole argument is email address of recipient\n");
    exit( 1);
  }

  while( in_header && ((c = getchar()) != EOF)){
    /* in all cases we need to write the character */
    putchar( c);
    if( c == '\n'){
      if( start_of_line){
	/* header separates from body here */
	in_header = FALSE;
      } else {
        /* just another line in the header */
	start_of_line = TRUE;
      }
    } else {
      start_of_line = FALSE;
    }
  }
  fflush( stdout);

  if( c == EOF){
    /* no body, don't encrypt */
    printf( "WARNING: Message received with no body, cannot encrypt\n");
    printf( "Please contact sender\n");
    exit( 0); 
  } else {
    command = (char *) calloc( strlen( base_command) + strlen( argv[1]) + 1,
			       sizeof( char));
    command = strcpy( command, base_command);
    command = strcat( command, argv[1]);
    exit( system( command));
  }
}

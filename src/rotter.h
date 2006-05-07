/*

	rotter.h
	Recording of Transmission / Audio Logger
	Copyright (C) 2006  Nicholas J. Humfrey
	
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#include <jack/jack.h>
#include <jack/ringbuffer.h>


#ifndef _ROTTER_H_
#define _ROTTER_H_


// ------- Constants -------
#define DEFAULT_RB_LEN			(0.5)
#define WRITE_BUFFER_SIZE		(2048)
#define MAX_FILEPATH_LEN		(1024)
#define DEFAULT_DIR_MODE		(0755)
#define DEFAULT_CLIENT_NAME		"rotter"
#define DEFAULT_BITRATE			(160)
#define DEFAULT_CHANNELS		(2)


// ------- Globals ---------
extern jack_port_t *inport[2];
extern jack_ringbuffer_t *ringbuffer[2];
extern jack_client_t *client;


// ------- Logging ---------

typedef enum {
	ROTTER_DEBUG=1,		// Only display debug if verbose
	ROTTER_INFO,		// Don't show info when quiet
	ROTTER_ERROR,		// Always display warnings
	ROTTER_FATAL		// Quit if fatal
} RotterLogLevel;


#define rotter_debug( args... ) \
		rotter_log( ROTTER_DEBUG, args )

#define rotter_info( args... ) \
		rotter_log( ROTTER_INFO, args )

#define rotter_error( args... ) \
		rotter_log( ROTTER_ERROR, args )

#define rotter_fatal( args... ) \
		rotter_log( ROTTER_FATAL, args )



// ------- Encoder Callbacks -------

typedef struct encoder_funcs_s
{
	const char* file_suffix;		// Suffix for archive files

	int (*open)(char * filepath);	// Result: 0=success
	int (*close)();					// Result: 0=success
		
	int (*encode)();				// Result: negative=error
									//		   0= try again later
									//         positive=bytes written
	void (*shutdown)();

} encoder_funcs_t;



// ------- ID3v1.0 Structure -------

typedef struct id3v1_s
{
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	char genre;
} id3v1_t;


// In rotter.c
void write_id3v1( FILE *file );
void rotter_log( RotterLogLevel level, const char* fmt, ... );

// In twolame.c
encoder_funcs_t* init_twolame( int channels, int bitrate );



#endif

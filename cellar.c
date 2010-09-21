/*
 * vim: ts=8 sw=8
 */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <gcc-compat.h>

#include <time_utils.h>

#define	EXEC_GOOD			1
#define	EXEC_BAD			2

#define	OPTION	5

static inline unsigned		_inline _pure
timestamp_size(
	unsigned const			choice
)
{
	static unsigned const	choices[] =	{
		[1]	=	10,
		[2]	=	8,
		[3]	=	6,
		[5]	=	6,
		[7]	=	99,
	};
	unsigned				result;

	result = 0;
	do	{
		if( choice < DIM( choices ) )	{
			result = choices[ choice ];
		}
	} while( 0 );
	return( result );
}

static inline	int				_pure _inline
boeing_to_timestamp(
	boeing_time_t const		bt,
	void * const			cell
)
{
	int						result;

	result = EXEC_BAD;
	switch( OPTION )	{
	default:
		break;
	case 5:
		{
			uint64_t			timestamp;
			unsigned			i;
			char *				bp;

			/* Make sure this time will fit into this option				*/
			printf(
				"Inbound boeing time = 0x%08lX:0x%08lX\n",
				(unsigned long) bt.seconds,
				(unsigned long) bt.nanoseconds
			);
			if( bt.seconds > 0x3FFFUL )	{
				/* Too many seconds for 14 bits to hold						*/
				break;
			}
			/* Build formatted time into a convenient form					*/
			timestamp  = (
				((uint64_t) bt.seconds & 0x3FFFULL) << (64 - 14)
			);
			timestamp |= (
				((uint64_t) bt.nanoseconds & 0x3FFFFFFFULL) << (64 - 14 - 30)
			);;
			printf( "Packed 64-bit timestamp = 0x%16llu\n", timestamp );
			/* Put into output buffer in network byte sex					*/
			for(
				bp = cell,
				i = 0;
				i < timestamp_size( OPTION );
				++i
			)	{
				*bp++ = timestamp >> (64-8);
				timestamp <<= 8;
			}
			result = EXEC_GOOD;
		}
		break;
	}
	return( result );
}

int
main(
	int				argc	_unused,
	char * *		argv	_unused
)
{
	boeing_time_t	bt;
	uint8_t			storagecell[ BUFSIZ ];
	int				result;

	printf( "Option %u uses %u bytes.\n", OPTION, timestamp_size( OPTION ) );
	bt.seconds = 0x2001UL;
	bt.nanoseconds = 0x20000001UL;
	printf(
		"Original boeing time = 0x%04lX.%08lX\n",
		(unsigned long) bt.seconds,
		(unsigned long) bt.nanoseconds
	);
	result = boeing_to_timestamp( bt, storagecell );
	if( result == EXEC_BAD )	{
		puts( "CONVERSION FAILED" );
	} else	{
		unsigned	i;

		printf( "Storage cell timestamp = '" );
		for( i = 0; i < timestamp_size( OPTION ); ++i )	{
			printf( "%02X", storagecell[ i ] );
		}
		printf( "'\n" );
	}
	return( 0 );
}

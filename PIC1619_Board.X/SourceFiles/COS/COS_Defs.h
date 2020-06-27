/** @file COS_Defs.h
 *  @ingroup group_COS_NutsBolts
 *
 * @author Steve Deasy
 * @author Joshua Hartwig
 *
 * 
 */

#ifndef COS_DEFS
#define COS_DEFS

/******************************************************************************/
/****************************** DECLARATIONS **********************************/
/******************************************************************************/

/* Misc or general purpose constant definitions for all ETM tasks. */
#define TRUE        1
#define FALSE       0
#define BYTE        8
#define BYTE_MSB    0x80
#define BYTE_LSB    0x01
/* Status Defines */
#define ON      1
#define OFF     0

#define HIGH    1
#define LOW     0

#define UINT_16_MAX 65535
#define UINT_16_MIN 0
#define SINT_16_MAX 32767
#define SINT_16_MIN -32768
#define UINT_8_MAX  255
#define UINT_8_MIN  0
#define SINT_8_MAX  127
#define SINT_8_MIN  -128


#define BIN0CONV    1       /* bin point 0 conversion factor */
#define BIN1CONV    2       /* bin point 1 conversion factor */
#define BIN2CONV    4       /* bin point 2 conversion factor */
#define BIN3CONV    8       /* bin point 3 conversion factor */
#define BIN4CONV    16      /* bin point 4 conversion factor */
#define BIN5CONV    32      /* bin point 5 conversion factor */
#define BIN6CONV    64      /* bin point 6 conversion factor */
#define BIN7CONV    128     /* bin point 7 conversion factor */
#define BIN8CONV    256     /* bin point 8 conversion factor */
#define BIN9CONV    512     /* bin point 9 conversion factor */
#define BIN10CONV   1024    /* bin point 10 conversion factor */
#define BIN11CONV   2058    /* bin point 11 conversion factor */
#define BIN12CONV   4096    /* bin point 12 conversion factor */
#define BIN13CONV   8192    /* bin point 13 conversion factor */
#define BIN14CONV   16384   /* bin point 14 conversion factor */
#define BIN15CONV   32768   /* bin point 15 conversion factor */
#define BIN16CONV   65536   /* bin point 16 conversion factor */


/* COS Variable Types */
typedef unsigned char UINT_1;   /* unsigned integer  8 bit value */
typedef unsigned char UINT_8;   /* unsigned integer  8 bit value */
typedef signed char SINT_8;     /* signed integer  8 bit value */
typedef unsigned short UINT_16; /* unsigned integer 16 bit value */
typedef signed short SINT_16;   /* signed integer 16 bit value */
typedef unsigned long UINT_32;  /* unsigned integer 32 bit value */
typedef signed long SINT_32;    /* signed integer 32 bit value */
typedef float FLOAT_24;         /* floating point 24 bit value */
typedef double FLOAT_32;        /* floating point 32 bit value */

#define MS_PER_SEC      1000        /* number of milliseconds in a second */
#define SECS_PER_MIN    60          /* number of seconds in a minute */
#define TIME_10_SECS    10000       /* number of milliseconds in ten seconds */
#define TIME_5_SECS     5000        /* number of milliseconds in five seconds */
#define TIME_4_SECS     4000        /* number of milliseconds in four seconds */
#define TIME_2_SECS     2000        /* number of milliseconds in two seconds */
#define TIME_ONE_SEC    1000        /* number of milliseconds in 1 second */
#define TIME_HALF_SEC   500         /* number of milliseconds in 1/2 second */
#define TIME_50_MSEC    50          /* number of milliseconds in 50 milliseconds */
#define TIME_100_MSEC   100         /* number of milliseconds in 100 milliseconds */

#define STATIC static
#define COS_FORCE_WDT_RESET()	while(1)

#endif
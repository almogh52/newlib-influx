/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 *
 * http://www.sgi.com
 *
 * For further information regarding this notice, see:
 *
 * http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */
/* $Id$ */
/**********************************************************
 * 
 *    OS Test - Silicon Graphics, Inc.
 * 
 *    TEST IDENTIFIER	: lseek04
 * 
 *    EXECUTED BY	: anyone
 * 
 *    TEST TITLE	: Negative test for lseek(2) of a fifo
 * 
 *    PARENT DOCUMENT	: usctpl01
 * 
 *    TEST CASE TOTAL	: 1
 * 
 *    WALL CLOCK TIME	: 1
 * 
 *    CPU TYPES		: ALL
 * 
 *    AUTHOR		: Richard Logan
 * 
 *    CO-PILOT		: William Roske
 * 
 *    DATE STARTED	: 04/25/94
 * 
 *    INITIAL RELEASE	: UNICOS 7.0
 * 
 *    TEST CASES
 * 
 * 	1.) lseek(2) returns...(See Description)
 *	
 *    INPUT SPECIFICATIONS
 * 	The standard options for system call tests are accepted.
 *	(See the parse_opts(3) man page).
 * 
 *    OUTPUT SPECIFICATIONS
 * 	
 *    DURATION
 * 	Terminates - with frequency and infinite modes.
 * 
 *    SIGNALS
 * 	Uses SIGUSR1 to pause before test if option set.
 * 	(See the parse_opts(3) man page).
 *
 *    RESOURCES
 * 	None
 * 
 *    ENVIRONMENTAL NEEDS
 *      No run-time environmental needs.
 * 
 *    SPECIAL PROCEDURAL REQUIREMENTS
 * 	None
 * 
 *    INTERCASE DEPENDENCIES
 * 	None
 * 
 *    DETAILED DESCRIPTION
 *	This is a Phase I test for the lseek(2) system call.  It is intended
 *	to provide a limited exposure of the system call, for now.  It
 *	should/will be extended when full functional tests are written for
 *	lseek(2).
 * 
 * 	Setup:
 * 	  Setup signal handling.
 *	  Pause for SIGUSR1 if option specified.
 * 
 * 	Test:
 *	 Loop if the proper options are given.
 * 	  Execute system call
 *	  Check return code, if system call failed (return=-1)
 *		Log the errno and Issue a FAIL message.
 *	  Otherwise, Issue a PASS message.
 * 
 * 	Cleanup:
 * 	  Print errno log and/or timing stats if options given
 * 
 * 
 *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#**/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "test.h"
#include "usctest.h"

void setup();
void cleanup(void) __attribute__((noreturn));



const char *TCID="lseek04";		/* Test program identifier.    */
int TST_TOTAL=1;    		/* Total number of test cases. */
extern int Tst_count;		/* Test Case counter for tst_* routines */

int exp_enos[]={0, 0};

char Fname[255];
int Fd;


int
main(int ac, char **av)
{
    int lc;		/* loop counter */
    const char *msg;		/* message returned from parse_opts */

    /***************************************************************
     * parse standard options
     ***************************************************************/
    if ( (msg=parse_opts(ac, av, (option_t *) NULL, NULL)) != (char *) NULL ) {
	tst_brkm(TBROK, NULL, "OPTION PARSING ERROR - %s", msg);
	tst_exit();
    }

    /***************************************************************
     * perform global setup for test
     ***************************************************************/
    setup();

    /* set the expected errnos... */
    TEST_EXP_ENOS(exp_enos);

    /***************************************************************
     * check looping state if -c option given
     ***************************************************************/
    for (lc=0; TEST_LOOPING(lc); lc++) {

	/* reset Tst_count in case we are looping. */
	Tst_count=0;

        /*
         *  Call lseek(2)
         */
	TEST(lseek(Fd, (long)1, SEEK_SET));
	
	/* check return code */
	if ( TEST_RETURN == -1 ) {
	    if ( STD_FUNCTIONAL_TEST ) {

	        if ( TEST_ERRNO == ESPIPE )
	            tst_resm(TPASS,
		        "lseek(fifofd, 1, SEEK_SET) Failed, errno=%d : %s",
		        TEST_ERRNO, strerror(TEST_ERRNO));
	         else
	            tst_resm(TFAIL,
			"lseek(fifofd, 1, SEEK_SET) Failed, errno=%d %s, expected %d(ESPIPE)",
		        TEST_ERRNO, strerror(TEST_ERRNO),
			EINVAL);
	    }
	    else
	       Tst_count++;
	} else {
	    
	        tst_resm(TFAIL, "lseek(fifofd, 1, SEEK_SET) returned %d",
		    TEST_RETURN);
	}

    }	/* End for TEST_LOOPING */

    /***************************************************************
     * cleanup and exit
     ***************************************************************/
    cleanup();

    return 0;
}	/* End main */

/***************************************************************
 * setup() - performs all ONE TIME setup for this test.
 ***************************************************************/
void 
setup()
{
    /* capture signals */
    tst_sig(NOFORK, DEF_HANDLER, cleanup);

    /* make a temp directory and cd to it */
    tst_tmpdir();

    /* Pause if that option was specified */
    TEST_PAUSE;

    sprintf(Fname,"tfile_%d",getpid());
    if ( mkfifo(Fname,0700) == -1) {
        tst_brkm(TBROK, cleanup,
	    "mkfifo(%s, 0700) Failed, errno=%d : %s",
	    Fname, errno, strerror(errno));
    }

    if ( (Fd=open(Fname, O_RDWR, 0777)) == -1) {
        tst_brkm(TBROK, cleanup,
	    "open(%s, O_RDWR, 0777) Failed, errno=%d : %s",
	    Fname, errno, strerror(errno));
    }
}	/* End setup() */


/***************************************************************
 * cleanup() - performs all ONE TIME cleanup for this test at
 *		completion or premature exit.
 ***************************************************************/
void 
cleanup()
{
    /*
     * print timing stats if that option was specified.
     * print errno log if that option was specified.
     */
    TEST_CLEANUP;

    /* close the file we have open */
    if (close(Fd) == -1) {
       tst_resm(TWARN, "close(%s) Failed, errno=%d : %s", Fname, errno, strerror(errno));
    }

    /* Remove tmp dir and all files in it */
    tst_rmdir();

    /* exit with return code appropriate for results */
    tst_exit();
}	/* End cleanup() */

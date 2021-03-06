/*****************************************************************************
    test_test.c
    Zac Hester
    2012-10-19

    Test the test framework.

*****************************************************************************/

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include "test.h"

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Types and Structures
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Memory Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Module Variables
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Module Prototypes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------*/

/*==========================================================================*/
int run_test(                       //run the unit test
    test_control_type*  test        //test control object
) {                                 //result of test

    #define MEM_LEN ( 9 )
    static char         mema[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19 };
    static char         memb[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19 };
    static char         memc[] = { 8, 6, 7, 5, 3,  0,  9,  4,  2 };

    test_hdg( test, "Test Test Framework", 0 );

    test_hdg( test, "Test Verification", 1 );

    test_hdg( test, "Test Verify: Boolean", 2 );
    test_v_boolean( test, 0, 0 );
    test_v_boolean( test, 1, 42 );
    test_v_boolean( test, 0, 42 );

    test_hdg( test, "Test Verify: Integer", 2 );
    test_v_long( test, 42, 42 );
    test_v_long( test, 3, 5 );

    test_hdg( test, "Test Verify: Double, Range", 2 );
    test_v_double_range( test, 3.14, 3.0, 3.2 );
    test_v_double_range( test, 3.14, 0.1, 0.2 );

    test_hdg( test, "Test Verify: Double, Tolerance", 2 );
    test_v_double_tolerance( test, 3.14, 3.0, 0.2 );
    test_v_double_tolerance( test, 3.14, 1.0, 0.5 );

    test_hdg( test, "Test Verify: String", 2 );
    test_v_str( test, "Hello World", "Hello World" );
    test_v_str( test, "Hello World", "Hello Earth" );

    test_hdg( test, "Test Verify: Memory", 2 );
    test_v_mem( test, mema, memb, MEM_LEN );
    test_v_mem( test, mema, memc, MEM_LEN );

    test_hdg( test, "Test Verify: Pointer", 2 );
    test_v_ptr( test, mema );

    test_hdg( test, "Test Verify: NULL", 2 );
    test_v_null( test, NULL );

    return 0;
}


/*----------------------------------------------------------------------------
Test Stubs
----------------------------------------------------------------------------*/

/*==========================================================================*/

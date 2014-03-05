/*****************************************************************************
    test.c
    Zac Hester
    2012-09-19

    Notes

*****************************************************************************/

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

extern int run_test(                //allow the linker to use run_test()
    test_control_type*  test        //pointer to test control object
);                                  //result of test


/*----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------*/


/*==========================================================================*/
int main(                           //entry point of test runner
    int                 argc,       //number of command-line arguments
    char**              argv        //list of command-line arguments
) {                                 //test result status

    //local variables
    int                 result;     //result of unit test
    test_control_type   test;       //unit test control object

    //clear the test object
    memset( &test, 0, sizeof( test_control_type ) );

    //check for an output log file
    if( argc >= 2 ) {
        test.out = fopen( argv[ 1 ], "w" );
        if( test.out == NULL ) {
            printf( "Unable to open \"%s\" for writing.\n", argv[ 1 ] );
            return 1;
        }
    }

    //run the unit test
    result = run_test( &test );

    //check the result of the run
    if( ( result == 0 ) && ( test.result == 0 ) ) {
        test_out( &test, "\nTEST PASSED\n" );
        if( test.out != NULL ) {
            printf( "TEST PASSED\n" );
        }
    }
    else {
        test_out( &test, "\nTEST FAILED\n" );
        if( test.out != NULL ) {
            printf( "TEST FAILED\n" );
        }
    }

    //check for open output file
    if( test.out != NULL ) {
        fclose( test.out );
    }

    //return the result of the test
    return result;
}


/*==========================================================================*/
void test_fmt(                      //write formatted text to a test
    test_control_type*  test,       //the test instance
    const char*         format,     //the format string
    ...                             //format parameters
) {

    //local variables
    FILE*               fd;         //output file descriptor
    va_list             varargs;    //variable argument list

    //set the output file descriptor
    fd = test->out != NULL ? test->out : stdout;

    va_start( varargs, format );

    //output the formatted text
    vfprintf( fd, format, varargs );

    va_end( varargs );
}


/*==========================================================================*/
void test_hdg(                      //write a heading to a test
    test_control_type*  test,       //the test instance
    char*               hdg,        //heading to write
    unsigned char       level       //heading level (0 -> 3)
) {

    //local variables
    FILE*               fd;         //output file descriptor

    //check level
    if( level >= TEST_NUM_HDGS ) {
        test->result = 1;
        test_out(
            test,
            "Test Error: Invalid heading level specified"
            " in call to test_hdg().\n"
        );
        return;
    }

    //set the output file descriptor
    fd = test->out != NULL ? test->out : stdout;

    //display heading based on level
    switch( level ) {

        //major headings provide document separations
        case 0:
            test_fmt(
                test,
                "\n=============================="
                "==============================\n"
                "%d. %s"
                "\n=============================="
                "==============================\n",
                ( test->headings[ level ] + 1 ),
                hdg
            );
            break;

        //second-level headings are important
        case 1:
            test_fmt(
                test,
                "\n  %d. %s"
                "\n------------------------------"
                "------------------------------\n",
                ( test->headings[ level ] + 1 ),
                hdg
            );
            break;

        //third-level headings
        case 2:
            test_fmt(
                test,
                "\n    %d. %s"
                "\n------------------------------"
                "------------------------------\n",
                ( test->headings[ level ] + 1 ),
                hdg
            );
            break;

        //all lower-level headings
        default:
            test_fmt(
                test,
                "\n      %d. %s"
                "\n------------------------------"
                "------------------------------\n",
                ( test->headings[ level ] + 1 ),
                hdg
            );
            break;
    }

    //clear all lower-level heading counters
    memset( &( test->headings[ level + 1 ] ), 0, ( TEST_NUM_HDGS - level ) );

    //increment this heading counter
    test->headings[ level ] += 1;
}


/*==========================================================================*/
void test_out(                      //write text to a test
    test_control_type*  test,       //the test instance
    const char*         text        //text to append to test output
) {

    //local variables
    FILE*               fd;         //output file descriptor

    //set the output file descriptor
    fd = test->out != NULL ? test->out : stdout;

    //output the text
    fputs( text, fd );
}


/*==========================================================================*/
int test_v_boolean(                 //verify values have boolean equivalence
    test_control_type*  test,       //the test instance
    long                value0,     //value 0
    long                value1      //value 1
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: Boolean\n"
        "        Given:     %ld\n"
        "        Expected:  %ld\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        value0,
        value1
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //compare for both false or true values
    if( ( ( value0 == 0 ) && ( value1 == 0 ) )
     || ( ( value0 != 0 ) && ( value1 != 0 ) ) ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }

    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_double_range(            //verify a double is within a range
    test_control_type*  test,       //the test instance
    double              num,        //number
    double              minimum,    //minimum allowed value
    double              maximum     //maximum allowed value
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: Double, Range\n"
        "        Given:     %f\n"
        "        Minimum:   %f\n"
        "        Maximum:   %f\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        num,
        minimum,
        maximum
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if number is within range
    if( ( num >= minimum ) && ( num <= maximum ) ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_double_tolerance(        //verify a double using a tolerance
    test_control_type*  test,       //the test instance
    double              num,        //number
    double              target,     //target value
    double              precision   //precision (+/-) about target value
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: Double, Tolerance\n"
        "        Given:     %f\n"
        "        Expected:  %f\n"
        "        Precision: %f\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        num,
        target,
        precision
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if number has an acceptable value
    if( ( num >= ( target - precision ) )
     && ( num <= ( target + precision ) ) ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_long(                    //verify long integers are equal
    test_control_type*  test,       //the test instance
    long                num0,       //number 0
    long                num1        //number 1
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: Integer\n"
        "        Given:     %ld\n"
        "        Expected:  %ld\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        num0,
        num1
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if integers are equal
    if( num0 == num1 ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_mem(                     //verify memory has equal values
    test_control_type*  test,       //the test instance
    const void*         mem0,       //memory space 0
    const void*         mem1,       //memory space 1
    unsigned short      size        //size of memory space
) {                                 //0 if verified

    int                 result;
    unsigned short      index;

    test_fmt(
        test,
        "    %d. Verify: Memory\n        Given:",
        ( test->headings[ TEST_NUM_HDGS ] + 1 )
    );
    test->headings[ TEST_NUM_HDGS ] += 1;

    for( index = 0; index < size; ++index ) {
        if( ( index % 16 ) == 0 ) {
            test_out( test, "\n          " );
        }
        test_fmt( test, "%02X ", *( ( ( unsigned char* ) mem0 ) + index ) );
    }
    test_out( test, "\n" );

    test_out( test, "        Expected:" );

    for( index = 0; index < size; ++index ) {
        if( ( index % 16 ) == 0 ) {
            test_out( test, "\n          " );
        }
        test_fmt( test, "%02X ", *( ( ( unsigned char* ) mem1 ) + index ) );
    }
    test_out( test, "\n" );

    //see if memory segments contain equal data
    if( memcmp( mem0, mem1, size ) == 0 ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_null(                    //verify null pointer
    test_control_type*  test,       //the test instance
    void*               ptr         //pointer
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: NULL\n"
        "        Given:     %p\n"
        "        Expected:  %p\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        ptr,
        NULL
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if pointer is null
    if( ptr == NULL ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_ptr(                     //verify non-null pointer
    test_control_type*  test,       //the test instance
    void*               ptr         //pointer
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: Pointer\n"
        "        Given:     %p\n"
        "        Expected:  NOT %p\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        ptr,
        NULL
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if pointer is not null
    if( ptr != NULL ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}


/*==========================================================================*/
int test_v_str(                     //verify strings are the same
    test_control_type*  test,       //the test instance
    const char*         str0,       //string 0
    const char*         str1        //string 1
) {                                 //0 if verified

    int                 result;

    test_fmt(
        test,
        "    %d. Verify: String\n"
        "        Given:     %s\n"
        "        Expected:  %s\n",
        ( test->headings[ TEST_NUM_HDGS ] + 1 ),
        str0,
        str1
    );

    test->headings[ TEST_NUM_HDGS ] += 1;

    //see if strings are equal
    if( strcmp( str0, str1 ) == 0 ) {
        test_out( test, "        Result:    PASSED\n" );
        result = 0;
    }
    else {
        test_out( test, "        Result:    FAILED\n" );
        test->result = 1;
        result = 1;
    }

    return result;
}

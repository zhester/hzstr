/*****************************************************************************
    test.h
    Zac Hester
    2012-09-19

    Description

    Example Usage

*****************************************************************************/

#ifndef _TEST_H
#define _TEST_H

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include <stdio.h>

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

#define TEST_NUM_HDGS ( 4 )         //number of heading levels

//convenience/readability macros
#define t_hdg( _h ) test_hdg( test, ( _h ), 0 )
#define t_sec( _s ) test_hdg( test, ( _s ), 1 ); reset_test()
#define t_sub( _s ) test_hdg( test, ( _s ), 2 )
#define t_stp( _s ) test_hdg( test, ( _s ), 3 )

/*----------------------------------------------------------------------------
Types and Structures
----------------------------------------------------------------------------*/

typedef struct {                    //test control type
    FILE*               out;        //log output file descriptor
    int                 result;     //current test result
    unsigned char       headings[ TEST_NUM_HDGS + 1 ];
                                    //heading counters (+1 for verifies)
} test_control_type;

/*----------------------------------------------------------------------------
Memory Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Interface Prototypes
----------------------------------------------------------------------------*/

void test_fmt(                      //write formatted text to a test
    test_control_type*  test,       //the test instance
    const char*         format,     //the format string
    ...                             //format parameters
);

void test_hdg(                      //write a heading to a test
    test_control_type*  test,       //the test instance
    char*               hdg,        //heading to write
    unsigned char       level       //heading level (0 -> 3)
);

void test_out(                      //write text to a test
    test_control_type*  test,       //the test instance
    const char*         text        //text to append to test output
);

int test_v_boolean(                 //verify values have boolean equivalence
    test_control_type*  test,       //the test instance
    long                value0,     //value 0
    long                value1      //value 1
);                                  //0 if verified

int test_v_double_range(            //verify a double is within a range
    test_control_type*  test,       //the test instance
    double              num,        //number
    double              minimum,    //minimum allowed value
    double              maximum     //maximum allowed value
);                                  //0 if verified

int test_v_double_tolerance(        //verify a double using a tolerance
    test_control_type*  test,       //the test instance
    double              num,        //number
    double              target,     //target value
    double              precision   //precision (+/-) about target value
);                                  //0 if verified

int test_v_long(                    //verify long integers are equal
    test_control_type*  test,       //the test instance
    long                num0,       //number 0
    long                num1        //number 1
);                                  //0 if verified

int test_v_ptr(                     //verify non-null pointer
    test_control_type*  test,       //the test instance
    void*               ptr         //pointer
);                                  //0 if verified

int test_v_mem(                     //verify memory has equal values
    test_control_type*  test,       //the test instance
    const void*         mem0,       //memory space 0
    const void*         mem1,       //memory space 1
    unsigned short      size        //size of memory space
);                                  //0 if verified

int test_v_null(                    //verify null pointer
    test_control_type*  test,       //the test instance
    void*               ptr         //pointer
);                                  //0 if verified

int test_v_ptr(                     //verify non-null pointer
    test_control_type*  test,       //the test instance
    void*               ptr         //pointer
);                                  //0 if verified

int test_v_str(                     //verify strings are the same
    test_control_type*  test,       //the test instance
    const char*         str0,       //string 0
    const char*         str1        //string 1
);                                  //0 if verified

#endif /* _TEST_H */

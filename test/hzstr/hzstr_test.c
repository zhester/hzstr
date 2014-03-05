/*****************************************************************************
    hzstr_test.c
    Zac Hester
    2012-10-24

    Notes

*****************************************************************************/

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "test.h"

#include "hzstr.h"

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

#define MAX_ALLOCS ( 8 )

//define symbols to disable the corresponding test section
//#define TNO_APPEND
//#define TNO_CREATE
//#define TNO_CREATE_SUBSTR
//#define TNO_CAT
//#define TNO_CPY
//#define TNO_CMP
//#define TNO_DESTROY
//#define TNO_IMPORT
//#define TNO_MINIMIZE
//#define TNO_SPRINTF
//#define TNO_STRCHR
//#define TNO_TOCASE
//#define TNO_TRIM

/*----------------------------------------------------------------------------
Types and Structures
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Memory Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Module Variables
----------------------------------------------------------------------------*/

unsigned char           fail_calloc;
unsigned char           fail_callocs[ MAX_ALLOCS ];
unsigned char           fail_malloc;
unsigned char           fail_mallocs[ MAX_ALLOCS ];
unsigned char           fail_realloc;
unsigned char           fail_reallocs[ MAX_ALLOCS ];

long                    comp;
hzstr_index_t           strindex;
hzstr_result_t          result;
hzstr_type*             string;
hzstr_type*             string2;
hzstr_type*             substring;

/*----------------------------------------------------------------------------
Module Prototypes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------*/


/*==========================================================================*/
void reset_test(
    void
) {
    fail_calloc  = 0;
    fail_malloc  = 0;
    fail_realloc = 0;
    memset( fail_callocs,  0, MAX_ALLOCS );
    memset( fail_mallocs,  0, MAX_ALLOCS );
    memset( fail_reallocs, 0, MAX_ALLOCS );

    comp      = 0;
    strindex  = 0;
    result    = 0;
    string    = NULL;
    string2   = NULL;
    substring = NULL;
}


/*==========================================================================*/
int run_test(                       //run the unit test
    test_control_type*  test        //test control object
) {                                 //result of test

#ifndef TNO_APPEND
    t_hdg( "String Append" );

        t_sec( "hzstr_append()" );
        string = hzstr_create_cstr( "Hello " );
        hzstr_append( string, 'W' );
        test_v_str( test, hzstr_cstr( string ), "Hello W" );
        hzstr_destroy( string );

        t_sec( "hzstr_append(), invalid usage" );
        result = hzstr_append( NULL, 'Z' );
        test_v_long( test, result, HZSTR_RSLT_USAGE );

        t_sec( "hzstr_append(), invalid target string" );
        string = hzstr_create_ccstr( "Hello " );
        result = hzstr_append( string, 'W' );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );

        t_sec( "hzstr_append(), failed reallocation" );
        string = hzstr_create_cstr( "Hello Hello Hello Hello Hello H" );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_append( string, 'W' );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string );
#endif

#ifndef TNO_CREATE
    t_hdg( "String Creation" );

        t_sec( "hzstr_create(), empty string" );
        string = hzstr_create( 0 );
        test_v_ptr( test, string );
        test_v_long( test, hzstr_length( string ), 0 );
        test_v_long( test, string->alloc, HZSTR_CHUNK_SIZE );
        test_v_str( test, hzstr_cstr( string ), "" );
        hzstr_destroy( string );

        t_sec( "hzstr_create(), failed data allocation" );
        fail_mallocs[ 0 ] = 1;
        string = hzstr_create( 0 );
        test_v_null( test, string );

        t_sec( "hzstr_create(), failed object allocation" );
        fail_callocs[ 0 ] = 1;
        string = hzstr_create( 0 );
        test_v_null( test, string );

        t_sec( "hzstr_create_ccstr(), static string" );
        string = hzstr_create_ccstr( "Hello World" );
        test_v_ptr( test, string );
        test_v_long( test, hzstr_length( string ), 11 );
        test_v_long( test, string->alloc, 0 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_create_ccstr(), failed object allocation" );
        fail_callocs[ 0 ] = 1;
        string = hzstr_create_ccstr( "Hello World" );
        test_v_null( test, string );
        hzstr_destroy( string );

        t_sec( "hzstr_create_cstr(), C string" );
        string = hzstr_create_cstr( "Hello World" );
        test_v_ptr( test, string );
        test_v_long( test, hzstr_length( string ), 11 );
        test_v_long( test, string->alloc, HZSTR_CHUNK_SIZE );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_create_cstr(), failed object allocation" );
        fail_callocs[ 0 ] = 1;
        string = hzstr_create_cstr( "Hello World" );
        test_v_null( test, string );
        hzstr_destroy( string );
#endif

#ifndef TNO_CREATE_SUBSTR
    t_hdg( "Substring Creation" );

        t_sec( "hzstr_create_substr()" );
        string = hzstr_create_cstr( "Hello World" );
        substring = hzstr_create_substr( string, 3, 6 );
        test_v_str( test, hzstr_cstr( substring ), "lo World" );
        string2 = hzstr_create( 0 );
        result = hzstr_cpy( string2, substring );
        test_v_long( test, result, hzstr_length( substring ) );
        test_v_str( test, hzstr_cstr( string2 ), "lo Wor" );
        hzstr_destroy( string2 );
        hzstr_destroy( substring );
        hzstr_destroy( string );

        t_sec( "hzstr_create_substr(), failed object allocation" );
        string = hzstr_create_cstr( "Hello World" );
        fail_callocs[ 1 ] = 1;
        substring = hzstr_create_substr( string, 3, 6 );
        test_v_null( test, substring );
        hzstr_destroy( substring );
        hzstr_destroy( string );

        t_sec( "hzstr_create_substr(), invalid offset" );
        string = hzstr_create_cstr( "Hello World" );
        substring = hzstr_create_substr( string, 20, 6 );
        test_v_null( test, substring );
        hzstr_destroy( substring );
        hzstr_destroy( string );

        t_sec( "hzstr_create_substr(), invalid length" );
        string = hzstr_create_cstr( "Hello World" );
        substring = hzstr_create_substr( string, 2, 25 );
        test_v_null( test, substring );
        hzstr_destroy( substring );
        hzstr_destroy( string );

        t_sec( "hzstr_create_substr(), invalid offset and length" );
        string = hzstr_create_cstr( "Hello World" );
        substring = hzstr_create_substr( string, 20, 25 );
        test_v_null( test, substring );
        hzstr_destroy( substring );
        hzstr_destroy( string );
#endif

#ifndef TNO_CAT
    t_hdg( "String Concatenation" );

        t_sec( "hzstr_cat()" );
        string = hzstr_create_cstr( "Hello " );
        string2 = hzstr_create_cstr( "World" );
        result = hzstr_cat( string, string2 );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cat(), invalid target string" );
        string = hzstr_create_ccstr( "Hello " );
        string2 = hzstr_create_cstr( "World" );
        result = hzstr_cat( string, string2 );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cat(), failed reallocation" );
        string = hzstr_create_cstr( "Hello " );
        string2 = hzstr_create_cstr( "World World World World World World" );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_cat( string, string2 );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cat(), increased reallocation" );
        string = hzstr_create_cstr( "Hello " );
        string2 = hzstr_create_cstr( "World World World World World World" );
        result = hzstr_cat( string, string2 );
        test_v_long( test, result, 41 );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cat_cstr()" );
        string = hzstr_create_cstr( "Hello " );
        result = hzstr_cat_cstr( string, "World" );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_cat_cstr(), invalid target string" );
        string = hzstr_create_ccstr( "Hello " );
        result = hzstr_cat_cstr( string, "World" );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );

        t_sec( "hzstr_cat_cstr(), failed reallocation" );
        string = hzstr_create_cstr( "Hello " );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_cat_cstr( string, "World World World World World World" );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string );
#endif

#ifndef TNO_CPY
    t_hdg( "String Copying" );

        t_sec( "hzstr_cpy()" );
        string = hzstr_create( 0 );
        string2 = hzstr_create_cstr( "Hello World" );
        result = hzstr_cpy( string, string2 );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cpy(), invalid target string" );
        string = hzstr_create_ccstr( "Constant" );
        string2 = hzstr_create_cstr( "Hello World" );
        result = hzstr_cpy( string, string2 );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cpy(), failed reallocation" );
        string = hzstr_create( 0 );
        string2 = hzstr_create_cstr(
            "Hello World World World World World World"
        );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_cpy( string, string2 );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cpy_cstr()" );
        string = hzstr_create( 0 );
        result = hzstr_cpy_cstr( string, "Hello World" );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_cpy_cstr(), invalid target string" );
        string = hzstr_create_ccstr( "Constant" );
        result = hzstr_cpy_cstr( string, "Hello World" );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );

        t_sec( "hzstr_cpy_cstr(), failed reallocation" );
        string = hzstr_create( 0 );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_cpy_cstr(
            string,
            "Hello World World World World World World"
        );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string );
#endif

#ifndef TNO_CMP
    t_hdg( "String Comparisons" );

        t_sec( "hzstr_cmp(), same strings" );
        string = hzstr_create_cstr( "Hello World" );
        string2 = hzstr_create_cstr( "Hello World" );
        comp = hzstr_cmp( string, string2 );
        test_v_long( test, comp, 0 );
        hzstr_destroy( string2 );
        hzstr_destroy( string );

        t_sec( "hzstr_cmp(), different strings" );
        string = hzstr_create_cstr( "Hello World" );
        string2 = hzstr_create_cstr( "Hello Earth" );
        comp = hzstr_cmp( string, string2 );
        test_v_long( test, comp, ( 'W' - 'E' ) );
        hzstr_destroy( string2 );
        hzstr_destroy( string );
#endif

#ifndef TNO_DESTROY
    t_hdg( "Object Destruction" );

        t_sec( "hzstr_destroy(), invalid object" );
        string = hzstr_create( 0 );
        free( string->data );
        string->data = NULL;
        test_v_null( test, string->data );
        hzstr_destroy( string );
#endif

#ifndef TNO_IMPORT
    t_hdg( "String Import" );

        t_sec( "hzstr_import()" );
        string = hzstr_create( 0 );
        result = hzstr_import( string, "Hello Beautiful World!", 15 );
        test_v_long( test, result, HZSTR_RSLT_SAFETY );
        test_v_long( test, hzstr_length( string ), 0 );
        result = hzstr_import( string, "Hello Beautiful", 15 );
        test_v_long( test, result, 15 );
        test_v_long( test, hzstr_length( string ), 15 );
        test_v_str( test, hzstr_cstr( string ), "Hello Beautiful" );
        hzstr_destroy( string );

        t_sec( "hzstr_import(), invalid target string" );
        string = hzstr_create_ccstr( "Constant" );
        result = hzstr_import( string, "Hello World", 11 );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );
#endif

#ifndef TNO_MINIMIZE
    t_hdg( "String Memory Minimize" );

        t_sec( "hzstr_minimize()" );
        string = hzstr_create_cstr( "Hello World" );
        test_v_long( test, string->alloc, 32 );
        result = hzstr_minimize( string );
        test_v_long( test, result, 12 );
        test_v_long( test, string->alloc, 12 );
        hzstr_destroy( string );

        t_sec( "hzstr_minimize(), invalid target string" );
        string = hzstr_create_ccstr( "Hello World" );
        result = hzstr_minimize( string );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );

        t_sec( "hzstr_minimize(), failed reallocation" );
        string = hzstr_create_cstr( "Hello World" );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_minimize( string );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string );
#endif

#ifdef HZSTR_COMPAT_CSTR
#ifndef TNO_SPRINTF
    t_hdg( "String Formatted Printing" );

        t_sec( "hzstr_sprintf()" );
        string = hzstr_create( 0 );
        result = hzstr_sprintf( string, "%s %s", "Hello", "World" );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_sprintf(), invalid target string" );
        string = hzstr_create_ccstr( "Constant" );
        result = hzstr_sprintf( string, "%s %s", "Hello", "World" );
        test_v_long( test, result, HZSTR_RSLT_TYPE );
        hzstr_destroy( string );

        t_sec( "hzstr_sprintf(), failed reallocation" );
        string = hzstr_create( 0 );
        fail_reallocs[ 0 ] = 1;
        result = hzstr_sprintf(
            string,
            "%s %s",
            "Hello",
            "World World World World World World"
        );
        test_v_long( test, result, HZSTR_RSLT_ALLOC );
        hzstr_destroy( string );
#endif
#endif

#ifndef TNO_STRCHR
    t_hdg( "Character Search" );

        t_sec( "hzstr_strchr()" );
        string = hzstr_create_cstr( "Hello World" );
        strindex = hzstr_strchr( string, 'W' );
        test_v_long( test, strindex, 6 );
        strindex = hzstr_strchr( string, 'z' );
        test_v_long( test, strindex, HZSTR_INDEX_NONE );
        hzstr_destroy( string );
#endif

#ifndef TNO_TOCASE
    t_hdg( "Changing Case" );

        t_sec( "hzstr_tolower()" );
        string = hzstr_create_cstr( "Hello World~" );
        result = hzstr_tolower( string );
        test_v_long( test, result, HZSTR_RSLT_OK );
        test_v_str( test, hzstr_cstr( string ), "hello world~" );
        hzstr_destroy( string );

        t_sec( "hzstr_toupper()" );
        string = hzstr_create_cstr( "Hello World~" );
        result = hzstr_toupper( string );
        test_v_long( test, result, HZSTR_RSLT_OK );
        test_v_str( test, hzstr_cstr( string ), "HELLO WORLD~" );
        hzstr_destroy( string );
#endif

#ifndef TNO_TRIM
    t_hdg( "String Trim" );

        t_sec( "hzstr_trim()" );
        string = hzstr_create_cstr( "\t Hello World     \r\n" );
        test_v_long( test, hzstr_length( string ), 20 );
        result = hzstr_trim( string );
        test_v_long( test, result, 11 );
        test_v_str( test, hzstr_cstr( string ), "Hello World" );
        hzstr_destroy( string );

        t_sec( "hzstr_trim(), empty string" );
        string = hzstr_create_cstr( "" );
        test_v_long( test, hzstr_length( string ), 0 );
        result = hzstr_trim( string );
        test_v_long( test, result, 0 );
        test_v_str( test, hzstr_cstr( string ), "" );
        hzstr_destroy( string );
#endif

    return 0;
}


/*----------------------------------------------------------------------------
Test Stubs
----------------------------------------------------------------------------*/


/*==========================================================================*/
void* mem_calloc(
    size_t              num,
    size_t              size
) {
    void*               ptr;
    if( fail_callocs[ fail_calloc ] == 0 ) {
        ptr = calloc( num, size );
    }
    else {
        ptr = NULL;
    }
    if( fail_calloc < ( MAX_ALLOCS - 1 ) ) {
        fail_calloc += 1;
    }
    return ptr;
}


/*==========================================================================*/
void mem_free(
    void*               ptr
) {
    free( ptr );
}


/*==========================================================================*/
void* mem_malloc(
    size_t              size
) {
    void*               ptr;
    if( fail_mallocs[ fail_malloc ] == 0 ) {
        ptr = malloc( size );
    }
    else {
        ptr = NULL;
    }
    if( fail_malloc < ( MAX_ALLOCS - 1 ) ) {
        fail_malloc += 1;
    }
    return ptr;
}


/*==========================================================================*/
void* mem_realloc(
    void*               ptr,
    size_t              size
) {
    void*               reptr;
    if( fail_reallocs[ fail_realloc ] == 0 ) {
        reptr = realloc( ptr, size );
    }
    else {
        reptr = NULL;
    }
    if( fail_realloc < ( MAX_ALLOCS - 1 ) ) {
        fail_realloc += 1;
    }
    return reptr;
}

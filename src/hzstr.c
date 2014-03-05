/*****************************************************************************
    hzstr.c
    Zac Hester
    2012-10-16

    Notes

*****************************************************************************/

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdio.h>

#include "hzstr.h"
#include "memory.h"

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

//performance tuning
#define CHUNK_SIZE ( 32 )           //allocation chunk size

//useful values
#define MAX_LENGTH ( 65535 )        //largest possible single string

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

static hzstr_result_t check_alloc(  //check a string's allocation
    hzstr_type*         string,     //the string to check
    hzstr_length_t      length      //the desired length of the string
);                                  //result of the check

static hzstr_length_t cstr_len(     //compute length of C-string
    const char*         cstring     //pointer to C-string
);                                  //length of C-string

static hzstr_length_t str_alloc(    //allocate/re-allocate string memory
    hzstr_char_t**      target,     //target string pointer
    hzstr_char_t*       source,     //optional source pointer
    hzstr_length_t      length      //length to allocate
);                                  //number of bytes actually allocated

/*----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------*/


/*==========================================================================*/
hzstr_result_t hzstr_append(        //append character onto end of string
    hzstr_type*         string,     //target string
    char                c           //character to append
) {                                 //result of appending

    //local variables
    hzstr_result_t      result;     //allocation result

    //check pointer
    if( string == NULL ) {
        return HZSTR_RSLT_USAGE;
    }

    //check the type of target string
    if( string->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //check allocation of target string
    result = check_alloc( string, ( string->length + 1 ) );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

    //append the character
    string->data[ string->length ] = ( hzstr_char_t ) c;
    string->length += 1;
    string->data[ string->length ] = 0;

    //return new length of string
    return string->length;
}


/*==========================================================================*/
hzstr_type* hzstr_create(           //create a new string
    hzstr_length_t      length      //initial length to create (0 for default)
) {                                 //pointer to new string

    //local variables
    hzstr_type*         string;     //pointer to new string

    //allocate a string object
    string = mem_calloc( 1, sizeof( hzstr_type ) );

    //check allocation
    if( string != NULL ) {

        //allocate memory for a new string
        length = str_alloc( &string->data, NULL, length );

        //check allocation
        if( string->data == NULL ) {
            mem_free( string );
            return NULL;
        }

        //null terminate string
        string->data[ 0 ] = 0;

        //initialize string object
        string->type  = HZSTR_TYPE_DYNAMIC;
        string->flags = HZSTR_FLG_WRITABLE;
        string->alloc = length;
    }

    //return the string pointer
    return string;
}


/*==========================================================================*/
#ifdef HZSTR_COMPAT_CSTR

hzstr_type* hzstr_create_ccstr(     //create new string using a const C-string
    const char*         cstring     //C string
) {                                 //pointer to new string

    //local variables
    hzstr_type*         string;     //pointer to new string

    //allocate a string object
    string = mem_calloc( 1, sizeof( hzstr_type ) );

    //check allocation
    if( string != NULL ) {

        //use the constant memory location for this string
        string->data = ( hzstr_char_t* ) cstring;

        //initialize the string object
        string->type   = HZSTR_TYPE_CONST;
        string->length = cstr_len( string->data );
    }

    //return the new string object
    return string;
}

#endif


/*==========================================================================*/
hzstr_type* hzstr_create_cstr(      //create new string from a C-string
    const char*         cstring     //C string
) {                                 //pointer to new string

    //local variables
    hzstr_type*         string;     //pointer to new string

    //allocate the string
    string = hzstr_create( cstr_len( cstring ) );

    //check allocation
    if( string != NULL ) {

        //copy the C string into the string
        hzstr_cpy_cstr( string, cstring );
    }

    //return the new string
    return string;
}


/*==========================================================================*/
hzstr_type* hzstr_create_substr(    //create a substring of another string
    const hzstr_type*   source,     //source string
    hzstr_index_t       offset,     //beginning offset in string
    hzstr_length_t      length      //length of substring
) {                                 //pointer to new substring

    //local variables
    hzstr_type*         string;     //pointer to new string

    //validate the requested substring
    if( ( offset >= source->length             )
     || ( length > ( source->length - offset ) ) ) {
        return NULL;
    }

    //create a new container string object
    string = mem_calloc( 1, sizeof( hzstr_type ) );

    //check allocation
    if( string != NULL ) {

        //set the data pointer
        string->data = source->data + offset;

        //initialize the string object
        string->type   = HZSTR_TYPE_SUBSTR;
        string->alloc  = 0;
        string->length = length;
    }

    //return the new string
    return string;
}


/*==========================================================================*/
hzstr_result_t hzstr_cat(           //concatenate two strings
    hzstr_type*         target,     //concatenation target
    const hzstr_type*   source      //concatenation source
) {                                 //result of operation

    //local variables
    hzstr_result_t      result;     //allocation result

    //check the type of target string
    if( target->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //check allocation of target string
    result = check_alloc( target, ( target->length + source->length ) );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

    //copy the source data onto the end of the target data
    mem_copy(
        &( target->data[ target->length ] ),
        source->data,
        source->length
    );

    //update the length of the target
    target->length += source->length;

    //null-terminate the target string
    target->data[ target->length ] = 0;

    //return the new length
    return target->length;
}


/*==========================================================================*/
hzstr_result_t hzstr_cat_cstr(      //concatenate a C-string onto a string
    hzstr_type*         target,     //concatenation target
    const char*         source      //concatenation source
) {                                 //result of operation
    //local variables
#ifndef HZSTR_COMPAT_CSTR
    hzstr_index_t       index;      //index
#endif
    hzstr_length_t      length;     //length of source string
    hzstr_result_t      result;     //allocation result

    //check the type of target string
    if( target->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //find the length of the source string
    length = cstr_len( source );

    //check allocation of target string
    result = check_alloc( target, ( target->length + length ) );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

#ifdef HZSTR_COMPAT_CSTR

    //copy the source onto the end of the target
    mem_copy( ( target->data + target->length ), source, length );

#else

    //copy the source onto the end of the target
    for( index = 0; index < length; ++index ) {
        target->data[ target->length + index ]
            = ( hzstr_char_t ) source[ index ];
    }

#endif

    //update the length of the target string
    target->length += length;

    //null-terminate target string
    target->data[ target->length ] = 0;

    //return new length of target
    return target->length;
}


/*==========================================================================*/
hzstr_result_t hzstr_cpy(           //copy one string into another
    hzstr_type*         target,     //copy target
    const hzstr_type*   source      //copy source
) {                                 //result of operation

    //local variables
    hzstr_result_t      result;     //allocation result

    //check the type of target string
    if( target->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //check allocation of target string
    result = check_alloc( target, source->length );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

    //copy the source data into the target
    mem_copy(
        target->data,
        source->data,
        ( source->length * sizeof( hzstr_char_t ) )
    );

    //update the length of the target string
    target->length = source->length;

    //null-terminate target string
    target->data[ target->length ] = 0;

    //return new length of target
    return target->length;
}


/*==========================================================================*/
hzstr_result_t hzstr_cpy_cstr(      //copy a C-string into a string
    hzstr_type*         target,     //copy target
    const char*         source      //copy source
) {                                 //result of operation

    //local variables
#ifndef HZSTR_COMPAT_CSTR
    hzstr_index_t       index;      //index
#endif
    hzstr_length_t      length;     //length of string copied
    hzstr_result_t      result;     //allocation result

    //check the type of string
    if( target->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //get the length of the C-string
    length = cstr_len( source );

    //check allocation for this string
    result = check_alloc( target, length );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

#ifdef HZSTR_COMPAT_CSTR

    //copy the C-string (including NULL byte) into the string
    mem_copy( target->data, source, ( length + 1 ) );

#else

    //do not assume a single-byte type
    for( index = 0; index < ( length + 1 ); ++index ) {
        target->data[ index ] = ( hzstr_char_t ) source[ index ];
    }

#endif

    //update the string's length
    target->length = length;

    //return length of string
    return target->length;
}


/*==========================================================================*/
long hzstr_cmp(                     //compare two strings for equality
    const hzstr_type*   string0,    //first string
    const hzstr_type*   string1     //second string
) {                                 //comparison value (0 means equal)

    //local variables
    hzstr_length_t      pos;        //position index

    //initialize position index
    pos = 0;

    //scan both strings
    while( pos < string0->length ) {

        //check the current character
        if( string0->data[ pos ] != string1->data[ pos ] ) {

            //return the difference in these characters
            return string0->data[ pos ] - string1->data[ pos ];
        }

        //advance the position index
        pos += 1;
    }

    //strings appear the be the same
    return 0;
}


/*==========================================================================*/
void hzstr_destroy(                 //destroy a string
    hzstr_type*         string      //the string to destroy
) {

    //check the pointer
    if( string != NULL ) {

        //check for a valid, allocated string
        if( ( string->alloc > 0 ) && ( string->data != NULL ) ) {

            //free the string memory
            mem_free( string->data );
        }

        //free the string object
        mem_free( string );
    }
}


/*==========================================================================*/
hzstr_result_t hzstr_import(        //safely import an untrusted string
    hzstr_type*         string,     //existing string to use for storage
    const char*         source,     //C string to import
    hzstr_length_t      length      //maximum length to import
) {                                 //result of operation

    //local variables
    hzstr_length_t      srclen;     //source string length

    //this can only be done to dynamic strings
    if( string->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //find the length of the source string
    srclen = cstr_len( source );

    //check what was found against the user's limit
    if( srclen > length ) {
        return HZSTR_RSLT_SAFETY;
    }

    //copy the user's C-string into the string
    return hzstr_cpy_cstr( string, source );
}


/*==========================================================================*/
hzstr_result_t hzstr_minimize(      //minimize the memory used for this string
    hzstr_type*         string      //string to minimize
) {                                 //result of operation

    //local variables
    hzstr_char_t*       ptr;        //pointer to newly-allocated string data

    //this can only be done to dynamic strings
    if( string->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    //re-allocate the string down to its current length (+1 for NULL)
    ptr = mem_realloc( string->data, ( string->length + 1 ) );

    //check memory allocation
    if( ptr == NULL ) {
        return HZSTR_RSLT_ALLOC;
    }

    //update string object
    string->alloc = string->length + 1;
    string->data  = ptr;

    //return the new size of the string's allocation
    return string->alloc;
}


/*==========================================================================*/
#ifdef HZSTR_COMPAT_CSTR

hzstr_result_t hzstr_sprintf(       //formatted printing into a string
    hzstr_type*         string,     //target string
    const char*         format,     //string format
    ...                             //format arguments
) {                                 //result of operation

    //local variables
    char                dummy[ 1 ]; //dummy C-string buffer
    hzstr_length_t      length;     //length of printed string
    hzstr_result_t      result;     //allocation result
    va_list             varargs;    //varargs object

    //this can only be done to dynamic strings
    if( string->type != HZSTR_TYPE_DYNAMIC ) {
        return HZSTR_RSLT_TYPE;
    }

    va_start( varargs, format );

    //perform a dummy print to compute the size of the print
    length = vsnprintf( dummy, 1, format, varargs );

    //check the size we would have printed, and allocate, as needed
    result = check_alloc( string, length );
    if( result < HZSTR_RSLT_OK ) {
        return result;
    }

    //print to the string
    length = vsnprintf( string->data, string->alloc, format, varargs );

    va_end( varargs );

    //update the string's length
    string->length = length;

    //return the printed length
    return length;
}

#endif


/*==========================================================================*/
hzstr_index_t hzstr_strchr(         //find first instance of a character
    const hzstr_type*   string,     //string to search
    hzstr_char_t        c           //character to search for
) {                                 //index of character in string (-1=none)

    //local variables
    hzstr_index_t       pos;        //position in string

    //initialize the position
    pos = 0;

    //loop through the hzstring (may be a substring)
    while( pos < string->length ) {

        //check for the character
        if( string->data[ pos ] == c ) {
            return pos;
        }

        //increment the position index
        pos += 1;
    }

    //unable to find character in string
    return HZSTR_INDEX_NONE;
}


/*==========================================================================*/
hzstr_result_t hzstr_tolower(       //transform the string to lowercase
    hzstr_type*         string      //the string to make lowercase
) {                                 //result of operation

    //local variables
    hzstr_index_t       pos;        //position index

    //initialize position index
    pos = 0;

    //loop through the entire string
    while( pos < string->length ) {

        //check for an uppercase letter
        if( ( string->data[ pos ] >= 'A' )
         && ( string->data[ pos ] <= 'Z' ) ) {

            //make it lowercase
            string->data[ pos ] += 'a' - 'A';
        }

        //advance to the next character
        pos += 1;
    }

    //return success
    return HZSTR_RSLT_OK;
}


/*==========================================================================*/
hzstr_result_t hzstr_toupper(       //transform the string to uppercase
    hzstr_type*         string      //the string to make uppercase
) {                                 //result of operation

    //local variables
    hzstr_index_t       pos;        //position index

    //initialize position index
    pos = 0;

    //loop through the entire string
    while( pos < string->length ) {

        //check for a lowercase letter
        if( ( string->data[ pos ] >= 'a' )
         && ( string->data[ pos ] <= 'z' ) ) {

            //make it uppercase
            string->data[ pos ] -= 'a' - 'A';
        }

        //advance to the next character
        pos += 1;
    }

    //return success
    return HZSTR_RSLT_OK;
}


/*==========================================================================*/
hzstr_result_t hzstr_trim(          //trim a string in-place
    hzstr_type*         string      //string to trim
) {                                 //result of operation

    //local variables
    hzstr_length_t      leading;    //amount of leading whitespace
    hzstr_length_t      length;     //length of string after trimming
    hzstr_char_t*       pos;        //position pointer in string data

    //initialize length
    length = string->length;

    //set the position to the last character in the string
    pos = string->data + string->length - 1;

    //loop until the last non-whitespace character is found
    //  note: do not scan past the start of the string
    while( ( *pos <= ' ' ) && ( pos >= string->data ) ) {

        //set all whitespace characters to the NULL byte
        *pos = '\0';

        //decrement length
        length -= 1;

        //move position back one character
        pos -= 1;
    }

    //set the position to the first character in the string
    pos = string->data;

    //set the leading space to zero
    leading = 0;

    //loop until the first non-whitespace character is found
    //  note: do not scan past the end of the string
    while( ( *pos <= ' ' ) && ( *pos != '\0' ) ) {

        //increment the leading space counter
        leading += 1;

        //move the position to the next character
        pos += 1;
    }

    //move all the characters up to the beginning of the string
    while( *pos != '\0' ) {

        //place the current character earlier in the string
        *( pos - leading ) = *pos;

        //move the position to the next character
        pos += 1;
    }

    //set the null pointer in the last position
    *( pos - leading ) = '\0';

    //update the string's length
    string->length = length - leading;

    //return the trimmed length
    return string->length;
}


/*==========================================================================*/
static hzstr_result_t check_alloc(  //check a string's allocation
    hzstr_type*         string,     //the string to check
    hzstr_length_t      length      //the desired length of the string
) {                                 //result of the check

    //local variables
    hzstr_length_t      alloc;      //new allocation of the string data
    hzstr_char_t*       data;       //pointer to allocated string data

    //see if the string needs to be embiggened
    if( string->alloc < ( length + 1 ) ) {

        //adjust the allocated memory for the string
        alloc = str_alloc( &data, string->data, length );

        //check allocation
        if( data == NULL ) {

            //report failure
            return HZSTR_RSLT_ALLOC;
        }

        //update the string's allocated length and string pointer
        string->alloc = alloc;
        string->data  = data;
    }

    //report the allocated length of the string
    return string->alloc;
}


/*==========================================================================*/
static hzstr_length_t cstr_len(     //compute length of C-string
    const char*         cstring     //pointer to C-string
) {                                 //length of C-string

    //local variables
    const char*         start;      //pointer to C-string

    //initialize the pointer to the beginning of the C-string
    start = cstring;

    //scan string for the NULL terminator
    while( *cstring != '\0' ) {

        //advance the C-string pointer
        cstring += 1;
    }

    //return the difference between the pointers
    return cstring - start;
}


/*==========================================================================*/
static hzstr_length_t str_alloc(    //allocate/re-allocate string memory
    hzstr_char_t**      target,     //target string data pointer
    hzstr_char_t*       source,     //optional source string data pointer
    hzstr_length_t      length      //length to allocate
) {                                 //number of bytes actually allocated

    //use a default allocation
    if( length == 0 ) {
        length = HZSTR_CHUNK_SIZE;
    }

    //compute a padded allocation
    else {
        length = ( ( length / HZSTR_CHUNK_SIZE ) + 1 ) * HZSTR_CHUNK_SIZE;
    }

    //check for new allocation
    if( source == NULL ) {

        //allocate the requested memory
        *target = mem_malloc( length * sizeof( hzstr_char_t ) );
    }

    //this is re-allocating an existing string
    else {

        //re-allocate the string
        *target = mem_realloc( source, ( length * sizeof( hzstr_char_t ) ) );
    }

    //return length requested for allocation
    return length;
}

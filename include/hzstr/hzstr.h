/*****************************************************************************
    hzstr.h
    Zac Hester
    2012-10-16

    The Hz string library is an interoperable abstraction for maintaining
    safe, consistent strings in memory without a lot of programmer
    intervention.

    Interface Notes:
        - Functions named hzstr_create* will always (if successful) allocate
            new memory that needs to be released by hzstr_destroy()
        - Parameters to functions marked as const pointers to string objects
            will not be mutated in the function.  All other reference
            parameters may be mutated in the function.  A check is made at
            run time to ensure the string has appropriate attributes.
        - If the user defines the HZSTR_DSBL_CSTR_COMPAT symbol, it is no
            longer assumed that Hz strings are interchangeable with C-strings.
            This can provide some flexibility in using this library with other
            primitive types to represent the individual characters in a
            string (e.g. unicode).  The user may wish to, then, provide an
            equivalent to the hzstr_cstr() if translating back to C-strings is
            necessary.
        - In the above case, the user may specify any primitive character type
            by defining the HZSTR_USER_TYPE symbol.  This type will then be
            used for all string storage and manipulation.

    Example Usage

*****************************************************************************/

#ifndef _HZSTR_H
#define _HZSTR_H

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

//see if the user wishes to override C-string compatibility
#ifndef HZSTR_DSBL_CSTR_COMPAT

//provide a flag to implementations that rely on C-string compatibility
#define HZSTR_COMPAT_CSTR

//represent a string as a C-string
#define hzstr_cstr( _s ) ( ( _s )->data )

#endif  //HZSTR_DSBL_CSTR_COMPAT

//find the length of a string
#define hzstr_length( _s ) ( ( _s )->length )

//performance tuning
#define HZSTR_CHUNK_SIZE ( 32 )     //allocation chunk size (characters)

/*----------------------------------------------------------------------------
Types and Structures
----------------------------------------------------------------------------*/

typedef unsigned short hzstr_length_t;
                                    //length of string or buffer

typedef long hzstr_index_t;         //string indexing type (can be negative)

enum {                              //special index values
    HZSTR_INDEX_NONE    = -1        //index not found (for return values)
};

typedef long hzstr_result_t;        //result of string operation

enum {                              //operation results (+ indicates length)
    HZSTR_RSLT_OK       = 0,        //successful operation
    HZSTR_RSLT_EMPTY    = 0,        //successful operation with zero length
    HZSTR_RSLT_USAGE    = -99,      //used incorrectly (invalid parameters)
    HZSTR_RSLT_TYPE     = -98,      //string type incorrectly accessed
    HZSTR_RSLT_SAFETY   = -97,      //operation failed safety check
    HZSTR_RSLT_ALLOC    = -89       //failed to allocate memory
};

typedef unsigned short hzstr_type_t;//string access types type

enum {                              //string access types
    HZSTR_TYPE_CONST,               //string is a memory-bound constant
    HZSTR_TYPE_SUBSTR,              //string is an immutable substring
    HZSTR_TYPE_DYNAMIC,             //normal, dynamic string

    HZSTR_NUM_TYPES                 //number of available string types
};

typedef unsigned short hzstr_flags_t;
                                    //object behavior flag type

enum {                              //string behavior flags
    HZSTR_FLG_WRITABLE = ( 1 << 0 ) //the string can accept mutating access
};

#ifdef HZSTR_USER_TYPE
typedef HZSTR_USER_TYPE hzstr_char_t;
                                    //internal type alias to character type
#else
typedef char hzstr_char_t;          //internal type alias to character type
#endif

typedef struct hzstr_s {            //string data type
    hzstr_type_t        type;       //type of string
    hzstr_flags_t       flags;      //object behavior flags
    hzstr_length_t      alloc;      //number of bytes allocated
    hzstr_length_t      length;     //length of string
    hzstr_char_t*       data;       //pointer to array of characters in string
} hzstr_type;

/*----------------------------------------------------------------------------
Memory Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Interface Prototypes
----------------------------------------------------------------------------*/

hzstr_result_t hzstr_append(        //append character onto end of string
    hzstr_type*         string,     //target string
    char                c           //character to append
);                                  //result of appending

hzstr_type* hzstr_create(           //create a new string
    hzstr_length_t      length      //initial length to create (0 for default)
);                                  //pointer to new string

#ifdef HZSTR_COMPAT_CSTR
hzstr_type* hzstr_create_ccstr(     //create new string using a const C-string
    const char*         cstring     //C string
);                                  //pointer to new string
#endif

hzstr_type* hzstr_create_cstr(      //create new string from a C-string
    const char*         cstring     //C string
);                                  //pointer to new string

hzstr_type* hzstr_create_substr(    //create a substring of another string
    const hzstr_type*   source,     //source string
    hzstr_index_t       offset,     //beginning offset in string
    hzstr_length_t      length      //length of substring
);                                  //pointer to new substring

hzstr_result_t hzstr_cat(           //concatenate two strings
    hzstr_type*         target,     //concatenation target
    const hzstr_type*   source      //concatenation source
);                                  //result of operation

hzstr_result_t hzstr_cat_cstr(      //concatenate a C-string onto a string
    hzstr_type*         target,     //concatenation target
    const char*         source      //concatenation source
);                                  //result of operation

hzstr_result_t hzstr_cpy(           //copy one string into another
    hzstr_type*         target,     //copy target
    const hzstr_type*   source      //copy source
);                                  //result of operation

hzstr_result_t hzstr_cpy_cstr(      //copy a C-string into a string
    hzstr_type*         target,     //copy target
    const char*         source      //copy source
);                                  //result of operation

long hzstr_cmp(                     //compare two strings for equality
    const hzstr_type*   string0,    //first string
    const hzstr_type*   string1     //second string
);                                  //comparison value (0 means equal)

void hzstr_destroy(                 //destroy a string
    hzstr_type*         string      //the string to destroy
);

hzstr_result_t hzstr_import(        //safely import an untrusted string
    hzstr_type*         string,     //existing string to use for storage
    const char*         source,     //C string to import
    hzstr_length_t      length      //maximum length to import
);                                  //result of operation

hzstr_result_t hzstr_minimize(      //minimize the memory used for this string
    hzstr_type*         string      //string to minimize
);                                  //result of operation

#ifdef HZSTR_COMPAT_CSTR
hzstr_result_t hzstr_sprintf(       //formatted printing into a string
    hzstr_type*         string,     //target string
    const char*         format,     //string format
    ...                             //format arguments
);                                  //result of operation
#endif

hzstr_index_t hzstr_strchr(         //find first instance of a character
    const hzstr_type*   string,     //string to search
    char                c           //character to search for
);                                  //index of character in string (-1=none)

hzstr_result_t hzstr_tolower(       //transform the string to lowercase
    hzstr_type*         string      //the string to make lowercase
);                                  //result of operation

hzstr_result_t hzstr_toupper(       //transform the string to uppercase
    hzstr_type*         string      //the string to make uppercase
);                                  //result of operation

hzstr_result_t hzstr_trim(          //trim a string in-place
    hzstr_type*         string      //string to trim
);                                  //result of operation

#endif /* _HZSTR_H */

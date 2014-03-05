/*****************************************************************************
    memory.h
    Zac Hester
    2012-10-17

    Memory management interface to provide testable interaction with memory
    allocation on the host platform.

    Example Usage

*****************************************************************************/

#ifndef _MEMORY_H
#define _MEMORY_H

/*----------------------------------------------------------------------------
Includes
----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------
Macros
----------------------------------------------------------------------------*/

#ifdef UNIT_TEST

//provide equivalent prototypes to link against test stubs
void* mem_calloc(
    size_t              num,
    size_t              size
);

void mem_free(
    void*               ptr
);

void* mem_malloc(
    size_t              size
);

void* mem_realloc(
    void*               ptr,
    size_t              size
);

#else

//use standard heap operations
#define mem_calloc      calloc
#define mem_free        free
#define mem_malloc      malloc
#define mem_realloc     realloc

#endif

//most implementations use memcpy to copy bytes because compilers like it
#define mem_copy        memcpy

/*----------------------------------------------------------------------------
Types and Structures
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Memory Constants
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
Interface Prototypes
----------------------------------------------------------------------------*/


#endif /* _MEMORY_H */

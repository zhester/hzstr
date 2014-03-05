hzstr
=====

Small String Library for Personal Projects

Description
-----------

This is a small library I use in my personal projects to consistently handle
string manipulation.  It is completely tested.  It includes a few things that
normal string libraries leave out (e.g. letter case conversions).

This is not intended to be a complete replacement for the standard string
functions.  And, it is intended to be used in conjunction with other string
functions/libraries.  The primary "feature" of the library is to automate
some of the error-prone parts of writing software that needs safe string
handling (especially allocation and re-allocation of memory for character
data).  Minor improvements in string handling performance is gained at the
expense of additional memory used to keep track of string object state between
function calls;  In a lot of cases, user code has to store the same amount of
state data to safely handle strings without the library, so the amount of
increased memory usage may be negligable for a high-quality application.

hzstr is meant to be used as a statically-linked library for another project.

Building
--------

_coming soon_

Testing
-------

See: test/README.md

Using
-----

_coming soon_

Function Reference
------------------

_coming soon_

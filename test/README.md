Unit Tests
==========

This directory contains the unit test framework and each module's unit test
code.  The `test` directory contains the framework verification test which is
a one-off sanity check of the framework against the host's platform.

Other directories should contain the test code necessary to fully exercise the
unit under test, and help report those findings in a clear, concise record.
The code under test is compiled directly from the file in the project's `src`
directory.

Requirements
------------

The test framework currently takes advantage of features unique to `gcc` and
`gcov`.  The build scripts are written for `gmake`.  Otherwise, no outside
libraries or utilities are necessary.  A transition to the LLVM environment
will eventually replace the GNU toolchain.  There is the possibility that
future automation will be implemented using Python.

Execution
---------

  make cov

Results
-------

The functional test is performed, and the per-step results are stored in
`build/test.log`.  The overall test results are immediately available on the
console.  This is indicated by either "TEST PASSED" or "TEST FAILED".

The coverage analysis is performed using the functional test code running
against a special build of the module with coverage instrumentation.  The
results are displayed on the console.  Each summary statistic should report
"100.00%" if full coverage is achieved.

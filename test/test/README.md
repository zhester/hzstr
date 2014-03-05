Test Framework Verification
===========================

This test verifies that the framework is operating correctly on the target
platform.

Execution
---------

  make
  make run

Results
-------

Unlike other unit tests, this test sends results to stdout.  This test *WILL
FAIL* as it needs to verify it can correctly handle negative assertions and
failed assertions.  The steps that fail should be:

  1.1.1.3: Boolean false checked against an integer that is not 1.
  1.1.2.2: Integer comparison of dissimilar values.
  1.1.3.2: Double range verified a failure outside specified range.
  1.1.4.2: Double tolerance verified a failure outside specified tolerance.
  1.1.5.2: String comparison of dissimilar values.
  1.1.6.2: Memory comparison of dissimilar contents.

If these are the only reported failures (along with the global "TEST FAILED"),
the framework is operating correctly.

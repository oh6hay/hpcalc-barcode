hpcalc-barcode
=============

Code39 and interleaved 2 of 5 generators for the HP 50g.

A quick & dirty code39 barcode generator I wrote the other night. 
Usage: place a string on the stack and call the 'bcgen' or 'i25gen'
binaries.

Notes on the code39 generator,
* Only the first 10 characters are rendered in the barcode, due to the
HP 82240 printer only printing max 166 pixels wide GROBs and 10
characters + 2 terminating chars results in 12*13 = 157 pixels.
* Makefile borrowed from the hpgcc-2 source tree's test directory
* Have the hpobject library installed from hpcalc.org according to the
instructions

Notes on the i25gen generator,
* the program expects a string on stack with an even number of
integers in it

To print the result, you may use something like this to invoke instead
of the bcgen binary: << DUP bcgen PR1 DROP >>


hpcalc-code39
=============

Code39 barcode generator for the HP 50g calculator

A quick & dirty code39 barcode generator I wrote the other night. 
Usage: place a string on the stack and call the 'bcgen' binary.

Notes,
* Only the first 10 characters are rendered in the barcode, due to the
HP 82240 printer only printing max 166 pixels wide GROBs and 10
characters + 2 terminating chars results in 12*13 = 157 pixels.
* Makefile borrowed from the hpgcc-2 source tree's test directory
* Have the hpobject library installed from hpcalc.org according to the
instructions

To print the result, you may use something like this to invoke instead
of the bcgen binary: << DUP bcgen PR1 DROP >>


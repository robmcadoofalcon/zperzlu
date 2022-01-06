puzzler
=======

This uses SIMD instructions to detect and shuffle ASCII characters. Currently
this doesn't shuffle characters around an apostrophe, which means words like
"o'clock" and singular possessives will be easy enough to spot. There's nothing
too interesting about it, since it just loads the characters 16 at a time into
a register, lower-cases each character and compares its value to the upper and
lower bounds of the lower-case ASCII alphabet range, which produces a mask of
characters to shuffle vs characters to leave in place. We rely on a static
lookup table that is indexed in by the mask of the detected ASCII characters to
be shuffled. For example:

The letters "abcd'" would produce a mask 11110, which is the integer value of
an index into the lookup table to produce a shuffle mask like {3,2,1,0,4},
which specifies the positions each character should move to. Then we apply the
shuffle and copy the results back to main memory. The shuffle mask for each of
the 16 bit mask combinations has been produced ahead of time.

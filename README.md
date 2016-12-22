# L_AC
This project realise a compiler of language L_AC

## The order of basic functions (fonction de base)
```
0 lit (not available in interpreter mode)
1 str (not available in interpreter mode)
2 fin (not available in interpreter mode)
3 .
4 +
5 -
6 *
7 = (comparison)
8 dup
9 drop
10 swap
11 count
12 type
13 if (not available in interpreter mode)
14 else (not available in interpreter mode)
15 then (not available in interpreter mode, no processor implementation)
16 / (division)
20 && (optional functions from now on)
21 ||
22 !
23 inferior or equal
24 defer
25 recurse (need no processor implementation)
26 '
27 is
28 calculate
29 catenate
30 cr (generate a string of \n to put in stack)
```
##Follower

- Hugo
- Nicolas
## Document structure
## Explanation of basic functions
### dup
In LAC, the parameters number and output number of <code>dup</code> are 0. This means the processor stop the parameter type checking, the checking is implemented by function itself. Because this function will change the type stack.

/*************
*
* slreadme.txt
*
* Package name: SL - String Library
*
* Purpose: 	SL is intended to provide simple library routines
*		that are useful for string manipulations, data
*		conversion, etc.  This library replaces the need
*		for including standard C libraries such as
*		stdio.h or string.h.
*
* Copyright: 	© 2008 Sierra Wireless, all rights reserved
*
**************/

1. Machine Dependent Aspect of SL Package.

1.1.1. Variable Argument List Implementation in SL Package.

In SL package, there exist two functions, which takes a variable argument list, slsscanf() and slvsprintf().  The variable argument list allows a function to take a variable number of arguments.  The variable argument list is implemented by first pointing an argument pointer, a character pointer in SL implementation, to the last fixed argument.  According to the ARM-Thumb Procedure Call Standard (ATPCS), the first four arguments are passed along registers r0 to r3 and the rest on the stack.  By pointing an argument pointer to the last fixed argument, assuming that the fixed argument is in the register, the compiler is forced to push the first four arguments on to the stack because registers do not have addresses.  This action creates a contiguous, stacked argument list.  Calling slva_start() points the argument pointer at the first variable argument.  Subsequent access to the arguments is achieved by de-referencing the pointer to access the argument and then incrementing the argument pointer by the size of the argument in bytes to point at the next variable argument.  

1.1.2. Variable Argument List Portability Issues

In ATPCS, arguments less than 4 bytes are expanded to occupy 4 bytes in memory.  Because the current variable argument list implementation is optimized for slsscanf() and slvsprintf() on ARM machines, the argument pointer is always incremented by 4 bytes to access the next variable argument.  The variable arguments that slvsprintf() and slsscanf() take include characters, signed/unsigned short integers, signed/unsigned integers, unsigned/signed long integers and pointers.  Only characters and short integers are not 4 bytes in size for the current Bandit ARM machine.  In SL package, the argument pointer is incremented by 4 bytes instead of 1 byte after accessing a character argument by using sizeof(int) instead of sizeof(char) to determine the number of bytes to increment.  This is similar for short integers too.  This architecture is where different platform utilization of the SL package can be potentially problematic.  For example, on some machines the data type int might only be 2 bytes instead of 4 bytes.  In addition, on some machines, arguments less than 4 bytes might not be expanded to occupy 4 bytes in memory.  Furthermore, the argument pointer is incremented to access the next variable argument, which assumes that the machine has a descending instead of an ascending stack.  Therefore, users must port the variable argument list implementation to the specific machine to be able to use slsscanf() and slvsprintf() properly.
 
1.2. Word Alignment Requirements with Memory Copy

The current memory copy function, slmemcpy(), is optimized to copy word (32 bit in length) by word which speeds up memory copy operation.  However the addresses of words to be copied to and from have to be word-aligned, where the address is divisible by four, or else the copy operation would be unpredictable.  This word alignment requirement is a feature of the ARM architecture.  If the address is not word-aligned, slmemcpy() will copy byte by byte instead.  However, this word alignment requirement might become a portability issue if future embedded products' processors move to 64 bits operations with 2 word alignments requirement.  This situation is highly unlikely to occur in the near future.


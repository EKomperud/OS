#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <unistd.h>
#include <fcntl.h>

/*********************************************************************
 *
 * These C functions use patterns and functionality often found in
 * operating system code. Your job is to implement them. Of course you
 * should write test cases. However, do not hand in your test cases
 * and (especially) do not hand in a main() function since it will
 * interfere with our tester.
 *
 * Additional requirements on all functions you write:
 *
 * - you may not refer to any global variables
 *
 * - you may not call any functions except those specifically
 *   permitted in the specification
 *
 * - your code must compile successfully on CADE lab Linux
 *   machines when using:
 *
 * /usr/bin/gcc -O2 -fmessage-length=0 -pedantic-errors -std=c99 -Werror -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-include-dirs -Wno-unused-parameter -Wshadow -Wuninitialized -Wold-style-definition -c assign1.c 
 *
 * NOTE 1: Some of the specifications below are specific to 64-bit
 * machines, such as those found in the CADE lab.  If you choose to
 * develop on 32-bit machines, some quantities (the size of an
 * unsigned long and the size of a pointer) will change. Since we will
 * be grading on 64-bit machines, you must make sure your code works
 * there.
 *
 * NOTE 2: You should not need to include any additional header files,
 * but you may do so if you feel that it helps.
 *
 * HANDIN: submit your finished file, still called assign1.c, in Canvas
 *
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 *
 *********************************************************************/

unsigned long byte_sort (unsigned long arg)
{
  unsigned long arg2 = arg;
  unsigned long *ret = &arg2;
  unsigned char *bytes = (unsigned char*)ret;
  int i;
  for (i = 1; i < 8; i++)
  {
  	int j;
  	for (j = i; j > 0; j--)
  	{
  	  if (bytes[j - 1] > bytes[j])
  	  {
		char s = bytes[j-1];
		bytes[j-1] = bytes[j];
		bytes[j] = s;  	  	
  	  }
        }
  }
  return *ret;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
  unsigned char nibbles[16];
  unsigned long copier = 0x000000000000000f;
  unsigned long arg2 = arg;
  int i;
  for (i = 0; i < 16; i++)
  {
  	nibbles[i] = 0;
  	nibbles[i] = copier & arg2;
  	arg2 = arg2 >> 4;
  }

  for (i = 1; i < 16; i++)
  {
  	int j;
  	for (j = i; j > 0; j--)
  	{
  	  if (nibbles[j - 1] > nibbles[j])
  	  {
		char s = nibbles[j-1];
		nibbles[j-1] = nibbles[j];
		nibbles[j] = s;  
  	  }
  	}
  }

  unsigned long ret = 0;
  copier = 0;
  for (i = 0; i < 16; i++)
  {
  	unsigned long nibble = nibbles[i];
  	nibble = nibble << (4 * i);
  	ret = ret | nibble;
  }
  return ret;
}

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *  
 *********************************************************************/

struct elt {
  char val;
  struct elt *link;
};

struct elt *name_list (void)
{
  const char *name = "Redwood";
  struct elt *letters;
  struct elt *ret;
  letters = ret = (struct elt *)malloc(sizeof(struct elt));
  if (name[0] != '\0')
  	letters->val = name[0];
  struct elt *last = letters;
  int i = 1;
  while (name[i] != '\0')
  {
  	letters = (struct elt *)malloc(sizeof(struct elt));

  	if (letters ==  NULL)
  	{
  	  letters = ret;
  	  struct elt *next = letters->link;
  	  while (1)
  	  {
  	  	free(letters);
  	  	letters = next;
  	  	if (next->link != NULL)
  	  		next = next->link;
  	  	else
  	  		return NULL;
  	  }
  	}
  	else
  	{
  	  letters->val = name[i];
  	  letters->link = NULL;
  	  last->link = letters;
  	  last = letters;
  	  i++;
  	}
  }

  return ret;
}

/*********************************************************************
 *
 * convert()
 *
 * specification: depending on the value of "mode", print "value" as
 * octal, binary, or hexidecimal to stdout, followed by a newline
 * character
 *
 * extra requirement 1: output must be via putc()
 *
 * extra requirement 2: print nothing if "mode" is not one of OCT,
 * BIN, or HEX
 *
 * extra requirement 3: all leading/trailing zeros should be printed
 *
 * EXAMPLE: convert (HEX, 0xdeadbeef) should print
 * "00000000deadbeef\n" (including the newline character but not
 * including the quotes)
 *
 *********************************************************************/

enum format_t {
  OCT = 66, BIN, HEX
};

static unsigned long exponentiate(unsigned long a, unsigned long b)
{
  unsigned int i;
  unsigned long ret = 1;
  for (i = 0; i < b; i++)
  	ret *= a;
  return ret;
}

void convert (enum format_t mode, unsigned long value)
{
  unsigned long val = value;
  unsigned long digitValue;
  unsigned long baseValue;
  unsigned char digitChar;
  int i;
  switch (mode)
  {
	case BIN:
	  i = 63;
	  baseValue = 2;
	break;

	case HEX:
	  i = 15;
	  baseValue = 16;
	break;

	case OCT:
	  i = 21;
	  baseValue = 8;
	break;

	default:
	  return;
	break;
  }
  for (; i >= 0; i--)
  {
  	digitValue = exponentiate(baseValue, i);
  	digitChar = '0';
  	while (val - digitValue <= val)
  	{
  		digitChar += 1;
  		if (digitChar == ':')
	  	  digitChar = 'a';
  		val -= digitValue;
  	}
  	putc(digitChar, stdout);
  }
  putc('\n',stdout);
  return;
}

/*********************************************************************
 *
 * draw_me()
 *
 * this function creates a file called me.txt which contains an ASCII-art
 * picture of you (it does not need to be very big). the file must (pointlessly,
 * since it does not contain executable content) be marked as executable.
 * 
 * extra requirement 1: you may only call the function syscall() (type "man
 * syscall" to see what this does)
 *
 * extra requirement 2: you must ensure that every system call succeeds; if any
 * fails, you must clean up the system state (closing any open files, deleting
 * any files created in the file system, etc.) such that no trash is left
 * sitting around
 *
 * you might be wondering how to learn what system calls to use and what
 * arguments they expect. one way is to look at a web page like this one:
 * http://blog.rchapman.org/post/36801038863/linux-system-call-table-for-x86-64
 * another thing you can do is write some C code that uses standard I/O
 * functions to draw the picture and mark it as executable, compile the program
 * statically (e.g. "gcc foo.c -O -static -o foo"), and then disassemble it
 * ("objdump -d foo") and look at how the system calls are invoked, then do
 * the same thing manually using syscall()
 *
 *********************************************************************/

void draw_me (void)
{
	int fd = syscall(SYS_open, "me.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd < 0)
	{
		syscall(SYS_rmdir, "me.txt");
		return;
	}
	ssize_t rets[16];
	rets[0] = syscall(SYS_write, fd, "0123456789012345678901234567890123456789\n",41);
	rets[1] = syscall(SYS_write, fd, "0                _____                 9\n",41);
	rets[2] = syscall(SYS_write, fd, "0               |     |                9\n",41);
	rets[3] = syscall(SYS_write, fd, "0           ____|_____|____            9\n",41);
	rets[4] = syscall(SYS_write, fd, "0             / _    _  \\              9\n",41);
	rets[5] = syscall(SYS_write, fd, "0            |< O >< O > |             9\n",41);
	rets[6] = syscall(SYS_write, fd, "0            |    ~~~    |             9\n",41);
	rets[7] = syscall(SYS_write, fd, "0             \\  \\__/   /  ||          9\n",41);
	rets[8] = syscall(SYS_write, fd, "0              ---------   O           9\n",41);
	rets[9] = syscall(SYS_write, fd, "0                  |      /            9\n",41);
	rets[10] = syscall(SYS_write, fd, "0         -C-------|_____/             9\n",41);
	rets[11] = syscall(SYS_write, fd, "0                  |                   9\n",41);
	rets[12] = syscall(SYS_write, fd, "0                  |                   9\n",41);
	rets[13] = syscall(SYS_write, fd, "0                 /-\\                  9\n",41);
	rets[14] = syscall(SYS_write, fd, "0                /   \\                 9\n",41);	
	rets[15] = syscall(SYS_write, fd, "0123456789012345678901234567890123456789\n",41);
	int i;
	for (i = 0; i < 16; i++)
	{
	  if (rets[i] == -1)
	    syscall(SYS_rmdir, "me.txt");
	}
	syscall(SYS_close, fd);
	return;
}

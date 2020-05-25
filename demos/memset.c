/* Set a block of memory to some byte value.
   For Intel 80x86, x>=3.
   Copyright (C) 1991, 1992, 1993, 1997, 1998 Free Software Foundation, Inc.
   Contributed by Torbjorn Granlund (tege@sics.se).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <string.h>

#ifdef __i386__

#if defined(__GNUC__) 

#undef memset

#define op_t unsigned int
#define OPSIZ 4


void *
memset (void *dstpp, int c, size_t len)
{
  int d0;
  unsigned long int dstp = (unsigned long int) dstpp;

  /* This explicit register allocation
     improves code very much indeed.  */
  register op_t x asm("ax");

  x = (unsigned char) c;

  /* Clear the direction flag, so filling will move forward.  */
  asm volatile("cld");

  /* This threshold value is optimal.  */
  if (len >= 12)
    {
      /* Fill X with four copies of the char we want to fill with.  */
      x |= (x << 8);
      x |= (x << 16);

      /* Adjust LEN for the bytes handled in the first loop.  */
      len -= (-dstp) % OPSIZ;

      /* There are at least some bytes to set.
	 No need to test for LEN == 0 in this alignment loop.  */

      /* Fill bytes until DSTP is aligned on a longword boundary.  */
      asm volatile("rep\n"
		   "stosb" /* %0, %2, %3 */ :
		   "=D" (dstp), "=c" (d0) :
		   "0" (dstp), "1" ((-dstp) % OPSIZ), "a" (x) :
		   "memory");

      /* Fill longwords.  */
      asm volatile("rep\n"
		   "stosl" /* %0, %2, %3 */ :
		   "=D" (dstp), "=c" (d0) :
		   "0" (dstp), "1" (len / OPSIZ), "a" (x) :
		   "memory");
      len %= OPSIZ;
    }

  /* Write the last few bytes.  */
  asm volatile("rep\n"
	       "stosb" /* %0, %2, %3 */ :
	       "=D" (dstp), "=c" (d0) :
	       "0" (dstp), "1" (len), "a" (x) :
	       "memory");

  return dstpp;
}

#else /* __GNUC__ */
#include <sysdeps/generic/memset.c>
#endif

#endif /* __i386__ */

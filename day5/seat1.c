#include <stdio.h>

/* The file format is pretty straight foward. It will always have
   lg 128 + lg 8 = 7 + 3 = 10 characters.

   One nice thing about this problem is that we don't even have to store the
   whole list of boarding passes, just have to keep track of the highest ID.

   An approach to this problem might be to do a manual binary search and change
   the upper and lower bounds each recuse until they are the same. Do this for
   both rows and columns.

   Another - I think simpler - approach is to recognize that the row is simply
   encoded as a 7-bit integer. F = 0, B = 1. Similarly, columns are a 3 bit
   number where L = 0 and R = 1.

   What we're actually reading when we see the strings is

   FBFBBFFRLR
   0101100101

   This is the example in the problem set up.

   Row          Col
   FBFBBFF      RLR
   0101100      101
   44 (base-10) 5 (base-10)

   The algorithm given by the problem setup for computing the id is row * 8 +
   col. But we don't even really have to do the math, because we can take
   advantage of bit shifting. When shifting bits from LSB to MSB, each shift is
   a multiplication of 2, so if we need to multiply by 8, then lg 8 = 4, the
   bits are shifted 4 times.

   In the above if we have row 0101100 as a separate integer and column 101 as
   another integer, we can obtain the ID as (0101100 << 4) | 101.

   That is, if we're treating the rows and columns as separate variables. But to
   solve this problem, we don't even need to do that, because we are actually
   seeing in the string encoding is the ID, just obfuscated a bit by the F/B/R/L
   encoding.

   Thus, an algorithm for decoding the IDs from the file just needs to do a
   little bit shifting. And since we have a total of 10 bits to be concerned
   with, anything 2 bytes or larger will do (we don't even have to worry about
   sign bits).

   We thus take a really straightforward approach of reading from stdin,
   computing a 16-bit word based on decoding the ascii string, and keeping track
   of the largest integer decoded so far.
*/

int decode_id(char *buffer) {
  int decoded_id = 0;

  for (char *ptr = buffer; *ptr; ptr++) {
    switch(*ptr) {
      /* F and L encode 0. */
    case 'F':
    case 'L':
      decoded_id <<= 1;
      break;
      /* B and R encode 1 */
    case 'B':
    case 'R':
      decoded_id = (decoded_id << 1) | 0x00000001;
      break;
    default:
      /* ignore everything else */
      break;
    }
  }

  return decoded_id;
}

int main(int argc, char** argv) {
  int max_id = 0;

  char buffer[12];
  while (fgets(buffer, 12, stdin) != NULL) {
    int current_id = decode_id(buffer);

    if (current_id > max_id) {
      max_id = current_id;
    }
  }

  printf("Max ID: %d\n", max_id);

  return 0;
}

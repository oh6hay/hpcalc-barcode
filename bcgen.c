#include <hpgcc49.h>
#include "hpobjects.h"

#define HEIGHT 16

/**
 * The 43 characters in the code 39 spec. The integer constants'
 * binary representations define each character in a left-to-right 
 * manner, with 1 meaning black and 0 white.
 * Barcode characters are exactly 12 bits (vertical lines) wide. The
 * code 39 barcode doesn't require strictly 1:2 ratio of narrow to
 * wide bar widths, but it's way more simple to have them so.
 */
static int bars[] = {
0xa6d, //'0'
0xd2b, //'1'
0xb2b, //'2'
0xd95, //'3'
0xa6b, //'4'
0xd35, //'5'
0xb35, //'6'
0xa5b, //'7'
0xd2d, //'8'
0xb2d, //'9'
0xd4b, //'A'
0xb4b, //'B'
0xda5, //'C'
0xacb, //'D'
0xd65, //'E'
0xb65, //'F'
0xa9b, //'G'
0xd4d, //'H'
0xb4d, //'I'
0xacd, //'J'
0xd53, //'K'
0xb53, //'L'
0xda9, //'M'
0xad3, //'N'
0xd69, //'O'
0xb69, //'P'
0xab3, //'Q'
0xd59, //'R'
0xb59, //'S'
0xad9, //'T'
0xcab, //'U'
0x9ab, //'V'
0xcd5, //'W'
0x96b, //'X'
0xcb5, //'Y'
0x9b5, //'Z'
0x95b, //'-'
0xcad, //'.'
0x9ad, //' '
0x925, //'$'
0x929, //'/'
0x949, //'+'
0xa49, //'%'
0x96d}; //'*'

static char *keys = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%*";

/**
 * Finds the character bar representation
 */
int findBarValue(char key) {
  int i;
  for (i = 0 ; i < 43 ; i++) {
    if (key == keys[i]) {
      return bars[i];
    }
  }
  return 0x9ad; // return a space for unknown characters
}

/**
 * Renders a character to the grob, starting from the horizontal
 * position startPos. A trailing white vertical line is rendered
 * after the character.
 */
void encodeChar(int startPos, int c, SatAddr grob) {
  int i = 12;
  int x, y;
  bool val = 0;

  for (i = 0 ; i < 12 ; i++) {
    x = startPos + 11 - i; // render from right to left
    val = c & (1 << i);  // check bit at position i
    for (y = 0 ; y < HEIGHT ; y++) {
      GROBsetPixel(grob, y, x, val);
    }
  }
  // trailing whitespace
  for (y = 0 ; y < HEIGHT ; y++) {
    GROBsetPixel(grob, y, startPos+12, 0);
  }
}

int main(void) {
  char *msg;
  SatAddr grob = NULL;
  int len = 0;
  int currentChar = 0;

  msg = sat_stack_pop_string_alloc();
  if (!msg) {
    sat_stack_push_string("error: string expected");
    sat_push_real(1);
    return 0;
  }

  len = strlen(msg);
  if (len > 10) {
    len = 10;
  }

  grob = makeGROB(16, 13 * (2 + len)); // 2 terminating chars
  
  // start terminator or '*'
  encodeChar(0, 0x96d, grob);

  // encode chars
  for (currentChar = 0 ; currentChar < len ; currentChar++) {
    int c = findBarValue(msg[currentChar]);
    encodeChar(13 * (currentChar + 1), c, grob);
  }
  
  // end terminator, '*'
  encodeChar(13 * (1 + len), 0x96d, grob);
  
  STACKpush(grob);

  return 0;
}

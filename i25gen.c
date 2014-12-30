#include <hpgcc49.h>
#include "hpobjects.h"

#define HEIGHT 16

static bool digits[][5] = {
// 1 2 4 7 0
  {0,0,1,1,0}, // 0
  {1,0,0,0,1}, // 1
  {0,1,0,0,1}, // 2
  {1,1,0,0,0}, // 3
  {0,0,1,0,1}, // 4
  {1,0,1,0,0}, // 5
  {0,1,1,0,0}, // 6
  {0,0,0,1,1}, // 7
  {1,0,0,1,0}, // 8
  {0,1,0,1,0}  // 9
};

void drawVerticalLine(int x, bool val, SatAddr grob) {
  int y;
  for (y=0 ; y<HEIGHT ; y++) {
      GROBsetPixel(grob, y, x, val);
  }
}

// each pair is exactly 14 pixels wide -- 2 wide 3 narrow for each char.
void encodePair(char a, char b, int xpos, SatAddr grob) {
  int x = xpos;
  int i;
  int aidx = (int)a - (int)'0';
  int bidx = (int)b - (int)'0';
  if (aidx < 0) aidx = 0;
  if (aidx > 9) aidx = 9;
  if (bidx < 0) bidx = 0;
  if (bidx > 9) bidx = 9;
  for (i=0 ; i<5 ; i++) {
    drawVerticalLine(x++, 1, grob);
    if (digits[aidx][i]) { // thick black line
      drawVerticalLine(x++, 1, grob);
      drawVerticalLine(x++, 1, grob);
    }
    drawVerticalLine(x++, 0, grob);
    if (digits[bidx][i]) { // thick white line
      drawVerticalLine(x++, 0, grob);
      drawVerticalLine(x++, 0, grob);
    }
  }
}

int main(void) {
  char *msg;
  SatAddr grob = NULL;
  int totalWidth;
  int len = 0;
  int i = 0;
  char a, b;

  msg = sat_stack_pop_string_alloc();
  if (!msg) {
    sat_stack_push_string("error: string of even length expected");
    sat_push_real(1);
    return 0;
  }

  len = strlen(msg);
  if (len%2) {
    len--;
  }

  totalWidth = len * 9 + 9;
  grob = makeGROB(16, totalWidth); // 2 terminating chars

  // start code
  drawVerticalLine(0, 1, grob);
  drawVerticalLine(1, 0, grob);
  drawVerticalLine(2, 1, grob);
  drawVerticalLine(3, 0, grob);

  for (i=0 ; i < len ; i+=2) {
    a = msg[i];
    b = msg[i+1];
    encodePair(a, b, 4 + i*9, grob);
  }

  // end code
  drawVerticalLine(totalWidth-5, 1, grob);
  drawVerticalLine(totalWidth-4, 0, grob);
  drawVerticalLine(totalWidth-3, 1, grob);
  drawVerticalLine(totalWidth-2, 1, grob);
  drawVerticalLine(totalWidth-1, 1, grob);
  
  STACKpush(grob);

  return 0;
}

#include <pebble.h>
#include "TimeStyles.h"

void graphics_draw_line2(GContext *ctx, GPoint p0, GPoint p1, int8_t width);
void draw_line_fallback ( GContext * ctx, GPoint p1, GPoint p2, int width );
void draw_digit ( GContext * ctx, GRect box, int digit );

// Draw time as HH:MM
void draw_time_style_24hrs ( GContext * ctx, GRect bounds, struct tm * tick_time ) {
  char buffer[] = "00:00";
  int middle_width = 15;
  int digit_width = ( bounds.size.w - middle_width ) / 4;

  strftime ( buffer, sizeof( buffer ), "%H:%M", tick_time );

  for ( uint i = 0; i < sizeof ( buffer ); i++ ) {
    if ( i == 2 ) {
      draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * i, bounds.origin.y, middle_width, bounds.size.h ), buffer[i] );
    } else if ( i > 2 ) {
      draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * i - ( digit_width - middle_width ), bounds.origin.y, digit_width, bounds.size.h ), buffer[i] );
    } else {
      draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * i, bounds.origin.y, digit_width, bounds.size.h ), buffer[i] );
    }
  }
}

// Draw time as HH:MM AM|PM
void draw_time_style_12hrs ( GContext * ctx, GRect bounds, struct tm * tick_time ) {
  char mins[]  = "00";
  char hours[] = "00";
  char amORpm [] = "XX";

  int digit_width = bounds.size.w / 4;
  int mins_height = bounds.size.h / 2 - 5;

  strftime ( hours, sizeof( hours ), "%I", tick_time );
  strftime ( mins , sizeof( mins  ), "%M", tick_time );
  strftime ( amORpm , sizeof( amORpm  ), "%p", tick_time );

  for ( uint a = 0; a < sizeof ( hours ); a++ ) {
    draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * a, bounds.origin.y, digit_width, bounds.size.h ), hours[a] );
  }

  for ( uint b = 2; b < sizeof ( mins ) + 2; b++ ) {
    draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * b, bounds.origin.y + 5, digit_width, mins_height ), mins[b - 2] );
  }

  for ( uint c = 2; c < sizeof ( amORpm ) + 2; c++ ) {
    draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * c, bounds.origin.y + 10 + mins_height, digit_width, mins_height ), amORpm[c - 2] );
  }
}


void draw_digit ( GContext * ctx, GRect box, int digit ) {
  GPoint p1, p2;
  int line_width = ( 25 * box.size.w ) / 100;

  switch ( digit ) {
    case '0':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '1':
      p1.x = box.origin.x + box.size.w / 2;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + box.size.w / 2;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '2':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '3':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '4':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '5':
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '6':
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '7':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '8':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case '9':
      p1.x = box.origin.x + box.size.w / 4;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + box.size.w / 4;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + box.size.h / 10;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'A': // A
      p1.x = box.origin.x + ( box.size.w / 4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      p2.x = box.origin.x + ( box.size.w / 4 );
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'B':
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'C':
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'D':
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'E':
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case 'F':
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    // Time characters
    case 'M':
      p1.x = box.origin.x + ( box.size.w / 4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      p2.x = box.origin.x + ( box.size.w / 4 );
      p2.y = box.origin.y + box.size.h / 10;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w / 4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w / 4  ) * 2;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w / 4  ) * 2;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 6;
      draw_line_fallback ( ctx, p1, p2, line_width / 2 );
      break;
    case 'P':
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 9;
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      draw_line_fallback ( ctx, p1, p2, line_width );
      p2.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p2.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      p1.x = box.origin.x + ( box.size.w /  4 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    case ':':
      // TODO: From circles to Rectangles.
      p1.x = box.origin.x + ( box.size.w /  2 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 3;
      graphics_fill_circle ( ctx, p1, line_width );
      p2.x = box.origin.x + ( box.size.w /  2 );
      p2.y = box.origin.y + ( box.size.h / 10 ) * 7;
      graphics_fill_circle ( ctx, p2, line_width );
      break;
    case '.':
      // TODO: From circles to Rectangles.
      p1.x = box.origin.x + ( box.size.w /  2 );
      p1.y = box.origin.y + ( box.size.h / 10 ) * 5;
      graphics_fill_circle ( ctx, p1, line_width );
      break;
    case '/':
      p1.x = box.origin.x + ( box.size.w /  4 ) * 3;
      p1.y = box.origin.y + ( box.size.h / 10 );
      p2.x = box.origin.x + ( box.size.w /  4 );
      p2.y = box.origin.y + ( box.size.h / 10 ) * 9;
      draw_line_fallback ( ctx, p1, p2, line_width );
      break;
    default:
      APP_LOG ( APP_LOG_LEVEL_ERROR, "Invalid digit in decimal system %d", digit );
      break;
  }
}

// Draw line with width
// (Based on code found here http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C)
void graphics_draw_line2(GContext *ctx, GPoint p0, GPoint p1, int8_t width) {
  // Order points so that lower x is first
  int16_t x0, x1, y0, y1;
  if (p0.x <= p1.x) {
    x0 = p0.x; x1 = p1.x; y0 = p0.y; y1 = p1.y;
  } else {
    x0 = p1.x; x1 = p0.x; y0 = p1.y; y1 = p0.y;
  }

  // Init loop variables
  int16_t dx = x1-x0;
  int16_t dy = abs(y1-y0);
  int16_t sy = y0<y1 ? 1 : -1;
  int16_t err = (dx>dy ? dx : -dy)/2;
  int16_t e2;

  // Calculate whether line thickness will be added vertically or horizontally based on line angle
  int8_t xdiff, ydiff;

  if (dx > dy) {
    xdiff = 0;
    ydiff = width/2;
  } else {
    xdiff = width/2;
    ydiff = 0;
  }

  // Use Bresenham's integer algorithm, with slight modification for line width, to draw line at any angle
  while (true) {
    // Draw line thickness at each point by drawing another line
    // (horizontally when > +/-45 degrees, vertically when <= +/-45 degrees)
    graphics_draw_line(ctx, GPoint(x0-xdiff, y0-ydiff), GPoint(x0+xdiff, y0+ydiff));

    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0++; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


void draw_line_fallback ( GContext * ctx, GPoint p1, GPoint p2, int width ) {
  #if defined ( PBL_SDK_3 )
    graphics_draw_line ( ctx, p1, p2 );
  #else
    graphics_draw_line2 ( ctx, p1, p2, width );
  #endif
}

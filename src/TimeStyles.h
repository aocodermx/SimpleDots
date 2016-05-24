#pragma once

#define DIGIT_BINARY_SPACE 6
#define DIGIT_BCD_SPACE 6

void draw_digit ( GContext * ctx, GRect box, char digit );
void draw_time_style_12hrs  ( GContext * ctx, GRect bounds, struct tm * tick_time );
void draw_time_style_24hrs  ( GContext * ctx, GRect bounds, struct tm * tick_time );
void draw_time_style_bcd    ( GContext * ctx, GRect bounds, struct tm * tick_time );
void draw_time_style_binary ( GContext * ctx, GRect bounds, struct tm * tick_time );

// TODO: draw_time_binary
// TODO: draw_time_hex

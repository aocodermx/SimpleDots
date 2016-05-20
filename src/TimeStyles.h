#pragma once

void draw_digit ( GContext * ctx, GRect box, int digit );
void draw_time_style_24hrs ( GContext * ctx, GRect bounds, struct tm * tick_time );
void draw_time_style_12hrs ( GContext * ctx, GRect bounds, struct tm * tick_time );

// TODO: draw_time_binary
// TODO: draw_time_hex

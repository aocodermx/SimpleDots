#include <pebble.h>
#include "TimeStyles.h"
#include "SimpleDots.h"

static Window *window;

// Variables for UI.
static TextLayer *s_layer_weather;
static Layer     *s_layer_time;
static Layer     *s_layer_date;
static Layer     *s_layer_battery;

// Function prototypes in order of implementation.
static void window_load ( Window *window );
static void window_appear ( Window *window );
static void window_unload ( Window *window );
static void handler_ticktimer_service ( struct tm *tick_time, TimeUnits units_changed );
static void handler_battery_service ( BatteryChargeState charge );
static void update_proc_layer_time ( Layer *layer, GContext *context );
static void update_proc_layer_date ( Layer *layer, GContext *context );
static void update_proc_layer_battery ( Layer *layer, GContext *context );


// Functions implementation.
static void window_load ( Window *window ) {
  int height_offset = 0;
  Layer *window_layer = window_get_root_layer ( window );
  GRect bounds = layer_get_bounds ( window_layer );

  // Define UI
  /*
  s_layer_weather = text_layer_create ( GRect ( 0, height_offset + LAYER_HEIGHT_WEATHER / 3, bounds.size.w, LAYER_HEIGHT_WEATHER ) );
  text_layer_set_text_alignment ( s_layer_weather, GTextAlignmentCenter );
  text_layer_set_text ( s_layer_weather, "WEATHER" );
  height_offset += LAYER_HEIGHT_WEATHER;
  layer_add_child ( window_layer, text_layer_get_layer ( s_layer_weather ) );
*/
  height_offset += 15;

  s_layer_date = layer_create ( GRect ( 0, height_offset, bounds.size.w, LAYER_HEIGHT_DATE ) );
  layer_set_update_proc ( s_layer_date, update_proc_layer_date );
  height_offset += LAYER_HEIGHT_DATE;
  layer_add_child ( window_layer, s_layer_date );

  s_layer_time = layer_create ( GRect ( 0, height_offset, bounds.size.w, LAYER_HEIGHT_TIME ) );
  layer_set_update_proc ( s_layer_time, update_proc_layer_time );
  height_offset += LAYER_HEIGHT_TIME;
  layer_add_child ( window_layer, s_layer_time );

  s_layer_battery = layer_create ( GRect ( 0, height_offset, bounds.size.w, LAYER_HEIGHT_BATTERY ) );
  layer_set_update_proc ( s_layer_battery, update_proc_layer_battery );
  height_offset += LAYER_HEIGHT_BATTERY;
  layer_add_child ( window_layer, s_layer_battery );

  // Subscribe services.
  tick_timer_service_subscribe ( MINUTE_UNIT, handler_ticktimer_service );
  battery_state_service_subscribe ( handler_battery_service );
}

static void window_appear ( Window *window ) {
  handler_battery_service ( battery_state_service_peek ( ) );
}

static void window_unload ( Window *window ) {
  // Destroy UI components
  text_layer_destroy ( s_layer_weather );
  layer_destroy ( s_layer_time );
  layer_destroy ( s_layer_date );
  layer_destroy ( s_layer_battery );

  // Unsubscribe services.
  tick_timer_service_unsubscribe ( );
  battery_state_service_unsubscribe ( );
}

static void handler_ticktimer_service ( struct tm *tick_time, TimeUnits units_changed ) {
  layer_mark_dirty ( s_layer_time );
  layer_mark_dirty ( s_layer_date );
}

static void handler_battery_service ( BatteryChargeState charge ) {
  // static char current_battery[] = "XXX%%";
  // snprintf ( current_battery, sizeof ( current_battery ), "%d%%", charge.charge_percent );

  layer_mark_dirty ( s_layer_battery );
}

static void update_proc_layer_time ( Layer *layer, GContext *ctx ) {
  // TODO: Add ENUM for all time styles.
  int style = 1;

  GRect bounds = layer_get_bounds ( s_layer_time );

  time_t temp          = time      ( NULL );
  struct tm *tick_time = localtime ( &temp );

  switch (style) {
    case 1: // 12 HRS
      draw_time_style_12hrs ( ctx, bounds, tick_time );
      break;
    default: // 24 HRS
      draw_time_style_24hrs ( ctx, bounds, tick_time );
      break;
  }
}

static void update_proc_layer_date ( Layer *layer, GContext *ctx ) {

  GRect bounds = layer_get_bounds ( s_layer_date );

  char buffer[] = "DD.MM.YYYY";
  int digit_width = ( bounds.size.w * 0.9 ) / sizeof ( buffer );

  time_t temp          = time      ( NULL );
  struct tm *tick_time = localtime ( &temp );

  int offset_x = ( bounds.size.w - digit_width *  (sizeof ( buffer ) - 1 ) ) / 2;

  strftime ( buffer, sizeof( buffer ), "%d.%m.%Y", tick_time );

  for ( uint i = 0; i < sizeof ( buffer ); i++ ) {
    draw_digit ( ctx, GRect ( bounds.origin.x + digit_width * i + offset_x, bounds.origin.y, digit_width, bounds.size.h ), buffer[i] );
  }
}

static void update_proc_layer_battery ( Layer * layer, GContext * ctx ) {
  GRect bounds = layer_get_bounds ( layer );
  BatteryChargeState battery = battery_state_service_peek ( );

  int dots       = battery.charge_percent / 10;
  int space      = LAYER_HEIGHT_BATTERY;
  int dot_radius = LAYER_HEIGHT_BATTERY / 3;

  int start_y = LAYER_HEIGHT_BATTERY / 1.5;
  int start_x = ( bounds.size.w - (( dots - 1 ) * space) ) / 2;

  for ( int i = 0; i < dots; i++ ) {
    graphics_draw_circle ( ctx, GPoint ( start_x + space * i, start_y ), dot_radius );
    graphics_fill_circle ( ctx, GPoint ( start_x + space * i, start_y ), dot_radius );
  }
}

static void init ( void ) {
  window = window_create ( );
  window_set_window_handlers ( window, ( WindowHandlers ) {
    .load = window_load,
    .appear = window_appear,
    .unload = window_unload,
  } );
  window_stack_push ( window, true );
}

static void deinit ( void ) {
  window_destroy ( window );
}

int main ( void ) {
  init ( );

  APP_LOG ( APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window );

  app_event_loop ( );
  deinit ( );
}

#include <pebble.h>
#include "TimeStyles.h"
#include "SimpleDots.h"

static Window *window;

// Variables for UI.
static TextLayer *s_layer_weather;
static Layer     *s_layer_time;
static Layer     *s_layer_date;
static Layer     *s_layer_battery;

static bool s_conf_weather;
static bool s_conf_date;
static int  s_conf_date_format;
static int  s_conf_hour_format;
static bool s_conf_battery;
static int  s_conf_background;

// Enum for Phone communication.
typedef enum {
  appkVersion    = 0,
  appkWeather    = 1,
  appkDate       = 2,
  appkDateFormat = 3,
  appkHourFormat = 4,
  appkBattery    = 5,
  appkBackground = 6
} AppKey;

typedef enum {
  format12hrs = 1,
  format24hrs = 2,
} TimeFormat;

// Function prototypes in order of implementation.
static void window_load ( Window *window );
static void window_appear ( Window *window );
static void window_disappear ( Window *window );
static void window_unload ( Window *window );
static void draw_ui   ( );
static void redraw_ui ( );
static void handler_ticktimer_service ( struct tm *tick_time, TimeUnits units_changed );
static void handler_battery_service ( BatteryChargeState charge );
static void update_proc_layer_time ( Layer *layer, GContext *context );
static void update_proc_layer_date ( Layer *layer, GContext *context );
static void update_proc_layer_battery ( Layer *layer, GContext *context );
// Communication handlers
static void handler_inbox_success   ( DictionaryIterator *, void *                  );
static void handler_inbox_failed    ( AppMessageResult    , void *                  );
static void handler_outbox_success  ( DictionaryIterator *, void *                  );
static void handler_outbox_failed   ( DictionaryIterator *, AppMessageResult, void *);


// Functions implementation.
static void window_load ( Window *window ) {
  s_conf_weather     = persist_exists ( appkWeather    ) ? persist_read_int ( appkWeather    ) : 1;
  s_conf_date        = persist_exists ( appkDate       ) ? persist_read_int ( appkDate       ) : 1;
  s_conf_date_format = persist_exists ( appkDateFormat ) ? persist_read_int ( appkDateFormat ) : 1;
  s_conf_hour_format = persist_exists ( appkHourFormat ) ? persist_read_int ( appkHourFormat ) : 1;
  s_conf_battery     = persist_exists ( appkBattery    ) ? persist_read_int ( appkBattery    ) : 1;
  s_conf_background  = persist_exists ( appkBackground ) ? persist_read_int ( appkBackground ) : 1;

  draw_ui ( );

  // Subscribe services.
  tick_timer_service_subscribe ( MINUTE_UNIT, handler_ticktimer_service );
  battery_state_service_subscribe ( handler_battery_service );
}

static void window_appear ( Window *window ) {
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_weather    : %d", s_conf_weather     );
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_date       : %d", s_conf_date        );
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_date_format: %d", s_conf_date_format );
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_hour_format: %d", s_conf_hour_format );
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_battery    : %d", s_conf_battery     );
  APP_LOG ( APP_LOG_LEVEL_INFO, "CONFIG s_conf_background : %d", s_conf_background  );
  handler_battery_service ( battery_state_service_peek ( ) );
}

static void window_disappear ( Window *window ) {
  persist_write_int ( appkWeather, s_conf_weather );
  persist_write_int ( appkDate, s_conf_date );
  persist_write_int ( appkDateFormat, s_conf_date_format );
  persist_write_int ( appkHourFormat, s_conf_hour_format );
  persist_write_int ( appkBattery, s_conf_battery );
  persist_write_int ( appkBackground, s_conf_background );
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

static void draw_ui ( ) {
  int height_offset = 0;
  int time_height = 0;
  height_offset += 5;

  Layer *window_layer = window_get_root_layer ( window );
  GRect bounds        = layer_get_bounds ( window_layer );

  if ( s_conf_weather ) {
    s_layer_weather = text_layer_create ( GRect ( 0, height_offset + LAYER_HEIGHT_WEATHER / 3, bounds.size.w, LAYER_HEIGHT_WEATHER ) );
    text_layer_set_text_alignment ( s_layer_weather, GTextAlignmentCenter );
    text_layer_set_text ( s_layer_weather, "WEATHER" );
    height_offset += LAYER_HEIGHT_WEATHER;
    layer_add_child ( window_layer, text_layer_get_layer ( s_layer_weather ) );
  }

  if ( s_conf_date ) {
    s_layer_date = layer_create ( GRect ( 0, height_offset, bounds.size.w, LAYER_HEIGHT_DATE ) );
    layer_set_update_proc ( s_layer_date, update_proc_layer_date );
    height_offset += LAYER_HEIGHT_DATE;
    layer_add_child ( window_layer, s_layer_date );
  }

  time_height = s_conf_battery ? bounds.size.h - ( height_offset + LAYER_HEIGHT_BATTERY + 5 ) : bounds.size.h - ( height_offset + 5 );
  s_layer_time = layer_create ( GRect ( 0, height_offset, bounds.size.w, time_height ) );
  layer_set_update_proc ( s_layer_time, update_proc_layer_time );
  height_offset += time_height;
  layer_add_child ( window_layer, s_layer_time );

  if ( s_conf_battery ) {
    s_layer_battery = layer_create ( GRect ( 0, height_offset, bounds.size.w, LAYER_HEIGHT_BATTERY ) );
    layer_set_update_proc ( s_layer_battery, update_proc_layer_battery );
    height_offset += LAYER_HEIGHT_BATTERY;
    layer_add_child ( window_layer, s_layer_battery );
  }
}

static void redraw_ui ( ) {
  layer_remove_child_layers ( window_get_root_layer ( window ) );

  if ( s_conf_weather ) text_layer_destroy ( s_layer_weather );
  if ( s_conf_date    ) layer_destroy ( s_layer_date );
  layer_destroy ( s_layer_time );
  if ( s_conf_battery ) layer_destroy ( s_layer_battery );

  draw_ui ( );

  layer_mark_dirty ( text_layer_get_layer ( s_layer_weather ) );
  layer_mark_dirty ( s_layer_date );
  layer_mark_dirty ( s_layer_time );
  layer_mark_dirty ( s_layer_battery );

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
  GRect bounds = layer_get_bounds ( s_layer_time );

  time_t temp          = time      ( NULL );
  struct tm *tick_time = localtime ( &temp );

  switch ( s_conf_hour_format ) {
    case format12hrs:
      draw_time_style_12hrs ( ctx, bounds, tick_time );
      break;
    case format24hrs:
      draw_time_style_24hrs ( ctx, bounds, tick_time );
      break;
    default:
      draw_time_style_24hrs ( ctx, bounds, tick_time );
      break;
  }
}

static void update_proc_layer_date ( Layer *layer, GContext *ctx ) {
  time_t temp          = time      ( NULL );
  struct tm *tick_time = localtime ( &temp );

  char buffer[15];
  GRect bounds = layer_get_bounds ( s_layer_date );

  switch ( s_conf_date_format ) {
    case 1:
      strftime ( buffer, sizeof ( buffer ), "%d.%m.%Y", tick_time );
      break;
    case 2:
      strftime ( buffer, sizeof ( buffer ), "%d.%m.%y", tick_time );
      break;
    case 3:
      strftime ( buffer, sizeof ( buffer ), "%d %b %Y", tick_time );
      break;
    case 4:
      strftime ( buffer, sizeof ( buffer ), "%A %d", tick_time );
      break;
    default:
      strftime ( buffer, sizeof( buffer ), "%d.%m.%Y", tick_time );
      break;
  }

  int digit_width = ( bounds.size.w * 0.8 ) / strlen ( buffer );
  int offset_x = ( bounds.size.w - digit_width *  (strlen ( buffer ) ) ) / 2;

  for ( uint i = 0; i < strlen ( buffer ); i++ ) {
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

static void handler_inbox_success ( DictionaryIterator * iterator, void * context) {
  Tuple * tuple_appkWeather    = dict_find ( iterator, appkWeather );
  Tuple * tuple_appkDate       = dict_find ( iterator, appkDate );
  Tuple * tuple_appkHourFormat = dict_find ( iterator, appkHourFormat );
  Tuple * tuple_appkBattery    = dict_find ( iterator, appkBattery );
  Tuple * tuple_appkBackground = dict_find ( iterator, appkBackground );
  Tuple * tuple_appkDateFormat = dict_find ( iterator, appkDateFormat );

  if ( tuple_appkWeather    && tuple_appkDate       &&
       tuple_appkDateFormat && tuple_appkHourFormat &&
       tuple_appkBattery    && tuple_appkBackground ) {

    s_conf_weather     = tuple_appkWeather->value->uint8;
    s_conf_date        = tuple_appkDate->value->uint8;
    s_conf_date_format = tuple_appkDateFormat->value->uint8;
    s_conf_hour_format = tuple_appkHourFormat->value->uint8;
    s_conf_battery     = tuple_appkBattery->value->uint8;
    s_conf_background  = tuple_appkBattery->value->int32;

    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkWeather   : %d", s_conf_weather );
    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkDate      : %d", s_conf_date );
    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkDateFormat: %d", s_conf_date_format );
    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkHourFormat: %d", s_conf_hour_format );
    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkBattery   : %d", s_conf_battery );
    APP_LOG ( APP_LOG_LEVEL_INFO, "handler_inbox_success appkBackground: %d", s_conf_background );

    redraw_ui ( );
  }
}

static void handler_inbox_failed    ( AppMessageResult reason, void * context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "handler_inbox_failed: Message dropped! -> %d", reason);
}

static void handler_outbox_success  ( DictionaryIterator * iterator, void * context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "handler_outbox_success: Outbox send success!");
}

static void handler_outbox_failed   ( DictionaryIterator * iterator, AppMessageResult reason, void * context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "handler_outbox_failed: Outbox send failed! -> %d", reason);
}

static void init ( void ) {
  window = window_create ( );
  window_set_window_handlers ( window, ( WindowHandlers ) {
    .load      = window_load,
    .appear    = window_appear,
    .unload    = window_unload,
    .disappear = window_disappear,
  } );

  app_message_register_inbox_received ( handler_inbox_success  );
  app_message_register_inbox_dropped  ( handler_inbox_failed   );
  app_message_register_outbox_failed  ( handler_outbox_failed  );
  app_message_register_outbox_sent    ( handler_outbox_success );
  app_message_open ( 200, 200 );

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

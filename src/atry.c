/*
 * main.c
 * Creates a Window, Layer and animates it around the screen
 * from corner to corner. It uses the `.stopped` callback to schedule the
 * next stage of the animation.
 */

#include <pebble.h>

static Window *s_main_window;
static Layer *s_layer;
static Layer *g_layer;
int x = 0;

//setup text layer
static TextLayer *s_time_layer, *s_date_layer;
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  static char d_buffer[8];
  strftime(d_buffer, sizeof(d_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  
  strftime(s_buffer, sizeof(s_buffer), "%a %d %b", tick_time);
  text_layer_set_text(s_date_layer, d_buffer);
  
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  if(x<=120)//from 0 to 120 minutes, increment x so the rectangle width grows
  {
    x=x+1;
  }
  else
    x=0;//reset babr back to zero.
}
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
 
  s_layer = layer_create(GRect(12+x, 108, 120, 20));
  g_layer = layer_create(GRect(12, 108, x, 20));
  
 // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(5, PBL_IF_ROUND_ELSE(0,20), bounds.size.w, 80));
  /**layer_set_update_proc(s_layer, update_proc);
  graphics_context_set_fill_color(ctx, GColorKellyGreen);
  graphics_fill_rect(ctx, GRect(12+x, 108, 120, 20), 2, GCornersAll);
  graphics_context_set_fill_color(ctx, GColorSunsetOrange);
  graphics_fill_rect(ctx, GRect(12, 108, x, 20), 2, GCornersAll);*/
  

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlue);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //Create Date text layer
  s_date_layer = text_layer_create(GRect(0, 50, 150, 50));
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_background_color(s_date_layer, GColorLiberty);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
     
  //Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorKellyGreen);
  graphics_fill_rect(ctx, layer_get_bounds(s_layer), 4, GCornersAll);
}
static void main_window_unload(Window *window) {
  //destroy text and date layers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  layer_destroy(s_layer);
  layer_destroy(g_layer);
}
static void init(void) {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  window_set_background_color(s_main_window, GColorClear);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 }
static void deinit(void) {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
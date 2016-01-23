/*
 * main.c
 * Creates a Window, Layer and animates it around the screen
 * from corner to corner. It uses the `.stopped` callback to schedule the
 * next stage of the animation.
 */

#include <pebble.h>

#define BOX_SIZE 20

#define ANIM_DURATION 500
#define ANIM_DELAY 300

static Window *s_main_window;

//setup text layer
static TextLayer *s_time_layer;
static GPath *s_my_path_ptr = NULL;

static const GPathInfo BOLT_PATH_INFO = {
  .num_points = 6,
  .points = (GPoint []) {{21, 0}, {14, 26}, {28, 26}, {7, 60}, {14, 34}, {0, 34}}
};

// .update_proc of my_layer:
void my_layer_update_proc(Layer *my_layer, GContext* ctx) {
  // Fill the path:
  graphics_context_set_fill_color(ctx, GColorWhite);
  gpath_draw_filled(ctx, s_my_path_ptr);
  // Stroke the path:
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_draw_outline(ctx, s_my_path_ptr);
}

void setup_my_path(void) {
  s_my_path_ptr = gpath_create(&BOLT_PATH_INFO);
  // Rotate 15 degrees:
  gpath_rotate_to(s_my_path_ptr, TRIG_MAX_ANGLE / 360 * 15);
  // Translate by (5, 5):
  gpath_move_to(s_my_path_ptr, GPoint(5, 5));
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//setup animation layer
static Layer *s_box_layer;
static PropertyAnimation *s_box_animation;
static int s_current_stage = 0;

// Function prototype
static void next_animation();

static void anim_stopped_handler(Animation *animation, bool finished, void *context) {
  // Schedule the next one, unless the app is exiting
  if (finished) {
    next_animation();
  }
}

static void next_animation() {
  // Determine start and finish positions
  GRect start, finish;
  switch (s_current_stage) {
    case 0:
      start = GRect(0, 0, BOX_SIZE, BOX_SIZE);
      finish = GRect(144 - BOX_SIZE, 0, BOX_SIZE, BOX_SIZE);
      break;
    case 1:
      start = GRect(144 - BOX_SIZE, 0, BOX_SIZE, BOX_SIZE);
      finish = GRect(144 - BOX_SIZE, 168 - BOX_SIZE, BOX_SIZE, BOX_SIZE);
      break;
    case 2:
      start = GRect(144 - BOX_SIZE, 168 - BOX_SIZE, BOX_SIZE, BOX_SIZE);
      finish = GRect(0, 168 - BOX_SIZE, BOX_SIZE, BOX_SIZE);
      break;
    case 3:
      start = GRect(0, 168 - BOX_SIZE, BOX_SIZE, BOX_SIZE);
      finish = GRect(0, 0, BOX_SIZE, BOX_SIZE);
      break;
    default:
      start = GRect(0, 0, BOX_SIZE, BOX_SIZE);
      finish = GRect(0, 0, BOX_SIZE, BOX_SIZE);
      break;
  }

  // Schedule the next animation
  s_box_animation = property_animation_create_layer_frame(s_box_layer, &start, &finish);
  animation_set_duration((Animation*)s_box_animation, ANIM_DURATION);
  animation_set_delay((Animation*)s_box_animation, ANIM_DELAY);
  animation_set_curve((Animation*)s_box_animation, AnimationCurveEaseInOut);
  animation_set_handlers((Animation*)s_box_animation, (AnimationHandlers) {
    .stopped = anim_stopped_handler
  }, NULL);
  animation_schedule((Animation*)s_box_animation);

  // Increment stage and wrap
  s_current_stage = (s_current_stage + 1) % 4;
}

static void update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 4, GCornersAll);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  GRect bounds = layer_get_bounds(window_layer);
// Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorMintGreen);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // ******Create Animation Layer
  s_box_layer = layer_create(GRect(0, 0, BOX_SIZE, BOX_SIZE));
  layer_set_update_proc(s_box_layer, update_proc);
  layer_add_child(window_layer, s_box_layer);
}

static void main_window_unload(Window *window) {
  //destroy text layer
  text_layer_destroy(s_time_layer);
  // Destroy Layer
  layer_destroy(s_box_layer);
}

static void init(void) {
    // Create main Window element and assign to pointer
  s_main_window = window_create();

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
 
  // Start animation loop
  next_animation();
}

static void deinit(void) {
  // Stop any animation in progress
  animation_unschedule_all();

  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

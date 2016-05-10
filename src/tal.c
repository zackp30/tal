#include <pebble.h>

static Window *window;
static Layer *window_layer;
static ActionBarLayer *s_action_bar;
static TextLayer *s_text_layer;
static GBitmap *s_increment_bitmap, *s_decrement_bitmap, *s_mode_bitmap;

static char* mode = "normal";

static int counter = 0;
static char[16] counter_s;

void increment_normal() {
  counter += 1;
  sprintf(counter_s, "%d", counter);
  text_layer_set_text(s_text_layer, counter_s);
}

void decrement_normal() {
  counter -= 1;
  sprintf(counter_s, "%d", counter);
  text_layer_set_text(s_text_layer, counter_s);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch (mode) {
  case "normal": {
    decrement_normal();
    break;
  }
  default:
    break;
  }
}
void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch (mode) {
  case "normal": {
    increment_normal();
    break;
  }
  default:
    break;
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
}

static void window_load(Window *window) {
  s_increment_bitmap = gbitmap_create_with_resource(RESOURCE_ID_INCREMENT_BITMAP);
  s_decrement_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DECREMENT_BITMAP);
  s_mode_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MODE_BITMAP);

  GRect bounds = layer_get_bounds(window_layer);
  s_action_bar = action_bar_layer_create();
  action_bar_layer_set_background_color(s_action_bar, GColorRed);
  action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_UP, s_increment_bitmap);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_DOWN, s_decrement_bitmap);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_mode_bitmap);
  action_bar_layer_add_to_window(s_action_bar, window);

  s_text_layer = text_layer_create(GRect(0, 0, 144, 40));
  text_layer_set_text(s_text_layer, counter_s);
}

static void window_unload(Window *window) {
  action_bar_layer_destroy(s_action_bar);
  gbitmap_destroy(s_increment_bitmap);
  gbitmap_destroy(s_decrement_bitmap);
  gbitmap_destroy(s_mode_bitmap);
}

static void init(void) {
  window = window_create();
  /* window_set_click_config_provider(window, click_config_provider); */
  window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
        .unload = window_unload,
        });
  const bool animated = true;
  window_stack_push(window, animated);
  window_layer = window_get_root_layer(window);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}

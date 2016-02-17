//make configurable

#include <pebble.h>

static Window *window;
static Layer *bg_layer;
static Layer *hands_layer;

static bool circles = false; // 0 for lines & 1 for circles

static void bg_create_proc(Layer *layer, GContext *ctx) {
	graphics_context_set_stroke_width(ctx, 1);
	graphics_context_set_stroke_color(ctx, GColorWhite);

	for (int i = 0; i < 60; i++) {
		int32_t minute_angle = TRIG_MAX_ANGLE * i / 60;
		int minute_length = 70;
		GPoint center = GPoint(72, 84);
		GPoint minute_point = {
			.x = (int16_t)(sin_lookup(minute_angle) * (int32_t)minute_length / TRIG_MAX_RATIO) + center.x,
			.y = (int16_t)(-cos_lookup(minute_angle) * (int32_t)minute_length / TRIG_MAX_RATIO) + center.y,
		};
		int minute_length_2 = 66;
		GPoint minute_point_2 = {
			.x = (int16_t)(sin_lookup(minute_angle) * (int32_t)minute_length_2 / TRIG_MAX_RATIO) + center.x,
			.y = (int16_t)(-cos_lookup(minute_angle) * (int32_t)minute_length_2 / TRIG_MAX_RATIO) + center.y,
		};
		
		if (circles == true) {
			graphics_context_set_fill_color(ctx, GColorWhite);
			graphics_fill_circle(ctx, minute_point, 1);
		} else {
			graphics_draw_line(ctx, minute_point, minute_point_2);
		}
	}
	
	graphics_context_set_stroke_width(ctx, 3);
	for (int j = 0; j < 12; j++) {
		int32_t hour_angle = TRIG_MAX_ANGLE * j / 12;
		int hour_length = 70;
		GPoint center = GPoint(72, 84);
		GPoint hour_point = {
			.x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.x,
			.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.y
		};
	
		int hour_length_2 = 60;
		GPoint hour_point_2 = {
			.x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length_2 / TRIG_MAX_RATIO) + center.x,
			.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length_2 / TRIG_MAX_RATIO) + center.y
		};

		graphics_draw_line(ctx, hour_point, hour_point_2);
	}

	
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
	time_t temp = time(NULL);
	struct tm *t = localtime(&temp);

	GPoint center = GPoint(72, 84);

	//need 2 different lines for second hand (overflowing)
	//360 - minute ANGLE gives inverse
	

	
	//----------------------------------------------------------------------------------------------------------
	//MINUTE

	graphics_context_set_stroke_width(ctx, 1);
	graphics_context_set_stroke_color(ctx, GColorWhite);

	int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min / 60;
	
	int minute_length_1_small = 16;
	GPoint minute_point_1_small = {
		.x = (int16_t)(sin_lookup(minute_angle) * (int32_t)minute_length_1_small / TRIG_MAX_RATIO) + center.x,
	    .y = (int16_t)(-cos_lookup(minute_angle) * (int32_t)minute_length_1_small / TRIG_MAX_RATIO) + center.y,
	};

	graphics_draw_line(ctx, center, minute_point_1_small);

	graphics_context_set_stroke_width(ctx, 5);

	int minute_length_1_long = 64;
	GPoint minute_point_1_long = {
		.x = (int16_t)(sin_lookup(minute_angle) * (int32_t)minute_length_1_long / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(minute_angle) * (int32_t)minute_length_1_long / TRIG_MAX_RATIO) + center.y,
	};

	int minute_length_2_long = 14;
	GPoint minute_point_2_long = {
		.x = (int16_t)(sin_lookup(minute_angle) * (int32_t)minute_length_2_long / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(minute_angle) * (int32_t)minute_length_2_long / TRIG_MAX_RATIO) + center.y,
	};
	
	graphics_draw_line(ctx, minute_point_2_long, minute_point_1_long);

	//----------------------------------------------------------------------------------------------------------
	//HOUR
	
	graphics_context_set_stroke_width(ctx, 1);

	int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);

	int hour_length_1_small = 16;
	GPoint hour_hand_1_small = {
	    .x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length_1_small / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length_1_small / TRIG_MAX_RATIO) + center.y,
	};

	graphics_draw_line(ctx, center, hour_hand_1_small);
	
	graphics_context_set_stroke_width(ctx, 5);

	int hour_length_1_long = 40;
	GPoint hour_hand_1_long = {
		.x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length_1_long / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length_1_long / TRIG_MAX_RATIO) + center.y,
	};

	int hour_length_2_long = 14;
	GPoint hour_hand_2_long = {
		.x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length_2_long / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length_2_long / TRIG_MAX_RATIO) + center.y,
	};

	graphics_draw_line(ctx, hour_hand_1_long, hour_hand_2_long);

	//----------------------------------------------------------------------------------------------------------

	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_circle(ctx, center, 4);

	//----------------------------------------------------------------------------------------------------------
	//SECOND

	graphics_context_set_stroke_color(ctx, GColorOrange);
	graphics_context_set_stroke_width(ctx, 1);

	int second_length = 67;
	
	int32_t second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  	GPoint second_hand = {
		.x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_length / TRIG_MAX_RATIO) + center.x,
		.y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_length / TRIG_MAX_RATIO) + center.y,
	};

	graphics_draw_line(ctx, center, second_hand);

	int second_length_2 = 20;
	int second_angle_2 = (TRIG_MAX_ANGLE * t->tm_sec / 60);

  	GPoint second_hand_2 = {
		.x = -1 * (int16_t)(sin_lookup(second_angle_2) * (int32_t)second_length_2 / TRIG_MAX_RATIO) + center.x,
		.y = -1 * (int16_t)(-cos_lookup(second_angle_2) * (int32_t)second_length_2 / TRIG_MAX_RATIO) + center.y,
	};
	
	graphics_draw_line(ctx, center, second_hand_2);
	
	//----------------------------------------------------------------------------------------------------------
	//this comes at the end
	


	graphics_context_set_fill_color(ctx, GColorOrange);
	graphics_fill_circle(ctx, center, 3);
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_circle(ctx, center, 1);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	layer_mark_dirty(hands_layer);
}

static void main_window_load() {
	bg_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(bg_layer, bg_create_proc);
	layer_add_child(window_get_root_layer(window), bg_layer);

	hands_layer = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(hands_layer, hands_update_proc);
	layer_add_child(window_get_root_layer(window), hands_layer);
}

static void main_window_unload() {
	layer_destroy(bg_layer);
	layer_destroy(hands_layer);
}

static void init() {
	window = window_create();
	
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

	window_set_window_handlers(window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});
	
	window_set_background_color(window, GColorBlack);

	window_stack_push(window, true);
}

static void deinit() {
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}

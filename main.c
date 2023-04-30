#include <stdio.h>

#define RANUI_IMPLEMENTATION
#include "ranui.h"

static bool quit = false;

int32_t main(void) {
    rui_init();

	int window_width, window_height;
	rui_screen_dims(&window_width, &window_height);
	window_width  /= 2;
	window_height /= 2;

	rui_window* window;
    window = rui_window_open("test", window_width, window_height);

    while (!quit) {
		rui_event event;
		while (rui_process_events(&event)) {
			switch (event.type) {
            case RUI_EVENT_TYPE_WINDOW_CLOSED: {
                quit = true;
                break;
            }
            case RUI_EVENT_TYPE_WINDOW_RESIZED: {
                window_width  = event.window_width;
                window_height = event.window_height;
            }
            case RUI_EVENT_TYPE_KEY_PRESSED: {
                switch (event.key) {
                case '1': {
                    quit = true;
                    break;
                }
                }

                printf("key pressed %c\n", event.key);
                break;
            }
			}
		}
    }

    rui_close_window(window);

    return rui_quit(0);
}

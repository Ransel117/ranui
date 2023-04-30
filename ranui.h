#ifndef RANUI_H
#define RANUI_H

#include <stdbool.h>
#include <stdint.h>

typedef int rui_event_type;
enum rui_event_type {
	RUI_EVENT_TYPE_UNKNOWN,
	RUI_EVENT_TYPE_KEY_PRESSED,
	RUI_EVENT_TYPE_KEY_RELEASED,
	RUI_EVENT_TYPE_WINDOW_RESIZED,
	RUI_EVENT_TYPE_WINDOW_CLOSED,
};

typedef struct rui_event rui_event;
struct rui_event {
	rui_event_type type;
	union {
		char key;
		struct {
			uint32_t window_width;
			uint32_t window_height;
		};
	};
	void* handle;
};


typedef struct rui_window rui_window;
struct rui_window {
	void* instance;
	void* handle;
};

void rui_init(void);
void rui_screen_dims(int*, int*);
rui_window *rui_window_open(char*, int, int);
bool rui_process_events(rui_event*);
void rui_close_window(rui_window*);

#define UNUSED(x) ((void)x)

#endif /* RANUI_H */

#ifdef RANUI_IMPLEMENTATION
#include <malloc.h>

#if defined(__linux__)
#include <X11/Xlib.h>
#include <signal.h>

typedef struct rui_x11 rui_x11;
struct rui_x11 {
	Display* display;
	uint32_t window_width;
	uint32_t window_height;
};

rui_x11 dm;

int x11_handler(Display *display, XErrorEvent *e) {
	char buf[2048];
	XGetErrorText(display, e->error_code, buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}

void rui_init(void) {
	dm.display = XOpenDisplay(NULL);
	XSetErrorHandler(x11_handler);
}

void rui_screen_dims(int *width_out, int *height_out) {
	Screen* screen = XDefaultScreenOfDisplay(dm.display);
	*width_out = screen->width;
	*height_out = screen->height;
}

rui_window *rui_window_open(char *title, int width, int height) {
	rui_window *window;

    window = (rui_window*)malloc(sizeof(rui_window));

	Display* d = dm.display;
	int s = DefaultScreen(d);

	Window w = XCreateSimpleWindow(d, RootWindow(d, s), width / 2, height / 2, width, height, 1, BlackPixel(d, s), WhitePixel(d, s));

    XStoreName(d, w, title);
    XSetIconName(d, w, title);

    window->instance = d;
	window->handle = (void*)(uintptr_t)w;
	XSelectInput(d, w, ExposureMask | KeyPressMask | StructureNotifyMask);
	XMapWindow(d, w);
	//XSetWMProtocols(dm.display, w, &dm.delete_message_atom, 1);
    XFlush(d);

	return window;
}

bool rui_process_events(rui_event* e) {
    XEvent xe;
    while (XPending(dm.display)) {
        XNextEvent(dm.display, &xe);
        if (xe.type == KeyPress) {
            e->type = RUI_EVENT_TYPE_KEY_PRESSED;
            KeySym key_sym = XLookupKeysym(&xe.xkey, 0);
            if (key_sym < 128) {
                e->key = (char)key_sym;
                return true;
            }
        } else if (xe.type == KeyRelease) {
            e->type = RUI_EVENT_TYPE_KEY_RELEASED;
            KeySym key_sym = XLookupKeysym(&xe.xkey, 0);
            if (key_sym < 128) {
                e->key = (char)key_sym;
                return true;
            }
        }
    }
    return false;
}
void rui_close_window(rui_window *window) {
    XCloseDisplay(window->instance);
    free(window);
    UNUSED(window);
}
#elif

#else
#error "not supported"
#endif /* platform check */
#endif /* RANUI_IMPLEMENTATION */

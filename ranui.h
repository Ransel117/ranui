#ifndef RANUI_H
#define RANUI_H

#include <stdint.h>

#ifndef __bool_true_false_are_defined
#define bool _Bool
#define true 1
#define false 0

#define __bool_true_false_are_defined 1
#endif /* Check if user has included stdbool.h */

#define RUI_PLATFORM_WINDOWS 0
#define RUI_PLATFORM_LINUX 1

#if defined(__linux__)
#define RUI_PLATFORM RUI_PLATFORM_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#define RUI_PLATFORM RUI_PLATFORM_WINDOWS
#else
#error "Platform not supported"
#endif /* Check platform */

/* GCC-compatible compiler (gcc, clang) */
#if defined(__GNUC__) || defined(__clang__) || defined(__PCC__)
#define RUI_INLINE static inline __attribute__((always_inline))
/* Microsoft cl */
#elif defined(_MSC_VER)
#define RUI_INLINE static inline __forceinline
/* Unknown */
#else
#define RUI_INLINE static inline
#endif /* Check compiler */

#define RUI_CLASS_NAME "ranui_program"
#define RUI_UNUSED(x) ((void)(x))

typedef int32_t rui_event_type;
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

RUI_INLINE void rui_init(void);
RUI_INLINE void rui_screen_dims(int32_t*, int32_t*);
RUI_INLINE rui_window* rui_window_open(char*, int32_t, int32_t);
RUI_INLINE bool rui_process_events(rui_event*);
RUI_INLINE void rui_close_window(rui_window*);
RUI_INLINE int32_t rui_quit(int32_t);
#endif /* RANUI_H */

#ifdef RANUI_IMPLEMENTATION
#include <malloc.h>

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <signal.h>

typedef struct rui_x11 rui_x11;
struct rui_x11 {
	Display* display;
	uint32_t window_width;
	uint32_t window_height;
};

rui_x11 dm;

RUI_INLINE int32_t x11_handler(Display *display, XErrorEvent *e) {
	char buf[2048];
	XGetErrorText(display, e->error_code, buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}
RUI_INLINE void rui_init(void) {
	dm.display = XOpenDisplay(NULL);
	XSetErrorHandler(x11_handler);
}
RUI_INLINE void rui_screen_dims(int32_t* width_out, int32_t* height_out) {
	Screen* screen = XDefaultScreenOfDisplay(dm.display);
	*width_out = screen->width;
	*height_out = screen->height;
}
RUI_INLINE rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
	rui_window *window;

    window = (rui_window*)malloc(sizeof(rui_window));

	Display* d = dm.display;
	int32_t s = DefaultScreen(d);

	Window w = XCreateSimpleWindow(d, RootWindow(d, s), width >> 1, height >> 1, width, height, 1, BlackPixel(d, s), WhitePixel(d, s));

    XSetClassHint(d, w, &(XClassHint){title, RUI_CLASS_NAME});
    XStoreName(d, w, title);
    XSetIconName(d, w, title);

    window->instance = d;
	window->handle = (void*)(uintptr_t)w;
	XSelectInput(d, w, ExposureMask | KeyPressMask | StructureNotifyMask);
	XMapWindow(d, w);
    XFlush(d);

	return window;
}
RUI_INLINE bool rui_process_events(rui_event* e) {
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
RUI_INLINE void rui_close_window(rui_window *window) {
    XDestroyWindow(window->instance, (Window)window->handle);
    XCloseDisplay(window->instance);
    free(window);
    RUI_UNUSED(window);
}
RUI_INLINE int32_t rui_quit(int32_t code) {
    return code;
}
#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct rui_win32 rui_win32;
struct rui_win32 {
    HINSTANCE  hinstance;
    HWND       hwnd;
    rui_event* event;
};

rui_win32 dm;

RUI_INLINE void rui_init(void) {
}
RUI_INLINE void rui_screen_dims(int32_t* width_out, int32_t* height_out) {
    *width_out  = GetSystemMetrics(SM_CXSCREEN);
    *height_out = GetSystemMetrics(SM_CYSCREEN);
}
RUI_INLINE LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    rui_event* e;

    // When called via DispatchMessage after TranslateMessage
    if ((e = dm.event)) {
        switch (Msg) {
        case WM_SIZE:
            e->type = RUI_EVENT_TYPE_WINDOW_RESIZED;
            e->window_width = LOWORD(lParam);
            e->window_height = HIWORD(lParam);
            dm.event = NULL;
            return 0;

        case WM_CHAR:
            e->type = RUI_EVENT_TYPE_KEY_PRESSED;
            e->key = wParam;
            dm.event = NULL;
            return 0;

        case WM_CLOSE:
            e->type = RUI_EVENT_TYPE_WINDOW_CLOSED;
            dm.event = NULL;
            return 0;

        default:
            return DefWindowProcA(hwnd, Msg, wParam, lParam);
        }
    // when called via PeekMessage or CreateWindowEx
    } else {
        switch (Msg) {
        default:
            return DefWindowProcA(hwnd, Msg, wParam, lParam);
        }
    }
}
RUI_INLINE rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
    int32_t swidth, sheight;
    HINSTANCE hinstance;
    WNDCLASSEXA wc;
    HWND hwnd;

    hinstance = GetModuleHandle(NULL);

    wc = (WNDCLASSEXA){0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW); // load cursor otherwise it defaults to busy cursor icon
    wc.lpszClassName = RUI_CLASS_NAME;

    RegisterClassExA(&wc);

    hwnd = CreateWindowExA(
        0, RUI_CLASS_NAME, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hinstance, NULL
    );

    rui_screen_dims(&swidth, &sheight);

    SetWindowPos(hwnd, HWND_TOP, width >> 1, height >> 1, swidth >> 1, sheight >> 1, 0);

    ShowWindow(hwnd, true);

    dm.hinstance = hinstance;
    dm.hwnd = hwnd;

    rui_window *window;

    window = (rui_window*)malloc(sizeof(rui_window));
    window->instance = dm.hinstance;
    window->handle = (void*)(uintptr_t)dm.hwnd;
    return window;
}
RUI_INLINE bool rui_process_events(rui_event* e) {
    MSG msg;

    e->type = RUI_EVENT_TYPE_UNKNOWN;
    dm.event = e;

    if (!PeekMessage(&msg, dm.hwnd, 0, 0, PM_REMOVE)) {
        return false;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    return true;
}
RUI_INLINE void rui_close_window(rui_window* window) {
    DestroyWindow((HWND)window->handle);
    free(window);
    RUI_UNUSED(window);
}
RUI_INLINE int32_t rui_quit(int32_t code) {
    PostQuitMessage(code);

    return code;
}
#else
#error "not supported"
#endif /* Platform check */
#endif /* RANUI_IMPLEMENTATION */

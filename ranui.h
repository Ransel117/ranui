#ifndef RANUI_H
#define RANUI_H

#define RUI_PLATFORM_WINDOWS 0
#define RUI_PLATFORM_LINUX 1

#define RUI_PLATFORM_ERROR "Platform not supported"

#if defined(__linux__)
#define RUI_PLATFORM RUI_PLATFORM_LINUX
#define VK_USE_PLATFORM_XCB_KHR
#elif defined(_WIN32) || defined(_WIN64)
#define RUI_PLATFORM RUI_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error RUI_PLATFORM_ERROR
#endif /* Check platform */

#ifndef __bool_true_false_are_defined
#define bool _Bool
#define true 1
#define false 0

#define __bool_true_false_are_defined 1
#endif /* Check if user has included stdbool.h */

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
#include <xcb/xcb.h>
#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
#include <windows.h>
#endif

#define RUI_CLASS_NAME "ranui_program"
#define RUI_VERSION 0
#define RUI_UNUSED(x) ((void)(x))
#define RUI_ERROR_RETURN(code, ...) do {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); return code;} while(0);
#define RUI_ERROR_EXIT(code, ...)   do {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(code);}  while(0);
#define arrlen(arr) (sizeof((arr)) / sizeof(*(arr)))

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
// TODO: maybe do something regarding åöä and their other specifics
// xcb keycodes seem to be locale independent and is position based i think
// might need to test with different keyboards
#define RUIK_0     0x13
#define RUIK_1     0x0a
#define RUIK_2     0x0b
#define RUIK_3     0x0c
#define RUIK_4     0x0d
#define RUIK_5     0x0e
#define RUIK_6     0x0f
#define RUIK_7     0x10
#define RUIK_8     0x11
#define RUIK_9     0x12
#define RUIK_Q     0x18
#define RUIK_W     0x19
#define RUIK_E     0x1a
#define RUIK_R     0x1b
#define RUIK_T     0x1c
#define RUIK_Y     0x1d
#define RUIK_U     0x1e
#define RUIK_I     0x1f
#define RUIK_O     0x20
#define RUIK_P     0x21
#define RUIK_A     0x26
#define RUIK_S     0x27
#define RUIK_D     0x28
#define RUIK_F     0x29
#define RUIK_G     0x2a
#define RUIK_H     0x2b
#define RUIK_J     0x2c
#define RUIK_K     0x2d
#define RUIK_L     0x2e
#define RUIK_Z     0x34
#define RUIK_X     0x35
#define RUIK_C     0x36
#define RUIK_V     0x37
#define RUIK_B     0x38
#define RUIK_N     0x39
#define RUIK_M     0x3a
#define RUIK_COMMA 0x3b
#define RUIK_DOT   0x3c
#define RUIK_SPACE 0x41
#define RUIK_ESC   0x09
#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
// TODO: maybe do something better other than pure ASCII for windows
#define RUIK_0     '0'
#define RUIK_1     '1'
#define RUIK_2     '2'
#define RUIK_3     '3'
#define RUIK_4     '4'
#define RUIK_5     '5'
#define RUIK_6     '6'
#define RUIK_7     '7'
#define RUIK_8     '8'
#define RUIK_9     '9'
#define RUIK_Q     'q'
#define RUIK_W     'w'
#define RUIK_E     'e'
#define RUIK_R     'r'
#define RUIK_T     't'
#define RUIK_Y     'y'
#define RUIK_U     'u'
#define RUIK_I     'i'
#define RUIK_O     'o'
#define RUIK_P     'p'
#define RUIK_A     'a'
#define RUIK_S     's'
#define RUIK_D     'd'
#define RUIK_F     'f'
#define RUIK_G     'g'
#define RUIK_H     'h'
#define RUIK_J     'j'
#define RUIK_K     'k'
#define RUIK_L     'l'
#define RUIK_Z     'z'
#define RUIK_X     'x'
#define RUIK_C     'c'
#define RUIK_V     'v'
#define RUIK_B     'b'
#define RUIK_N     'n'
#define RUIK_M     'm'
#define RUIK_COMMA ','
#define RUIK_DOT   '.'
#define RUIK_SPACE ' '
#else
#error RUI_PLATFORM_ERROR
#endif

#define RUI_NUM_KEYS 256
static const char* rui_key_list[RUI_NUM_KEYS] = {
/* NUMBERS */
    [RUIK_0]     = "0",
    [RUIK_1]     = "1",
    [RUIK_2]     = "2",
    [RUIK_3]     = "3",
    [RUIK_4]     = "4",
    [RUIK_5]     = "5",
    [RUIK_6]     = "6",
    [RUIK_7]     = "7",
    [RUIK_8]     = "8",
    [RUIK_9]     = "9",
/* LETTERS */
    [RUIK_Q]     = "q",
    [RUIK_W]     = "w",
    [RUIK_E]     = "e",
    [RUIK_R]     = "r",
    [RUIK_T]     = "t",
    [RUIK_Y]     = "y",
    [RUIK_U]     = "u",
    [RUIK_I]     = "i",
    [RUIK_O]     = "o",
    [RUIK_P]     = "p",
    [RUIK_A]     = "a",
    [RUIK_S]     = "s",
    [RUIK_D]     = "d",
    [RUIK_F]     = "f",
    [RUIK_G]     = "g",
    [RUIK_H]     = "h",
    [RUIK_J]     = "j",
    [RUIK_K]     = "k",
    [RUIK_L]     = "l",
    [RUIK_Z]     = "z",
    [RUIK_X]     = "x",
    [RUIK_C]     = "c",
    [RUIK_V]     = "v",
    [RUIK_B]     = "b",
    [RUIK_N]     = "n",
    [RUIK_M]     = "m",
/* WHITESPACE */
    [RUIK_COMMA] = ",",
    [RUIK_DOT]   = ".",
    [RUIK_SPACE] = "Space",
/* CONTROL CHARACTERS */
    [RUIK_ESC]   = "Esc"
};

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
        uint8_t key;
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
    char* title;
};

size_t rui_strlen(char*);

void rui_init(void);
void rui_screen_dims(int32_t*, int32_t*);
rui_window* rui_window_open(char*, int32_t, int32_t);
bool rui_process_events(rui_event*);
void rui_close_window(rui_window*);
#endif /* RANUI_H */

#ifdef RANUI_IMPLEMENTATION
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
typedef struct rui_xcb rui_native;
struct rui_xcb {
    xcb_connection_t* connection;
    xcb_screen_t*     screen;
    xcb_window_t      window;
    xcb_atom_t        wm_delete_window_atom;
    xcb_atom_t        wm_protocols_atom;
};
#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
typedef struct rui_win32 rui_native;
struct rui_win32 {
    HINSTANCE  hinstance;
    HWND       hwnd;
    rui_event* event;
};
#endif
rui_native dm;

size_t rui_strlen(char* s) {
    char* tmp;

    for (tmp = s; *s; ++s);

    return s - tmp;
}

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
void rui_init(void) {
#define WM_DEL_WIN "WM_DELETE_WINDOW"
#define WM_PROT    "WM_PROTOCOLS"
    dm.connection            = xcb_connect(NULL, NULL);
    dm.screen                = xcb_setup_roots_iterator(xcb_get_setup(dm.connection)).data;
    dm.window                = xcb_generate_id(dm.connection);
    dm.wm_delete_window_atom = xcb_intern_atom_reply(dm.connection,
                                                     xcb_intern_atom(dm.connection, 0, rui_strlen(WM_DEL_WIN), WM_DEL_WIN),
                                                     NULL)->atom;
    dm.wm_protocols_atom     = xcb_intern_atom_reply(dm.connection,
                                                     xcb_intern_atom(dm.connection, 1, rui_strlen(WM_PROT), WM_PROT),
                                                     NULL)->atom;
}
void rui_screen_dims(int32_t* width_out, int32_t* height_out) {
    *width_out  = dm.screen->width_in_pixels;
    *height_out = dm.screen->height_in_pixels;
}
rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
    rui_window*       window;
    uint32_t          mask;
    uint32_t          values[2];

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = dm.screen->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE;

    xcb_create_window(dm.connection, XCB_COPY_FROM_PARENT, dm.window, dm.screen->root, 0, 0,
                      width, height, 8, XCB_WINDOW_CLASS_INPUT_OUTPUT, dm.screen->root_visual,
                      mask, values);
    xcb_change_property(dm.connection, XCB_PROP_MODE_REPLACE, dm.window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING,
                        8, rui_strlen(title), title);
    xcb_change_property(dm.connection, XCB_PROP_MODE_REPLACE, dm.window, dm.wm_protocols_atom, XCB_ATOM_ATOM,
                        32, 1, &dm.wm_delete_window_atom);

    window = (rui_window*)malloc(sizeof(rui_window));
    if (!window) {
        return NULL;
    }

    window->instance = dm.connection;
    window->handle   = (void*)(uintptr_t)dm.window;
    window->title    = title;
    xcb_map_window(dm.connection, dm.window);
    xcb_flush(dm.connection);

    return window;
}
bool rui_process_events(rui_event* e) {
    bool                        status;
    xcb_generic_event_t*        xge;
    xcb_client_message_event_t* xcme;

    status = false;

    while((xge = xcb_poll_for_event(dm.connection))) {
        switch (xge->response_type & ~0x80) {
        case XCB_KEY_PRESS: {
            e->type = RUI_EVENT_TYPE_KEY_PRESSED;
            e->key  = ((xcb_key_press_event_t*)xge)->detail;
            status = true;
            break;
        }
        case XCB_KEY_RELEASE: {
            e->type = RUI_EVENT_TYPE_KEY_RELEASED;
            e->key  = ((xcb_key_release_event_t*)xge)->detail;
            status = true;
            break;
        }
        case XCB_CLIENT_MESSAGE: {
            xcme = (xcb_client_message_event_t*)xge;
            if (xcme->data.data32[0] == dm.wm_delete_window_atom) {
                e->type = RUI_EVENT_TYPE_WINDOW_CLOSED;
                status = true;
                break;
            }
        }
        default: {
            break;
        }
        }
        free(xge);
    }

    return status;
}
void rui_close_window(rui_window *window) {
    xcb_disconnect((xcb_connection_t*)window->instance);
    free(window);
    RUI_UNUSED(window);
}

#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
void rui_init(void) {
}
void rui_screen_dims(int32_t* width_out, int32_t* height_out) {
    *width_out  = GetSystemMetrics(SM_CXSCREEN);
    *height_out = GetSystemMetrics(SM_CYSCREEN);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    rui_event* e;

    // When called via DispatchMessage after TranslateMessage
    if ((e = dm.event)) {
        switch (Msg) {
        case WM_SIZE: {
            e->type = RUI_EVENT_TYPE_WINDOW_RESIZED;
            e->window_width = LOWORD(lParam);
            e->window_height = HIWORD(lParam);
            dm.event = NULL;
            return 0;
        }
        case WM_CHAR: {
            e->type = RUI_EVENT_TYPE_KEY_PRESSED;
            e->key = wParam;
            dm.event = NULL;
            return 0;
        }
        case WM_CLOSE: {
            e->type = RUI_EVENT_TYPE_WINDOW_CLOSED;
            dm.event = NULL;
            return 0;
        }
        default: {
            return DefWindowProcA(hwnd, Msg, wParam, lParam);
        }
        }
        // when called via PeekMessage or CreateWindowEx
    } else {
        switch (Msg) {
        default: {
            return DefWindowProcA(hwnd, Msg, wParam, lParam);
        }
        }
    }
}
rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
    int32_t swidth, sheight;
    HINSTANCE hinstance;
    WNDCLASSEXA wc;
    HWND hwnd;
    rui_window *window;

    hinstance = GetModuleHandle(NULL);

    wc = (WNDCLASSEXA){0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance;
    // load cursor otherwise it defaults to busy cursor icon
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.lpszClassName = RUI_CLASS_NAME;

    RegisterClassExA(&wc);

    hwnd = CreateWindowExA(0, RUI_CLASS_NAME, title, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           NULL, NULL, hinstance, NULL);

    rui_screen_dims(&swidth, &sheight);

    SetWindowPos(hwnd, HWND_TOP, width >> 1, height >> 1, swidth >> 1, sheight >> 1, 0);

    ShowWindow(hwnd, true);

    dm.hinstance = hinstance;
    dm.hwnd = hwnd;


    window = (rui_window*)malloc(sizeof(rui_window));
    window->instance = dm.hinstance;
    window->handle = (void*)(uintptr_t)dm.hwnd;
    window->title = title;

    return window;
}
bool rui_process_events(rui_event* e) {
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
void rui_close_window(rui_window* window) {
    DestroyWindow((HWND)window->handle);
    free(window);
    RUI_UNUSED(window);
}
#else
#error RUI_PLATFORM_ERROR
#endif /* Platform check */
#endif /* RANUI_IMPLEMENTATION */

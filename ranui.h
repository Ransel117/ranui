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

#define RUI_INLINE static inline

#define RUI_CLASS_NAME "ranui_program"
#define RUI_UNUSED(x) ((void)(x))

#if RUI_PLATFORM == RUI_PLATFORM_LINUX
// TODO: maybe do something regarding åöä and their other specifics
// xcb keycodes seem to be locale independent and is position based i think
// might need to test with different keyboards
#define RUIK_0 19
#define RUIK_1 10
#define RUIK_2 11
#define RUIK_3 12
#define RUIK_4 13
#define RUIK_5 14
#define RUIK_6 15
#define RUIK_7 16
#define RUIK_8 17
#define RUIK_9 18
#define RUIK_Q 24
#define RUIK_W 25
#define RUIK_E 26
#define RUIK_R 27
#define RUIK_T 28
#define RUIK_Y 29
#define RUIK_U 30
#define RUIK_I 31
#define RUIK_O 32
#define RUIK_P 33
#define RUIK_A 38
#define RUIK_S 39
#define RUIK_D 40
#define RUIK_F 41
#define RUIK_G 42
#define RUIK_H 43
#define RUIK_J 44
#define RUIK_K 45
#define RUIK_L 46
#define RUIK_Z 52
#define RUIK_X 53
#define RUIK_C 54
#define RUIK_V 55
#define RUIK_B 56
#define RUIK_N 57
#define RUIK_M 58
#define RUIK_COMMA 59
#define RUIK_DOT 60
#elif RUI_PLATFORM == RUI_PLATFORM_WINDOWS
// TODO: maybe do something better other than pure ASCII for windows
#define RUIK_0 '0'
#define RUIK_1 '1'
#define RUIK_2 '2'
#define RUIK_3 '3'
#define RUIK_4 '4'
#define RUIK_5 '5'
#define RUIK_6 '6'
#define RUIK_7 '7'
#define RUIK_8 '8'
#define RUIK_9 '9'
#define RUIK_Q 'q'
#define RUIK_W 'w'
#define RUIK_E 'e'
#define RUIK_R 'r'
#define RUIK_T 't'
#define RUIK_Y 'y'
#define RUIK_U 'u'
#define RUIK_I 'i'
#define RUIK_O 'o'
#define RUIK_P 'p'
#define RUIK_A 'a'
#define RUIK_S 's'
#define RUIK_D 'd'
#define RUIK_F 'f'
#define RUIK_G 'g'
#define RUIK_H 'h'
#define RUIK_J 'j'
#define RUIK_K 'k'
#define RUIK_L 'l'
#define RUIK_Z 'z'
#define RUIK_X 'x'
#define RUIK_C 'c'
#define RUIK_V 'v'
#define RUIK_B 'b'
#define RUIK_N 'n'
#define RUIK_M 'm'
#define RUIK_COMMA ','
#define RUIK_DOT '.'
#else
#error "not supported"
#endif

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
#include <xcb/xcb.h>
#include <signal.h>
#include <string.h>

typedef struct rui_x11 rui_x11;
struct rui_x11 {
    xcb_connection_t* connection;
    xcb_screen_t*     screen;
    xcb_atom_t        wm_delete_window_atom;
    xcb_atom_t        wm_protocols_atom;
    uint32_t window_width;
    uint32_t window_height;
};

rui_x11 dm;

RUI_INLINE void rui_init(void) {
    #define WM_DEL_WIN "WM_DELETE_WINDOW"
    #define WM_PROT    "WM_PROTOCOLS"
    dm.connection            = xcb_connect(NULL, NULL);
    dm.screen                = xcb_setup_roots_iterator(xcb_get_setup(dm.connection)).data;
    dm.wm_delete_window_atom = xcb_intern_atom_reply(dm.connection,
                                                     xcb_intern_atom(dm.connection, 0, strlen(WM_DEL_WIN), WM_DEL_WIN),
                                                     NULL)->atom;
    dm.wm_protocols_atom     = xcb_intern_atom_reply(dm.connection,
                                                     xcb_intern_atom(dm.connection, 1, strlen(WM_PROT), WM_PROT),
                                                     NULL)->atom;
}
RUI_INLINE void rui_screen_dims(int32_t* width_out, int32_t* height_out) {
    *width_out  = dm.screen->width_in_pixels;
    *height_out = dm.screen->height_in_pixels;
}
RUI_INLINE rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
    xcb_connection_t* c;
    xcb_screen_t*     s;
    xcb_window_t      w;
    rui_window*       window;
    uint32_t          mask;
    uint32_t          values[2];

    c = dm.connection;
    s = dm.screen;
    w = xcb_generate_id(c);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = s->black_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE;

    xcb_create_window(c, XCB_COPY_FROM_PARENT, w, s->root, 0, 0,
                      width, height, 8, XCB_WINDOW_CLASS_INPUT_OUTPUT, s->root_visual,
                      mask, values);
    xcb_change_property(c, XCB_PROP_MODE_REPLACE, w, XCB_ATOM_WM_NAME, XCB_ATOM_STRING,
                        8, strlen(title), title);
    xcb_change_property(c, XCB_PROP_MODE_REPLACE, w, dm.wm_protocols_atom, XCB_ATOM_ATOM,
                        32, 1, &dm.wm_delete_window_atom);

    window = (rui_window*)malloc(sizeof(rui_window));

    window->instance = c;
    window->handle   = (void*)(uintptr_t)w;

    xcb_map_window(c, w);
    xcb_flush(c);

    return window;
}
RUI_INLINE bool rui_process_events(rui_event* e) {
    bool                        status;
    xcb_generic_event_t*        xge;
    xcb_key_press_event_t*      xkpe;
    xcb_key_release_event_t*    xkre;
    xcb_client_message_event_t* xcme;

    status = false;

    while((xge = xcb_poll_for_event(dm.connection))) {
        switch (xge->response_type & ~0x80) {
        case XCB_KEY_PRESS: {
            xkpe    = (xcb_key_press_event_t*)xge;
            e->type = RUI_EVENT_TYPE_KEY_PRESSED;
            e->key  = xkpe->detail;
            status = true;
            break;
        }
        case XCB_KEY_RELEASE: {
            xkre    = (xcb_key_release_event_t*)xge;
            e->type = RUI_EVENT_TYPE_KEY_RELEASED;
            e->key  = xkre->detail;
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
RUI_INLINE void rui_close_window(rui_window *window) {
    xcb_disconnect((xcb_connection_t*)window->instance);
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
RUI_INLINE rui_window* rui_window_open(char* title, int32_t width, int32_t height) {
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

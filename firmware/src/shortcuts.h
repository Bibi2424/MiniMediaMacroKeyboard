#ifndef SHORTCUTS_H
#define SHORTCUTS_H


#define WINDOWS true
#define MACOS   false


typedef enum {
    COPY = 0,
    PASTE = 1,
    UNDO = 2
} shortcut_t;


extern void run_shortcut(shortcut_t shortcut, bool os);

#endif

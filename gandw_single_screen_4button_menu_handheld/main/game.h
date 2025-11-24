#ifndef GAME_H
#define GAME_H

typedef struct {
    const char *name;
    const char *year;
    int img_width;
    int img_height;
    uint16_t text_color;
} Game;

extern Game games[];
extern int game_count;
void game_load(int index);

#endif

#ifndef GAME_H
#define GAME_H

typedef struct {
    const char *name;
    const char *year;
    int img_unit_width;
    int img_unit_height;
    int img_box_width;
    int img_box_height;
    int color_line;
    uint16_t text_color;
    uint16_t box_bg_color;
    uint16_t box_border_color;
} Game;

extern Game games[];
extern int game_count;
void game_load(int index);

#endif

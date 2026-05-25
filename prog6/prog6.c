#include <stdio.h>
#include <string.h>
#include <math.h>

#define ROWS 25
#define COLS 60

/* the circle lives here */
struct circle {
    int cx;       /* center x (column) */
    int cy;       /* center y (row)    */
    int radius;
};

/* ---- 1. clear the buffer to spaces ---- */
void init_buffer(char buf[ROWS][COLS])
{
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++)
            buf[r][c] = ' ';
    }
}

/*
 * ---- 2. plot the circle using midpoint (Bresenham) algorithm ----
 *
 * idea: start at (0, radius) and walk around one octant.
 * for each point (x,y) we get 8 symmetric points for free.
 * much faster than looping every pixel and checking distance.
 */
void draw_circle(char buf[ROWS][COLS], struct circle circ)
{
    int x = 0;
    int y = circ.radius;
    int d = 1 - circ.radius;   /* decision variable */

    /* lambda-ish helper -- plot one point if it's inside the buffer */
    #define PLOT(r, c) \
        do { \
            if ((r) >= 0 && (r) < ROWS && (c) >= 0 && (c) < COLS) \
                buf[r][c] = '*'; \
        } while(0)

    while (x <= y) {
        /* 8 octant reflections */
        PLOT(circ.cy + y,  circ.cx + x);
        PLOT(circ.cy - y,  circ.cx + x);
        PLOT(circ.cy + y,  circ.cx - x);
        PLOT(circ.cy - y,  circ.cx - x);
        PLOT(circ.cy + x,  circ.cx + y);
        PLOT(circ.cy - x,  circ.cx + y);
        PLOT(circ.cy + x,  circ.cx - y);
        PLOT(circ.cy - x,  circ.cx - y);

        /* update decision variable */
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    #undef PLOT
}

/* ---- 3. print the buffer row by row ---- */
void print_buffer(char buf[ROWS][COLS])
{
    int r, c;
    /* top border */
    printf("+");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n");

    for (r = 0; r < ROWS; r++) {
        printf("|");
        for (c = 0; c < COLS; c++)
            putchar(buf[r][c]);
        printf("|\n");
    }

    /* bottom border */
    printf("+");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n");
}

/* ---- 4. build a circle struct (constructor-ish) ---- */
struct circle make_circle(int cx, int cy, int radius)
{
    struct circle circ;
    circ.cx     = cx;
    circ.cy     = cy;
    circ.radius = radius;
    return circ;
}

/* ---- driver ---- */
int main(void)
{
    char buf[ROWS][COLS];
    struct circle c;

    /* center of the buffer, radius 10 */
    c = make_circle(COLS / 2, ROWS / 2, 10);

    init_buffer(buf);
    draw_circle(buf, c);
    print_buffer(buf);

    printf("\ncenter=(%d,%d)  radius=%d\n", c.cx, c.cy, c.radius);
    return 0;
}
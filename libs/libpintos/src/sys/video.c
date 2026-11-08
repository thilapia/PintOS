#include <pintos/sys/video.h>
#include <pintos/sys/memory.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER_ADDR 0xB8000

static volatile uint16_t *g_vga_buffer = (volatile uint16_t *)VGA_BUFFER_ADDR;
static size_t g_cursor_x = 0;
static size_t g_cursor_y = 0;

static inline uint8_t make_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

static inline uint16_t make_vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void k_vga_init(void) {
    g_cursor_x = 0;
    g_cursor_y = 0;
    k_vga_clear(VGA_COLOR_BLACK);
}

void k_vga_clear(vga_color_t bg) {
    uint8_t color = make_color(VGA_COLOR_WHITE, bg);
    uint16_t blank = make_vga_entry(' ', color);

    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        g_vga_buffer[i] = blank;
    }
    g_cursor_x = 0;
    g_cursor_y = 0;
}

void k_vga_putchar(char c, vga_color_t fg, vga_color_t bg) {
    uint8_t color = make_color(fg, bg);

    if (c == '\n') {
        g_cursor_x = 0;
        g_cursor_y++;
    } else {
        size_t index = g_cursor_y * VGA_WIDTH + g_cursor_x;
        g_vga_buffer[index] = make_vga_entry(c, color);
        g_cursor_x++;
    }

    if (g_cursor_x >= VGA_WIDTH) {
        g_cursor_x = 0;
        g_cursor_y++;
    }

    if (g_cursor_y >= VGA_HEIGHT) {
        g_cursor_y = 0;
    }
}

void k_vga_write(const char *str, vga_color_t fg, vga_color_t bg) {
    while (*str) {
        k_vga_putchar(*str++, fg, bg);
    }
}
#include "display_oled.h"
#include "inc/ssd1306.h"
#include <string.h>

static struct render_area frame_area;
static uint8_t display_buffer[ssd1306_buffer_length];

static void config_i2c_display() {
    i2c_init(I2C_PORT_DISPLAY, ssd1306_i2c_clock * 1000);
    gpio_set_function(SDA_PIN_DISPLAY, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN_DISPLAY, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN_DISPLAY);
    gpio_pull_up(SCL_PIN_DISPLAY);
}

void display_oled_init() {
    config_i2c_display();
    ssd1306_init();

    frame_area = (struct render_area){
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);
    display_oled_clear();
}

void display_oled_clear() {
    memset(display_buffer, 0, ssd1306_buffer_length);
    render_on_display(display_buffer, &frame_area);
}

void display_oled_show_text(const char* line1, const char* line2) {
    memset(display_buffer, 0, ssd1306_buffer_length);
    if (line1) {
        ssd1306_draw_string(display_buffer, 5, 16, (char*)line1);
    }
    if (line2) {
        ssd1306_draw_string(display_buffer, 5, 32, (char*)line2);
    }
    render_on_display(display_buffer, &frame_area);
}
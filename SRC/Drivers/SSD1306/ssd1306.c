#include <stdarg.h>
#include "ssd1306.h"
#include "ch32v00x.h"

#define BUFFER_SIZE             256

#define SSD1306_CB_DATA         ((uint8_t)0b11000000)
#define SSD1306_CB_DATA_STREAM  ((uint8_t)0b01000000)
#define SSD1306_CB_CMD          ((uint8_t)0b10000000)
#define SSD1306_CB_CMD_STREAM   ((uint8_t)0b00000000)
#define SSD1306_ADDRESS         ((uint8_t)0b01111000)

// @array Init command
const uint8_t INIT_SSD1306[] = {
  0, SSD1306_DISPLAY_OFF,                                         // 0xAE = Set Display OFF
  1, SSD1306_SET_MUX_RATIO, SSD1306_HEIGHT - 1,                                   // 0xA8 - 64MUX for 128 x 64 version
                                                                  //      - 32MUX for 128 x 32 version
  1, SSD1306_MEMORY_ADDR_MODE, 0x00,                              // 0x20 = Set Memory Addressing Mode
                                                                  // 0x00 - Horizontal Addressing Mode
                                                                  // 0x01 - Vertical Addressing Mode
                                                                  // 0x02 - Page Addressing Mode (RESET)
  2, SSD1306_SET_COLUMN_ADDR, 0x00, SSD1306_WIDTH - 1,            // 0x21 = Set Column Address, 0 - 127
  2, SSD1306_SET_PAGE_ADDR, 0x00, SSD1306_CALC_PAGE(SSD1306_HEIGHT) - 1, // 0x22 = Set Page Address, 0 - 7
  0, SSD1306_SET_START_LINE,                                      // 0x40
  1, SSD1306_DISPLAY_OFFSET, 0x00,                                // 0xD3
  0, SSD1306_SEG_REMAP_OP,                                        // 0xA0 / remap 0xA1
  0, SSD1306_COM_SCAN_DIR_OP,                                     // 0xC0 / remap 0xC8
  1, SSD1306_COM_PIN_CONF, 0x12,                                  // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
                                                                  //       0x12 - for 128 x 64 version
                                                                  //       0x02 - for 128 x 32 version
  1, SSD1306_SET_CONTRAST, 0x7F,                                  // 0x81, 0x7F - reset value (max 0xFF)
  0, SSD1306_DIS_ENT_DISP_ON,                                     // 0xA4
  0, SSD1306_DIS_NORMAL,                                          // 0xA6
  1, SSD1306_SET_OSC_FREQ, 0x80,                                  // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
  1, SSD1306_SET_PRECHARGE, 0xc2,                                 // 0xD9, higher value less blinking
                                                                  // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
  1, SSD1306_VCOM_DESELECT, 0x20,                                 // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
  1, SSD1306_SET_CHAR_REG, 0x14,                                  // 0x8D, Enable charge pump during display on
};

/* ======== data function ======== */

void _i2c_init() {
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    I2C_InitTSturcture.I2C_ClockSpeed = 400000;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0x02;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTSturcture );
    I2C_Cmd( I2C1, ENABLE );
    I2C_AcknowledgeConfig( I2C1, ENABLE );
}

void SSD1306_i2c_start() {
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, SSD1306_ADDRESS, I2C_Direction_Transmitter );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );
}

void SSD1306_i2c_stop() {
    I2C_GenerateSTOP( I2C1, ENABLE );
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
}

void SSD1306_i2c_raw_data(uint8_t byte) {
    I2C_SendData( I2C1, byte );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
}

void SSD1306_i2c_start_command_stream() {
    SSD1306_i2c_raw_data(SSD1306_CB_CMD_STREAM);
}

void SSD1306_i2c_start_data_stream() {
    SSD1306_i2c_raw_data(SSD1306_CB_DATA_STREAM);
}

void SSD1306_i2c_command(uint8_t cmd) {
    SSD1306_i2c_raw_data(SSD1306_CB_CMD);
    SSD1306_i2c_raw_data(cmd);
}

void SSD1306_i2c_data(uint8_t byte) {
    SSD1306_i2c_raw_data(SSD1306_CB_DATA);
    SSD1306_i2c_raw_data(byte);
}

/* ======== driver function ======== */

void SSD1306_init() {
    _i2c_init();
    uint16_t i = 0;
    uint16_t cmd_count = sizeof(INIT_SSD1306);
    SSD1306_i2c_start();
    SSD1306_i2c_start_command_stream();
    while (i < cmd_count) {
        uint8_t cmd_stop_before = i + 2 + INIT_SSD1306[i];
        i += 1;
        uint8_t cmd = INIT_SSD1306[i];
        i += 1;
        SSD1306_i2c_raw_data(cmd);
        while (i < cmd_stop_before) {
            SSD1306_i2c_raw_data(INIT_SSD1306[i]);
            i += 1;
        }
        i = cmd_stop_before;
    }
    SSD1306_i2c_stop();
}

void SSD1306_invert(uint8_t invert) {
    SSD1306_i2c_start();
    SSD1306_i2c_command(SSD1306_DIS_NORMAL + (invert > 0));
    SSD1306_i2c_stop();
}

void SSD1306_set_contrast(uint8_t value) {
    SSD1306_i2c_start();
    SSD1306_i2c_command(SSD1306_SET_CONTRAST);
    SSD1306_i2c_command(value);
    SSD1306_i2c_stop();
}

void SSD1306_display_on_off(uint8_t on_off) {
    SSD1306_i2c_start();
    SSD1306_i2c_command(SSD1306_DISPLAY_OFF + (on_off > 0));
    SSD1306_i2c_stop();
}

void SSD1306_set_data_window(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    SSD1306_i2c_start();
    SSD1306_i2c_start_command_stream();
    SSD1306_i2c_raw_data(SSD1306_SET_COLUMN_ADDR);
    SSD1306_i2c_raw_data(x);
    SSD1306_i2c_raw_data(x + w - 1);
    SSD1306_i2c_raw_data(SSD1306_SET_PAGE_ADDR);
    SSD1306_i2c_raw_data(SSD1306_CALC_PAGE(y));
    SSD1306_i2c_raw_data(SSD1306_CALC_PAGE(y + h) - 1);
    SSD1306_i2c_stop();
}

void SSD1306_fill_screen(uint8_t color) {
    uint16_t mem_size = SSD1306_CALC_BUF_SIZE(SSD1306_WIDTH, SSD1306_HEIGHT);
    uint16_t p;
    uint8_t byte = color ? 0xFF : 0x00;
    SSD1306_set_data_window(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT);
    SSD1306_i2c_start();
    SSD1306_i2c_start_data_stream();
    for (p = 0; p < mem_size; p ++) {
        SSD1306_i2c_raw_data(byte);
    }
    SSD1306_i2c_stop();
}

/* ======== render function ======== */

static uint8_t bufx = 0;
static uint8_t bufy = 0;
static uint8_t bufw = 0;
static uint8_t bufh = 0;
static uint8_t buffer[BUFFER_SIZE];

uint8_t SSD1306_alloc_area(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    uint16_t mem_size = SSD1306_CALC_BUF_SIZE(w, h);
    uint16_t p;
    if (mem_size > BUFFER_SIZE) return 0;
    for (p = 0; p < mem_size; p ++) {
        buffer[p] = (uint8_t)0;
    }
    bufx = x;
    bufy = y;
    bufw = w;
    bufh = h;
    return 1;
}

void SSD1306_write_area() {
    uint16_t mem_size = SSD1306_CALC_BUF_SIZE(bufw, bufh);
    uint16_t p;
    SSD1306_set_data_window(bufx, bufy, bufw, bufh);
    SSD1306_i2c_start();
    SSD1306_i2c_start_data_stream();
    for (p = 0; p < mem_size; p ++) {
        SSD1306_i2c_raw_data(buffer[p]);
    }
    SSD1306_i2c_stop();
}

void SSD1306_write_image_data(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *img) {
    uint16_t mem_size = SSD1306_CALC_BUF_SIZE(w, h);
    uint16_t p;
    SSD1306_set_data_window(x, y, w, h);
    SSD1306_i2c_start();
    SSD1306_i2c_start_data_stream();
    for (p = 0; p < mem_size; p ++) {
        SSD1306_i2c_raw_data(img[p]);
    }
    SSD1306_i2c_stop();
}

void SSD1306_pixel(uint8_t x, uint8_t y, uint8_t c) {
    if (x >= bufw || y > bufh) return;
    uint16_t index = (y >> 3) * bufw + x;
    uint8_t offset = y & 0x07;
    buffer[index] = (buffer[index] & ~(0x01 << offset)) | ((c != 0) << offset);
}

void SSD1306_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c) {
    x = (x > bufw) ? bufw : x;
    y = (y > bufh) ? bufh : y;
    w = (x + w > bufw) ? bufw - x : w;
    h = (y + h > bufh) ? bufh - y : h;
    while (h--) {
        uint8_t *b = &((uint8_t *)buffer)[(y >> 3) * bufw + x];
        uint8_t offset = y & 0x07;
        for (uint8_t ww = w; ww; --ww) {
            *b = (*b & ~(0x01 << offset)) | ((c != 0) << offset);
            ++b;
        }
        ++y;
    }
}

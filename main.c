#include <reg51.h>

// Define ports and pins
sbit LED = P1^0;           // LED on P1.0
sbit PIR = P1^1;           // PIR sensor on P1.1
sbit LDR = P1^2;           // LDR on P1.2 (via ADC simulation)
sbit SCL = P3^6;           // I2C Clock on P3.6
sbit SDA = P3^7;           // I2C Data on P3.7
sbit LCD_RS = P2^0;        // LCD RS on P2.0
sbit LCD_EN = P2^1;        // LCD Enable on P2.1
sfr LCD_DATA = P2;         // LCD Data on P2

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1275; j++);
}

// I2C functions
void I2C_Start() {
    SDA = 1; SCL = 1; SDA = 0; SCL = 0;
}

void I2C_Stop() {
    SDA = 0; SCL = 1; SDA = 1;
}

void I2C_Write(unsigned char data) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (data & 0x80) ? 1 : 0;
        SCL = 1; delay_ms(1); SCL = 0;
        data <<= 1;
    }
    SDA = 1; SCL = 1; delay_ms(1); SCL = 0;
}

unsigned char I2C_Read(bit ack) {
    unsigned char i, data = 0;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        SCL = 1; delay_ms(1);
        data = (data << 1) | SDA;
        SCL = 0;
    }
    SDA = ack ? 0 : 1;
    SCL = 1; delay_ms(1); SCL = 0;
    return data;
}

// LCD functions
void LCD_Command(unsigned char cmd) {
    LCD_DATA = cmd; LCD_RS = 0; LCD_EN = 1; delay_ms(2); LCD_EN = 0;
}

void LCD_Char(unsigned char ch) {
    LCD_DATA = ch; LCD_RS = 1; LCD_EN = 1; delay_ms(2); LCD_EN = 0;
}

void LCD_Init() {
    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) LCD_Char(*str++);
}

// RTC functions
void RTC_SetTime(unsigned char hour, unsigned char minute, unsigned char second) {
    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Write(second);
    I2C_Write(minute);
    I2C_Write(hour);
    I2C_Stop();
}

void RTC_GetTime(unsigned char *hour, unsigned char *minute, unsigned char *second) {
    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start();
    I2C_Write(0xD1);
    *second = I2C_Read(1);
    *minute = I2C_Read(1);
    *hour = I2C_Read(0);
    I2C_Stop();
}

// Simulated ADC function
unsigned int ADC_Read() {
    return (LDR) ? 150 : 50; // Example: 150 for night, 50 for day
}

void main() {
    unsigned char hour, minute, second;
    unsigned int adc_value;

    LCD_Init();
    RTC_SetTime(0x23, 0x59, 0x50); // Set initial time

    while (1) {
        RTC_GetTime(&hour, &minute, &second);

        LCD_Command(0x80);
        LCD_String("Time: ");
        LCD_Char('0' + (hour >> 4));
        LCD_Char('0' + (hour & 0x0F));
        LCD_Char(':');
        LCD_Char('0' + (minute >> 4));
        LCD_Char('0' + (minute & 0x0F));
        LCD_Char(':');
        LCD_Char('0' + (second >> 4));
        LCD_Char('0' + (second & 0x0F));

        adc_value = ADC_Read();

        if (PIR) { // Motion detected
            LED = 1;
            delay_ms(2000);
        } else if (adc_value < 100) { // Daytime
            LED = 0;
        } else { // Nighttime
            LED = 1;
        }
    }
}

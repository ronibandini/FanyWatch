/*
Fany Watch - EHS Electromagnetic hypersensitivity Watch Scanner
Roni Bandini @RoniBandini - December 2020
http://bandini.medium.com
MIT License 
*/

#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

int n=0;
int myScore;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);

// Encoded images 

static const unsigned char wifi[] = {
  0x00, 0x00, 0x00, 0xC0, 0x1F, 0x00, 0xF0, 0xFF, 0x01, 0x3C, 0xC0, 0x03, 
  0x0E, 0x06, 0x07, 0xE3, 0x7F, 0x0C, 0xF0, 0xF9, 0x00, 0x38, 0xC0, 0x01, 
  0x00, 0x0F, 0x00, 0xC0, 0x3F, 0x00, 0xC0, 0x70, 0x00, 0x00, 0x04, 0x00, 
  0x00, 0x0F, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  };

static const unsigned char faceok[] = {
  0x00, 0x00, 0x01, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 
  0x80, 0xFF, 0xFF, 0x01, 0xC0, 0x0F, 0xF0, 0x03, 0xE0, 0x03, 0xC0, 0x07, 
  0xF0, 0x00, 0x00, 0x0F, 0x78, 0x00, 0x00, 0x1E, 0x38, 0x00, 0x00, 0x1E, 
  0x1C, 0x08, 0x10, 0x3C, 0x1C, 0x1C, 0x38, 0x38, 0x1E, 0x3E, 0x7C, 0x78, 
  0x0E, 0x7F, 0xFE, 0x70, 0x0E, 0x7F, 0xFE, 0x70, 0x0F, 0x33, 0xCC, 0x70, 
  0x0E, 0x00, 0x00, 0xF0, 0x0E, 0x00, 0x00, 0x70, 0x0F, 0x00, 0x00, 0x70, 
  0x0E, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x70, 0x1E, 0x0C, 0x30, 0x78, 
  0x1C, 0x3E, 0x7C, 0x38, 0x3C, 0xFE, 0x3F, 0x3C, 0x38, 0xF8, 0x1F, 0x1C, 
  0x78, 0xE0, 0x0F, 0x1F, 0xF0, 0x00, 0x80, 0x0F, 0xE0, 0x03, 0xC0, 0x07, 
  0xC0, 0x0F, 0xF0, 0x03, 0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFE, 0x7F, 0x00, 
  0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, };
  
static const unsigned char facesick[] = {
  0x00, 0x80, 0x00, 0x00, 0x00, 0xF8, 0x3F, 0x00, 0x00, 0xFE, 0xFF, 0x00, 
  0x80, 0xFF, 0xFF, 0x01, 0xC0, 0x0F, 0xF0, 0x03, 0xE0, 0x03, 0xC0, 0x0F, 
  0xF0, 0x01, 0x00, 0x0F, 0x78, 0x40, 0x00, 0x1E, 0x38, 0x77, 0xCE, 0x3C, 
  0x3C, 0x7F, 0xFE, 0x39, 0x1C, 0x3E, 0x7C, 0x78, 0x1E, 0x3E, 0x7C, 0x70, 
  0x0E, 0x3E, 0xFC, 0x70, 0x0E, 0xFF, 0xEE, 0xF1, 0x0E, 0x63, 0xC6, 0xF0, 
  0x0E, 0x00, 0x00, 0x60, 0x0E, 0x00, 0x00, 0xE0, 0x0E, 0x00, 0x01, 0x70, 
  0x0E, 0xF8, 0x1F, 0xF0, 0x0E, 0xFC, 0x7F, 0x70, 0x1E, 0xFF, 0xFF, 0x70, 
  0x1C, 0x0F, 0xF0, 0x79, 0x3C, 0x06, 0xC0, 0x38, 0x38, 0x00, 0x00, 0x3C, 
  0x78, 0x00, 0x00, 0x1E, 0xF0, 0x00, 0x00, 0x0F, 0xE0, 0x03, 0xC0, 0x07, 
  0xC0, 0x0F, 0xE0, 0x07, 0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFE, 0xFF, 0x00, 
  0x00, 0xF8, 0x1F, 0x00, 0x00, 0x80, 0x00, 0x00, };

static const unsigned char  a1[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x9F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x1E, 0x00, 
  0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0x40, 0x18, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x30, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x20, 0x70, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x70, 0x00, 
  0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x78, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0xB7, 0x7F, 0x00, 0x00, 0x44, 0x14, 0x75, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xFC, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x80, 0x01, 0x00, 0x00, 
  0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xDF, 0x0F, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0xFF, 0x0D, 0x00, 0x80, 0x71, 0x8C, 0x01, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0x00, 0x80, 0x63, 0x8E, 0x01, 
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0xFE, 0x07, 0x00, 0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x03, 0x00, 0x00, 0x44, 0x42, 0x22, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

static const unsigned char  a2[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x1E, 0x00, 
  0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x30, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x70, 0x20, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x00, 0x00, 0x98, 0x01, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x70, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xB8, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x78, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x90, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0xB7, 0x7F, 0x00, 0x00, 0x44, 0x14, 0x75, 
  0x01, 0x00, 0x38, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xB8, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0xFC, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x9C, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x80, 0x01, 0x00, 0x00, 
  0x70, 0x00, 0x98, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xDF, 0x0F, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x20, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0xFF, 0x0E, 0x00, 0x80, 0x71, 0x8E, 0x01, 0x70, 0x00, 0xE0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0x00, 0x80, 0x63, 0x8C, 0x01, 
  0x70, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0xFE, 0x07, 0x00, 0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x03, 0x00, 0x00, 0x28, 0x42, 0x22, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

static const unsigned char a3[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xEF, 
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFC, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x1E, 0x00, 
  0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xC0, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0x00, 0x80, 0x01, 0xC0, 0x00, 0x60, 0x30, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xC0, 0x01, 
  0x30, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x01, 
  0x90, 0x03, 0xC8, 0x01, 0x70, 0x20, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x00, 0x00, 0x98, 0x01, 0x30, 0x03, 0x9C, 0x01, 0x78, 0x00, 0x70, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xB8, 0x03, 0x70, 0x07, 0xB8, 0x03, 
  0x78, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x30, 0x03, 
  0x70, 0x06, 0x98, 0x03, 0xF8, 0xB7, 0x7F, 0x00, 0x00, 0x44, 0x10, 0x74, 
  0x01, 0x00, 0x38, 0x03, 0x60, 0x07, 0x38, 0x03, 0xFC, 0xFF, 0x3F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xBC, 0x03, 0x30, 0x07, 0x98, 0x03, 
  0xFC, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x98, 0x01, 
  0x38, 0x03, 0x9C, 0x03, 0xE0, 0xFF, 0x1F, 0x00, 0x80, 0x01, 0x00, 0x00, 
  0x70, 0x00, 0xC8, 0x01, 0x90, 0x07, 0xC8, 0x01, 0xE0, 0xEF, 0x0F, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x20, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xC0, 0x01, 
  0xE0, 0xFF, 0x0E, 0x00, 0x80, 0x73, 0x8E, 0x01, 0x70, 0x00, 0xE0, 0x00, 
  0xC0, 0x01, 0xE0, 0x00, 0xE0, 0xFF, 0x07, 0x00, 0x80, 0x61, 0x8C, 0x01, 
  0x70, 0x00, 0x60, 0x00, 0x80, 0x00, 0xC0, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0xFE, 0x07, 0x00, 0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x03, 0x00, 0x00, 0x18, 0x42, 0x21, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

static const unsigned char a4[] =  {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 
  0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x80, 0xEF, 
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x07, 
  0x00, 0x00, 0x80, 0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x83, 0x01, 0x00, 0x00, 0x00, 0x50, 0x0C, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0xDF, 0xCF, 0x01, 0x00, 0x00, 0x00, 0xFC, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x1E, 0x1C, 
  0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xC0, 0x01, 0x18, 0x2E, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0x00, 0x80, 0x01, 0xC0, 0x00, 0x60, 0x30, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xC0, 0x01, 
  0x30, 0x70, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x01, 
  0x90, 0x03, 0xC8, 0x01, 0x70, 0x20, 0x70, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x00, 0x00, 0x98, 0x01, 0x30, 0x03, 0x9C, 0x01, 0x78, 0x00, 0x70, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0xB8, 0x03, 0x70, 0x07, 0xB8, 0x03, 
  0x78, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x30, 0x03, 
  0x70, 0x06, 0x98, 0x03, 0xF8, 0xDB, 0x7F, 0x00, 0x00, 0x24, 0x10, 0x74, 
  0x01, 0x00, 0x38, 0x03, 0x60, 0x07, 0x38, 0x03, 0xFC, 0xFF, 0x3F, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0xBC, 0x03, 0x30, 0x07, 0x98, 0x03, 
  0xFC, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x98, 0x01, 
  0x38, 0x03, 0x9C, 0x03, 0xE0, 0x7F, 0x1F, 0x00, 0x80, 0x01, 0x00, 0x00, 
  0x70, 0x00, 0xC8, 0x01, 0x90, 0x07, 0xC8, 0x01, 0xE0, 0xFF, 0x0F, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x01, 0x80, 0x03, 0xC0, 0x01, 
  0xE0, 0xFF, 0x0F, 0x00, 0x80, 0x73, 0x8E, 0x01, 0x60, 0x00, 0xE0, 0x00, 
  0xC0, 0x01, 0xE0, 0x00, 0xE0, 0xFF, 0x03, 0x00, 0x80, 0x61, 0x8C, 0x01, 
  0x70, 0x00, 0x60, 0x00, 0x80, 0x00, 0xC0, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0xFE, 0x07, 0x00, 0x80, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 
  0x00, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x03, 0x00, 0x00, 0x18, 0x42, 0x21, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

static const unsigned char facemiddle[] = {
  0x00, 0x60, 0x01, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x00, 
  0x80, 0x7F, 0xFF, 0x01, 0xE0, 0x07, 0xF0, 0x03, 0xE0, 0x01, 0xC0, 0x07, 
  0xF0, 0x00, 0x00, 0x0F, 0x78, 0x00, 0x00, 0x1E, 0x3C, 0x00, 0x00, 0x1E, 
  0x3C, 0x00, 0x00, 0x3C, 0x1E, 0x1E, 0x38, 0x38, 0x0E, 0x1C, 0x38, 0x78, 
  0x0E, 0x1C, 0x38, 0x70, 0x0E, 0x1C, 0x38, 0x70, 0x0F, 0x04, 0x28, 0x70, 
  0x0E, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x70, 
  0x0E, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x70, 0x0E, 0x08, 0x10, 0x78, 
  0x1E, 0xFC, 0x3F, 0x38, 0x3C, 0xFC, 0x3F, 0x3C, 0x3C, 0xFC, 0x3F, 0x1C, 
  0x78, 0x00, 0x00, 0x1E, 0xF0, 0x00, 0x00, 0x0F, 0xE0, 0x03, 0xC0, 0x07, 
  0xE0, 0x07, 0xF0, 0x03, 0x80, 0xBF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0x00, 
  0x00, 0xFC, 0x1F, 0x00, 0x00, 0x40, 0x01, 0x00, };

static const unsigned char logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x5A, 0x04, 0xF8, 0xF3, 0xF1, 0xDC, 0x71, 0x3C, 0x8E, 0xF7, 
  0xFB, 0x8F, 0x0F, 0xEF, 0x01, 0x00, 0x00, 0x11, 0xF8, 0xFB, 0xF1, 0xDC, 
  0x73, 0x3C, 0xCF, 0xF7, 0xFB, 0xCF, 0xBF, 0xEF, 0x01, 0x00, 0x12, 0x02, 
  0xF8, 0xF3, 0xF3, 0xDC, 0x73, 0x7C, 0xCF, 0xF3, 0xFB, 0xEF, 0x3F, 0xE7, 
  0x01, 0x70, 0x80, 0x04, 0xF8, 0xFB, 0xF3, 0x9D, 0x73, 0x3C, 0xDF, 0xF3, 
  0xFF, 0xEF, 0xBD, 0xEF, 0x01, 0xF8, 0x80, 0x00, 0x78, 0xF8, 0xF3, 0x9E, 
  0x3B, 0x78, 0xCF, 0xF7, 0xE3, 0xE3, 0x79, 0xEF, 0x01, 0xE0, 0x00, 0x00, 
  0x78, 0xB8, 0xF3, 0x9D, 0x3B, 0x78, 0xDF, 0x73, 0xC7, 0xF1, 0x7D, 0xEF, 
  0x01, 0x06, 0x00, 0x00, 0x78, 0xB8, 0xF3, 0x9F, 0x3B, 0x38, 0xDF, 0x73, 
  0xC7, 0xE3, 0xBD, 0xFF, 0x01, 0x3F, 0xE0, 0x01, 0xF8, 0xBB, 0xF3, 0x1F, 
  0x3F, 0xF8, 0xDF, 0x73, 0xE7, 0xF3, 0x01, 0xFF, 0x01, 0x1F, 0xF0, 0x01, 
  0xF8, 0xBB, 0xF3, 0x1F, 0x1F, 0x38, 0xDF, 0x7B, 0xC7, 0xE3, 0x81, 0xFF, 
  0x81, 0x1F, 0x80, 0x01, 0xF8, 0xBB, 0xF7, 0x1F, 0x1F, 0x78, 0xDF, 0x7B, 
  0xC7, 0xE3, 0x01, 0xFF, 0xC1, 0x0F, 0x38, 0x00, 0xF8, 0x3F, 0xF7, 0x1F, 
  0x1F, 0xF0, 0xFB, 0x39, 0xEF, 0xF3, 0xBD, 0xEF, 0xE1, 0x07, 0x7C, 0x00, 
  0x78, 0x9C, 0xF7, 0x3F, 0x0E, 0xF8, 0xFB, 0x39, 0xC7, 0xE1, 0x7D, 0xEF, 
  0xE1, 0x03, 0x7E, 0x00, 0x78, 0xFC, 0xF7, 0x1F, 0x1E, 0xF0, 0xFB, 0xFB, 
  0xEF, 0xE3, 0xBD, 0xE7, 0xF1, 0x03, 0x3E, 0x00, 0x78, 0xFC, 0xF7, 0x1E, 
  0x1F, 0xF0, 0xFB, 0xF9, 0xEF, 0xE3, 0xBD, 0xEF, 0xF1, 0x03, 0x1F, 0x00, 
  0x78, 0xFC, 0xF7, 0x1E, 0x0E, 0xF0, 0xF1, 0xFD, 0xCF, 0xE1, 0x3D, 0xEF, 
  0xF1, 0x83, 0x1F, 0x00, 0x78, 0x1C, 0x77, 0x1E, 0x1E, 0xF0, 0xF9, 0x39, 
  0xCE, 0xE3, 0xBF, 0xEF, 0xE1, 0xFF, 0x0F, 0x00, 0x78, 0x1E, 0xFF, 0x1E, 
  0x1E, 0xF0, 0xF1, 0x3D, 0xEF, 0xC3, 0x9F, 0xEF, 0xE1, 0xFF, 0x07, 0x00, 
  0x78, 0x3C, 0x7F, 0x1C, 0x1E, 0xF0, 0xF1, 0x3D, 0xEE, 0x83, 0x0F, 0xEF, 
  0xC1, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

void setup() {

  Serial.begin(115200);
  Serial.println("Fany Watch started...");   
  Serial.println("Roni Bandini - Argentina");   

  u8g2.begin(); 
  
  u8g2.clearBuffer();   
  u8g2.setFont(u8g2_font_t0_11_tf);  
  u8g2.drawStr(0,10,"Fany Watch");  
  u8g2.drawStr(0,20,"Roni Bandini");   
  u8g2.drawStr(0,30,"Starting...");   
  u8g2.sendBuffer();           
  delay(2000);
    
  // WiFi station mode
  WiFi.mode(WIFI_STA);  
  delay(100);
  
}

void loop() {  
  
  // Logo
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, logo);
  u8g2.sendBuffer();          
  delay(2000);
  
  // Animation
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, a1);
  u8g2.sendBuffer();          
  delay(200);
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, a2);
  u8g2.sendBuffer();          
  delay(200);
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, a3);
  u8g2.sendBuffer();          
  delay(150);
  u8g2.clearBuffer();               
  u8g2.drawXBM(0,0, 128, 32, a4);
  u8g2.sendBuffer();          
  delay(150);
  
  n = WiFi.scanNetworks();
  
  // Display nearby routers
  for (int i = 0; i < n; ++i) {      

      u8g2.clearBuffer();    
      u8g2.drawStr(0,12,"Routers nearby");                    
      u8g2.drawStr(0,22,WiFi.SSID(i).substring(0,16).c_str());       
      u8g2.drawStr(0,32,"Strength:");  
      u8g2.drawStr(55,32,String(WiFi.RSSI(i)).c_str());             
      u8g2.drawXBM(100,10, 20, 16, wifi);                                          
      u8g2.sendBuffer();                  
      delay(150);
   }   
   
  // Real time damage calculation  
  int myCounter=0;
  
  while (myCounter<10){
        
        n = WiFi.scanNetworks();
        myScore=0;

        // get damage score
        for (int i = 0; i < n; ++i) {   
           
          int myRSSI=WiFi.RSSI(i)*-1;      
      
          if (myRSSI>0 and myRSSI<=40)
          {
            myScore=myScore+30;
            }
            
          if (myRSSI>40 and myRSSI<=60)
          {
            myScore=myScore+5;
          }
    
          if (myRSSI>60)
          {
            myScore=myScore+1;
          }                                                                                                          
            
        }               

        u8g2.clearBuffer();    
        u8g2.drawStr(0,12,"Fany Watch");                        
        u8g2.drawStr(0,22,"Routers:");        
        u8g2.drawStr(50,22,String(n).c_str()); 
        u8g2.drawStr(0,32,"Damage:");    
        u8g2.drawStr(44,32,String(myScore).c_str());    
        
        // Display emoji
        if (myScore>30){
          u8g2.drawXBM(90, 0,  32,32, facesick);        
        }
        
        if (myScore>10 and myScore<31)
        {
          u8g2.drawXBM(90, 0,  32,32, facemiddle);        
        }

        if (myScore<11)
        {
          u8g2.drawXBM(90, 0,  32,32, faceok);        
        }
                                                                                                                                             
        u8g2.sendBuffer(); 
        myCounter++;
        delay(25);
  }


} // loop

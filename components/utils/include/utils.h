#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_rom_md5.h"
#include "esp_timer.h"
#include "esp_mac.h"

#define INT32_BSWAP(i) \
    (int32_t)((((int32_t)i[0] << 16) & 0xffff0000) | (i[1] & 0xffff))

#define UINT32_BSWAP(i) \
    (uint32_t)((((uint32_t)i[0] << 16) & 0xffff0000) | (i[1] & 0xffff))

// (int32_t)((*i & 0x0000ffff) << 16) | ((*i & 0xffff0000) >> 16);

void utils_read_mac(char *buf, esp_mac_type_t type);
long utils_map(long x, long in_min, long in_max, long out_min, long out_max);
void utils_ip_to_tring(uint32_t ip, char *buf);
void utils_byteSwapArray(char *target, int size);
float utils_round(float var);
void utils_md5(char *src, char *dest);
int utils_get_day(int timezone);
int utils_get_hour(int timezone);
// void utils_timeFormated(uint32_t unixtimestamp, char *buff);
void utils_timeFormated(uint32_t unixtimestamp, char *buff);

int64_t IRAM_ATTR utils_millis();
int64_t IRAM_ATTR micros();
void IRAM_ATTR utils_delayMicroseconds(uint32_t us);

float int32ToIeeefloat(int32_t ieee754_bits);
uint16_t u16(uint16_t i);
int16_t i16(uint16_t i);
int16_t i16(int16_t i);

uint32_t to_u32(uint32_t *i);
int32_t to_i32(int32_t *i);

uint32_t utils_unixTimeGet();
int utils_get_daily_code();

#endif
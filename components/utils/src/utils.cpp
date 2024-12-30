#include "utils.h"

void utils_read_mac(char *buf, esp_mac_type_t type)
{
    uint8_t mac_addr[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(mac_addr, type));
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2],
            mac_addr[3], mac_addr[4], mac_addr[5]);
}

long utils_map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void utils_ip_to_tring(uint32_t ip, char *buf)
{
    sprintf(buf, "%ld.%ld.%ld.%ld",
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            (ip)&0xFF);
}

int64_t IRAM_ATTR utils_millis()
{
    return (esp_timer_get_time() / 1000);
}

int64_t IRAM_ATTR micros()
{
    return esp_timer_get_time();
}

void IRAM_ATTR utils_delayMicroseconds(uint32_t number_of_us)
{
    uint64_t microseconds = (uint64_t)esp_timer_get_time();
    if (number_of_us)
    {
        uint64_t total = (microseconds + number_of_us);
        if (microseconds > total)
        {
            while ((uint64_t)esp_timer_get_time() > total)
            {
                __asm__ __volatile__("nop");
            }
        }
        while ((uint64_t)esp_timer_get_time() < total)
        {
            __asm__ __volatile__("nop");
        }
    }
}

void utils_byteSwapArray(char *target, int size)
{
    for (int i = 1; i <= size - 1;)
    {
        char swap = *(target + i);
        *(target + i) = *(target + i - 1);
        *(target + i - 1) = swap;
        i += 2;
    }
}

float utils_round(float var)
{
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

void utils_md5(char *src, char *dest)
{
    uint8_t buff[32] = {};
    md5_context_t context;
    esp_rom_md5_init(&context);
    esp_rom_md5_update(&context, src, strlen(src));
    esp_rom_md5_final(buff, &context);

    char *target = dest;
    for (int i = 0; i < 16; i++)
    {
        target += sprintf(target, "%02x", buff[i]);
    }
}

float int32ToIeeefloat(int32_t ieee754_bits)
{
    float flt;
    *((int *)&flt) = ieee754_bits;
    return flt;
}

uint16_t u16(uint16_t i) { return __bswap16(i); }
int16_t i16(uint16_t i) { return (int16_t)__bswap16(i); }
int16_t i16(int16_t i) { return (int16_t)__bswap16((uint16_t)i); }

uint32_t to_u32(uint32_t *i)
{
    return ((*i & 0x0000ffff) << 16) | ((*i & 0xffff0000) >> 16);
}

// convert to int32_t
int32_t to_i32(int32_t *i)
{
    return (int32_t)((*i & 0x0000ffff) << 16) | ((*i & 0xffff0000) >> 16);
}

int utils_get_day(int timezone)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    now += timezone * 3600;
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_mday;
}

int utils_get_hour(int timezone)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    now += timezone * 3600;
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_hour;
}

void utils_timeFormated(uint32_t unixtimestamp, char *buff)
{
    time_t now = unixtimestamp;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    sprintf(buff, "20%02d-%02d-%02d %02d:%02d:%02d", timeinfo.tm_year % 100, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, 0); // sec = 0
}
// void utils_timeFormated(uint32_t unixtimestamp, char *buff)
// {
//     time_t now = unixtimestamp;
//     struct tm timeinfo;
//     time(&now);
//     localtime_r(&now, &timeinfo);
//     sprintf(buff, "20%02d-%02d-%02d %02d:%02d:%02d", timeinfo.tm_year % 100, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, 0); // sec = 0
// }

uint32_t utils_unixTimeGet()
{
    time_t now;
    time(&now);
    return now;
}

int utils_get_daily_code()
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return ((timeinfo.tm_year % 100) * 10000 + (timeinfo.tm_mon + 1) * 100 + timeinfo.tm_mday); // 240624 (int)
}

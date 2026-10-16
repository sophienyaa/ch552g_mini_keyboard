#ifndef __DATAFLASH_H__
#define __DATAFLASH_H__

#include <stdint.h>
#include <stdbool.h>

#define DATAFLASH_SIZE        128
#define DATAFLASH_MAGIC_0     0x4B // 'K'
#define DATAFLASH_MAGIC_1     0x42 // 'B'
#define DATAFLASH_VERSION     1

// DataFlash Memory Offsets
#define DF_OFFSET_MAGIC0      0
#define DF_OFFSET_MAGIC1      1
#define DF_OFFSET_VERSION     2
#define DF_OFFSET_ACTIVE_PROF 3
#define DF_OFFSET_LED_MODE    4
#define DF_OFFSET_CHECKSUM    5
#define DF_OFFSET_PROFILES    8   // Profiles data starts at offset 8 (24 bytes each)

#ifdef __cplusplus
extern "C" {
#endif

uint8_t dataflash_read_byte(uint8_t addr);
void dataflash_write_byte(uint8_t addr, uint8_t dat);

void dataflash_read(uint8_t addr, uint8_t *buf, uint8_t len);
void dataflash_write(uint8_t addr, const uint8_t *buf, uint8_t len);

bool dataflash_is_valid(void);
void dataflash_init(void);

#ifdef __cplusplus
}
#endif

#endif // __DATAFLASH_H__


#include "dataflash.h"
#include "neo/ch554.h"

uint8_t dataflash_read_byte(uint8_t addr) {
  if (addr >= DATAFLASH_SIZE) {
    return 0;
  }
  ROM_ADDR = ((uint16_t)addr << 1);
  ROM_CTRL = ROM_CMD_READ;
  return ROM_DATA_L;
}

void dataflash_write_byte(uint8_t addr, uint8_t dat) {
  if (addr >= DATAFLASH_SIZE) {
    return;
  }
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  GLOBAL_CFG |= bDATA_WE;

  ROM_ADDR = ((uint16_t)addr << 1);
  ROM_DATA_L = dat;
  ROM_CTRL = ROM_CMD_WRITE;

  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  GLOBAL_CFG &= ~bDATA_WE;
}

void dataflash_read(uint8_t addr, uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    buf[i] = dataflash_read_byte(addr + i);
  }
}

void dataflash_write(uint8_t addr, const uint8_t *buf, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    dataflash_write_byte(addr + i, buf[i]);
  }
}

bool dataflash_is_valid(void) {
  return (dataflash_read_byte(DF_OFFSET_MAGIC0) == DATAFLASH_MAGIC_0 &&
          dataflash_read_byte(DF_OFFSET_MAGIC1) == DATAFLASH_MAGIC_1 &&
          dataflash_read_byte(DF_OFFSET_VERSION) == DATAFLASH_VERSION);
}


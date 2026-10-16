#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_VERSION       1

// Protocol Commands
#define CMD_GET_INFO           0x01
#define CMD_GET_PROFILE        0x02
#define CMD_SET_PROFILE        0x03
#define CMD_SET_ACTIVE_PROFILE 0x04
#define CMD_RESTORE_DEFAULTS   0x05
#define CMD_JUMP_BOOTLOADER    0x06

#define FEATURE_REPORT_ID      3
#define FEATURE_REPORT_LEN     28

#ifdef __cplusplus
extern "C" {
#endif

// Returns pointer to response buffer and sets response len for GET_REPORT
__xdata uint8_t *protocol_handle_get_report(uint8_t report_id, uint8_t *len);

// Prepares protocol receiver for incoming SET_REPORT data
void protocol_start_set_report(uint8_t report_id, uint16_t total_len);

// Feeds incoming chunks from EP0 into feature buffer; processes command when complete
void protocol_receive_chunk(const uint8_t *chunk, uint8_t chunk_len);

#ifdef __cplusplus
}
#endif

#endif // __PROTOCOL_H__


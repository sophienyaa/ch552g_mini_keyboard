#include "protocol.h"
#include "keyboard.h"
#include "util.h"
#include <string.h>

__xdata uint8_t feature_rx_buf[FEATURE_REPORT_LEN];
__xdata uint8_t feature_tx_buf[FEATURE_REPORT_LEN];

static uint8_t rx_offset = 0;
static uint16_t rx_expected_len = 0;

void protocol_start_set_report(uint8_t report_id, uint16_t total_len) {
  (void)report_id;
  rx_offset = 0;
  rx_expected_len = total_len;
  memset(feature_rx_buf, 0, sizeof(feature_rx_buf));
}

static void protocol_process_command(void) {
  uint8_t offset = 0;
  // If host includes Report ID (0x03) as first byte, advance offset
  if (feature_rx_buf[0] == FEATURE_REPORT_ID) {
    offset = 1;
  }

  uint8_t cmd = feature_rx_buf[offset];
  uint8_t target = feature_rx_buf[offset + 1];

  memset(feature_tx_buf, 0, sizeof(feature_tx_buf));

  switch (cmd) {
    case CMD_GET_INFO: {
      feature_tx_buf[0] = CMD_GET_INFO;
      feature_tx_buf[1] = keyboard_get_active_profile();
      feature_tx_buf[2] = NUM_PROFILES;
      feature_tx_buf[3] = PROTOCOL_VERSION;
      feature_tx_buf[4] = 'K';
      feature_tx_buf[5] = 'B';
      break;
    }

    case CMD_GET_PROFILE: {
      if (target >= NUM_PROFILES) {
        target = 0;
      }
      feature_tx_buf[0] = CMD_GET_PROFILE;
      feature_tx_buf[1] = target;
      compact_profile_t *prof = keyboard_get_profile(target);
      if (prof != 0) {
        memcpy(&feature_tx_buf[2], prof, sizeof(compact_profile_t));
      }
      break;
    }

    case CMD_SET_PROFILE: {
      if (target < NUM_PROFILES) {
        compact_profile_t new_prof;
        memcpy(&new_prof, &feature_rx_buf[offset + 2], sizeof(compact_profile_t));
        keyboard_update_profile(target, &new_prof);
      }
      feature_tx_buf[0] = CMD_SET_PROFILE;
      feature_tx_buf[1] = target;
      feature_tx_buf[2] = 0; // Success
      break;
    }

    case CMD_SET_ACTIVE_PROFILE: {
      if (target < NUM_PROFILES) {
        keyboard_set_active_profile(target);
      }
      feature_tx_buf[0] = CMD_SET_ACTIVE_PROFILE;
      feature_tx_buf[1] = keyboard_get_active_profile();
      feature_tx_buf[2] = 0;
      break;
    }

    case CMD_RESTORE_DEFAULTS: {
      keyboard_load_factory_defaults();
      feature_tx_buf[0] = CMD_RESTORE_DEFAULTS;
      feature_tx_buf[1] = 0;
      break;
    }

    case CMD_JUMP_BOOTLOADER: {
      feature_tx_buf[0] = CMD_JUMP_BOOTLOADER;
      feature_tx_buf[1] = 0;
      BOOT_now();
      break;
    }

    default:
      feature_tx_buf[0] = 0xFF; // Error / unknown command
      break;
  }
}

void protocol_receive_chunk(const uint8_t *chunk, uint8_t chunk_len) {
  for (uint8_t i = 0; i < chunk_len && rx_offset < FEATURE_REPORT_LEN; i++) {
    feature_rx_buf[rx_offset++] = chunk[i];
  }

  if (rx_offset >= rx_expected_len || rx_offset >= FEATURE_REPORT_LEN) {
    protocol_process_command();
  }
}

__xdata uint8_t *protocol_handle_get_report(uint8_t report_id, uint8_t *len) {
  (void)report_id;
  *len = FEATURE_REPORT_LEN;
  return feature_tx_buf;
}


/// DBC file: _can_dbc\243.dbc    Self node: NOONE
/// This file should be included by a source file, for example: #include "generated.c"
#include <stdbool.h>
#include <stdint.h>



/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} msg_hdr_t; 

static const msg_hdr_t DRIVER_TX_DRIVER_KILL_SWITCH_HDR =     {    0, 0 };
static const msg_hdr_t DRIVER_TX_DRIVER_RESET_HDR =           {    1, 0 };
static const msg_hdr_t DRIVER_TX_DRIVER_SYNC_ACK_HDR =        {    2, 0 };
static const msg_hdr_t MOTORIO_TX_MOTORIO_SYNC_HDR =          {    3, 0 };
static const msg_hdr_t SENSOR_TX_SENSOR_SYNC_HDR =            {    4, 0 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_SYNC_HDR =      {    5, 0 };
static const msg_hdr_t GEO_TX_GEO_SYNC_HDR =                  {    6, 0 };
static const msg_hdr_t MOTORIO_TX_MOTORIO_HEARTBEAT_HDR =     {    7, 0 };
static const msg_hdr_t SENSOR_TX_SENSOR_HEARTBEAT_HDR =       {    8, 0 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_HDR = {    9, 0 };
static const msg_hdr_t GEO_TX_GEO_HEARTBEAT_HDR =             {   10, 0 };
static const msg_hdr_t MOTORIO_TX_MOTORIO_RUNMODE_HDR =       {   11, 1 };
static const msg_hdr_t SENSOR_TX_SENSOR_SONARS_HDR =          {   12, 6 };
static const msg_hdr_t MOTORIO_TX_MOTORIO_DIRECTION_HDR =     {   13, 2 };
static const msg_hdr_t DRIVER_TX_DRIVER_CHECKPOINT_REQ_HDR =  {   14, 1 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_HDR = {   15, 1 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_HDR = {   16, 1 };
static const msg_hdr_t DRIVER_TX_DRIVER_LOC_UPDATE_HDR =      {   17, 8 };
static const msg_hdr_t GEO_TX_GEO_SPEED_ANGLE_HDR =           {   18, 5 };
static const msg_hdr_t GEO_TX_GEO_LOC_DATA_HDR =              {   19, 8 };
static const msg_hdr_t SENSOR_TX_SENSOR_LIGHT_BAT_HDR =       {   20, 2 };


/// Not generating 'DRIVER_TX_DRIVER_KILL_SWITCH_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_DRIVER_RESET_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_DRIVER_SYNC_ACK_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'MOTORIO_TX_MOTORIO_SYNC_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'SENSOR_TX_SENSOR_SYNC_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'BLUETOOTH_TX_BLUETOOTH_SYNC_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'GEO_TX_GEO_SYNC_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'MOTORIO_TX_MOTORIO_HEARTBEAT_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'SENSOR_TX_SENSOR_HEARTBEAT_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'GEO_TX_GEO_HEARTBEAT_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'MOTORIO_TX_MOTORIO_RUNMODE_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'SENSOR_TX_SENSOR_SONARS_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'MOTORIO_TX_MOTORIO_DIRECTION_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_DRIVER_CHECKPOINT_REQ_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_DRIVER_LOC_UPDATE_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'GEO_TX_GEO_SPEED_ANGLE_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'GEO_TX_GEO_LOC_DATA_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'SENSOR_TX_SENSOR_LIGHT_BAT_t' since we are not the sender or a recipient of any of its signals

/// These 'externs' need to be defined in a source file of your project

/// Not generating code for DRIVER_TX_DRIVER_KILL_SWITCH_encode() since the sender is DRIVER and we are NOONE

/// Not generating code for DRIVER_TX_DRIVER_RESET_encode() since the sender is DRIVER and we are NOONE

/// Not generating code for DRIVER_TX_DRIVER_SYNC_ACK_encode() since the sender is DRIVER and we are NOONE

/// Not generating code for MOTORIO_TX_MOTORIO_SYNC_encode() since the sender is MOTORIO and we are NOONE

/// Not generating code for SENSOR_TX_SENSOR_SYNC_encode() since the sender is SENSOR and we are NOONE

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_SYNC_encode() since the sender is BLUETOOTH and we are NOONE

/// Not generating code for GEO_TX_GEO_SYNC_encode() since the sender is GEO and we are NOONE

/// Not generating code for MOTORIO_TX_MOTORIO_HEARTBEAT_encode() since the sender is MOTORIO and we are NOONE

/// Not generating code for SENSOR_TX_SENSOR_HEARTBEAT_encode() since the sender is SENSOR and we are NOONE

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_encode() since the sender is BLUETOOTH and we are NOONE

/// Not generating code for GEO_TX_GEO_HEARTBEAT_encode() since the sender is GEO and we are NOONE

/// Not generating code for MOTORIO_TX_MOTORIO_RUNMODE_encode() since the sender is MOTORIO and we are NOONE

/// Not generating code for SENSOR_TX_SENSOR_SONARS_encode() since the sender is SENSOR and we are NOONE

/// Not generating code for MOTORIO_TX_MOTORIO_DIRECTION_encode() since the sender is MOTORIO and we are NOONE

/// Not generating code for DRIVER_TX_DRIVER_CHECKPOINT_REQ_encode() since the sender is DRIVER and we are NOONE

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_encode() since the sender is BLUETOOTH and we are NOONE

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_encode() since the sender is BLUETOOTH and we are NOONE

/// Not generating code for DRIVER_TX_DRIVER_LOC_UPDATE_encode() since the sender is DRIVER and we are NOONE

/// Not generating code for GEO_TX_GEO_SPEED_ANGLE_encode() since the sender is GEO and we are NOONE

/// Not generating code for GEO_TX_GEO_LOC_DATA_encode() since the sender is GEO and we are NOONE

/// Not generating code for SENSOR_TX_SENSOR_LIGHT_BAT_encode() since the sender is SENSOR and we are NOONE

/// Not generating code for DRIVER_TX_DRIVER_KILL_SWITCH_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_RESET_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_SYNC_ACK_decode() since we are not the recipient of any of its signals

/// Not generating code for MOTORIO_TX_MOTORIO_SYNC_decode() since we are not the recipient of any of its signals

/// Not generating code for SENSOR_TX_SENSOR_SYNC_decode() since we are not the recipient of any of its signals

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_SYNC_decode() since we are not the recipient of any of its signals

/// Not generating code for GEO_TX_GEO_SYNC_decode() since we are not the recipient of any of its signals

/// Not generating code for MOTORIO_TX_MOTORIO_HEARTBEAT_decode() since we are not the recipient of any of its signals

/// Not generating code for SENSOR_TX_SENSOR_HEARTBEAT_decode() since we are not the recipient of any of its signals

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_decode() since we are not the recipient of any of its signals

/// Not generating code for GEO_TX_GEO_HEARTBEAT_decode() since we are not the recipient of any of its signals

/// Not generating code for MOTORIO_TX_MOTORIO_RUNMODE_decode() since we are not the recipient of any of its signals

/// Not generating code for SENSOR_TX_SENSOR_SONARS_decode() since we are not the recipient of any of its signals

/// Not generating code for MOTORIO_TX_MOTORIO_DIRECTION_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_CHECKPOINT_REQ_decode() since we are not the recipient of any of its signals

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_decode() since we are not the recipient of any of its signals

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_LOC_UPDATE_decode() since we are not the recipient of any of its signals

/// Not generating code for GEO_TX_GEO_SPEED_ANGLE_decode() since we are not the recipient of any of its signals

/// Not generating code for GEO_TX_GEO_LOC_DATA_decode() since we are not the recipient of any of its signals

/// Not generating code for SENSOR_TX_SENSOR_LIGHT_BAT_decode() since we are not the recipient of any of its signals

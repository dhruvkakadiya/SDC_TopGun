/// DBC file: TopGun_12_38am_11_16_2015.dbc    Self node: DRIVER
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
static const msg_hdr_t DRIVER_TX_MOTORIO_RUNMODE_HDR =        {   11, 1 };
static const msg_hdr_t SENSOR_TX_SENSOR_SONARS_HDR =          {   12, 6 };
static const msg_hdr_t DRIVER_TX_MOTORIO_DIRECTION_HDR =      {   13, 2 };
static const msg_hdr_t DRIVER_TX_DRIVER_CHECKPOINT_REQ_HDR =  {   14, 1 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_HDR = {   15, 1 };
static const msg_hdr_t BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_HDR = {   16, 1 };
static const msg_hdr_t DRIVER_TX_DRIVER_LOC_UPDATE_HDR =      {   17, 8 };
static const msg_hdr_t GEO_TX_GEO_SPEED_ANGLE_HDR =           {   18, 5 };
static const msg_hdr_t GEO_TX_GEO_LOC_DATA_HDR =              {   19, 8 };
static const msg_hdr_t SENSOR_TX_SENSOR_LIGHT_BAT_HDR =       {   20, 2 };


/// Message: DRIVER_KILL_SWITCH from 'DRIVER', DLC: 0 byte(s), MID: 0
typedef struct {
    uint8_t DRIVER_KILL_SWITCH_cmd;      ///< B-1:0   Destination: SENSOR,MOTORIO,BLUETOOTH,GEO

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_KILL_SWITCH_t;


/// Message: DRIVER_RESET from 'DRIVER', DLC: 0 byte(s), MID: 1
typedef struct {
    uint8_t DRIVER_RESET_cmd;            ///< B-1:0   Destination: SENSOR,MOTORIO,BLUETOOTH,GEO

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_RESET_t;


/// Message: DRIVER_SYNC_ACK from 'DRIVER', DLC: 0 byte(s), MID: 2
typedef struct {
    uint8_t DRIVER_SYNC_ACK_cmd;         ///< B-1:0   Destination: SENSOR,MOTORIO,BLUETOOTH,GEO

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_SYNC_ACK_t;


/// Message: MOTORIO_SYNC from 'MOTORIO', DLC: 0 byte(s), MID: 3
typedef struct {
    uint8_t MOTORIO_SYNC_cmd;            ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} MOTORIO_TX_MOTORIO_SYNC_t;


/// Message: SENSOR_SYNC from 'SENSOR', DLC: 0 byte(s), MID: 4
typedef struct {
    uint8_t SENSOR_SYNC_cmd;             ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_SYNC_t;


/// Message: BLUETOOTH_SYNC from 'BLUETOOTH', DLC: 0 byte(s), MID: 5
typedef struct {
    uint8_t BLUETOOTH_SYNC_cmd;          ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} BLUETOOTH_TX_BLUETOOTH_SYNC_t;


/// Message: GEO_SYNC from 'GEO', DLC: 0 byte(s), MID: 6
typedef struct {
    uint8_t GEO_SYNC_cmd;                ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} GEO_TX_GEO_SYNC_t;


/// Message: MOTORIO_HEARTBEAT from 'MOTORIO', DLC: 0 byte(s), MID: 7
typedef struct {
    uint8_t MOTORIO_HEARTBEAT_cmd;       ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} MOTORIO_TX_MOTORIO_HEARTBEAT_t;


/// Message: SENSOR_HEARTBEAT from 'SENSOR', DLC: 0 byte(s), MID: 8
typedef struct {
    uint8_t SENSOR_HEARTBEAT_cmd;        ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_HEARTBEAT_t;


/// Message: BLUETOOTH_HEARTBEAT from 'BLUETOOTH', DLC: 0 byte(s), MID: 9
typedef struct {
    uint8_t BLUETOOTH_HEARTBEAT_cmd;     ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_t;


/// Message: GEO_HEARTBEAT from 'GEO', DLC: 0 byte(s), MID: 10
typedef struct {
    uint8_t GEO_HEARTBEAT_cmd;           ///< B-1:0   Destination: DRIVER

    mia_info_t mia_info;
} GEO_TX_GEO_HEARTBEAT_t;


/// Message: MOTORIO_RUNMODE from 'DRIVER', DLC: 1 byte(s), MID: 11
typedef struct {
    uint8_t MOTORIO_RUNMODE_cmd;         ///< B7:0  Min: 0 Max: 3   Destination: MOTORIO

    mia_info_t mia_info;
} DRIVER_TX_MOTORIO_RUNMODE_t;


/// Message: SENSOR_SONARS from 'SENSOR', DLC: 6 byte(s), MID: 12
typedef struct {
    uint8_t SENSOR_SONARS_front_left;    ///< B7:0  Min: 0 Max: 4   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_SONARS_front_right;   ///< B15:8  Min: 0 Max: 4   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_SONARS_front_center;  ///< B23:16  Min: 0 Max: 4   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_SONARS_left;          ///< B31:24  Min: 0 Max: 4   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_SONARS_right;         ///< B39:32  Min: 0 Max: 4   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_SONARS_back;          ///< B47:40  Min: 0 Max: 4   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_SONARS_t;


/// Message: MOTORIO_DIRECTION from 'DRIVER', DLC: 2 byte(s), MID: 13
typedef struct {
    uint8_t MOTORIO_DIRECTION_speed_cmd; ///< B7:0  Min: 0 Max: 4   Destination: MOTORIO
    uint8_t MOTORIO_DIRECTION_turn_cmd;  ///< B15:8  Min: 0 Max: 5   Destination: MOTORIO

    mia_info_t mia_info;
} DRIVER_TX_MOTORIO_DIRECTION_t;


/// Message: DRIVER_CHECKPOINT_REQ from 'DRIVER', DLC: 1 byte(s), MID: 14
typedef struct {
    uint8_t DRIVER_CHECKPOINT_REQ_cmd;   ///< B7:0  Min: 0 Max: 3   Destination: BLUETOOTH,MOTORIO

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_CHECKPOINT_REQ_t;


/// Message: BLUETOOTH_CHECKPOINT_SEND from 'BLUETOOTH', DLC: 1 byte(s), MID: 15
typedef struct {
    uint8_t BLUETOOTH_CHECKPOINT_SEND_cmd; ///< B7:0  Min: 0 Max: 3   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_t;


/// Message: BLUETOOTH_CHECKPOINT_DATA from 'BLUETOOTH', DLC: 1 byte(s), MID: 16
typedef struct {
    uint8_t BLUETOOTH_CHECKPOINT_DATA_cmd; ///< B7:0  Min: 0 Max: 3   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_t;


/// Message: DRIVER_LOC_UPDATE from 'DRIVER', DLC: 8 byte(s), MID: 17
typedef struct {
    float DRIVER_LOC_UPDATE_LAT_cmd;     ///< B31:0   Destination: GEO,MOTORIO
    float DRIVER_LOC_UPDATE_LONG_cmd;    ///< B63:32   Destination: GEO,MOTORIO

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_LOC_UPDATE_t;


/// Message: GEO_SPEED_ANGLE from 'GEO', DLC: 5 byte(s), MID: 18
typedef struct {
    uint8_t GEO_SPEED_cmd;               ///< B7:0   Destination: DRIVER,MOTORIO
    uint16_t GEO_ANGLE_heading_cmd;      ///< B23:8   Destination: DRIVER,MOTORIO
    uint16_t GEO_ANGLE_bearing_cmd;      ///< B39:24   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} GEO_TX_GEO_SPEED_ANGLE_t;


/// Message: GEO_LOC_DATA from 'GEO', DLC: 8 byte(s), MID: 19
typedef struct {
    float GEO_LOC_LAT_cmd;               ///< B31:0   Destination: DRIVER,MOTORIO
    float GEO_LOC_LONG_cmd;              ///< B63:32   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} GEO_TX_GEO_LOC_DATA_t;


/// Message: SENSOR_LIGHT_BAT from 'SENSOR', DLC: 2 byte(s), MID: 20
typedef struct {
    uint8_t SENSOR_LIGHT_cmd;            ///< B7:0  Min: 0 Max: 1   Destination: DRIVER,MOTORIO
    uint8_t SENSOR_BAT_cmd;              ///< B15:8  Min: 0 Max: 1   Destination: DRIVER,MOTORIO

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_LIGHT_BAT_t;


/// These 'externs' need to be defined in a source file of your project
extern const uint32_t MOTORIO_SYNC__MIA_MS;
extern const MOTORIO_TX_MOTORIO_SYNC_t MOTORIO_SYNC__MIA_MSG;
extern const uint32_t SENSOR_SYNC__MIA_MS;
extern const SENSOR_TX_SENSOR_SYNC_t SENSOR_SYNC__MIA_MSG;
extern const uint32_t BLUETOOTH_SYNC__MIA_MS;
extern const BLUETOOTH_TX_BLUETOOTH_SYNC_t BLUETOOTH_SYNC__MIA_MSG;
extern const uint32_t GEO_SYNC__MIA_MS;
extern const GEO_TX_GEO_SYNC_t GEO_SYNC__MIA_MSG;
extern const uint32_t MOTORIO_HEARTBEAT__MIA_MS;
extern const MOTORIO_TX_MOTORIO_HEARTBEAT_t MOTORIO_HEARTBEAT__MIA_MSG;
extern const uint32_t SENSOR_HEARTBEAT__MIA_MS;
extern const SENSOR_TX_SENSOR_HEARTBEAT_t SENSOR_HEARTBEAT__MIA_MSG;
extern const uint32_t BLUETOOTH_HEARTBEAT__MIA_MS;
extern const BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_t BLUETOOTH_HEARTBEAT__MIA_MSG;
extern const uint32_t GEO_HEARTBEAT__MIA_MS;
extern const GEO_TX_GEO_HEARTBEAT_t GEO_HEARTBEAT__MIA_MSG;
extern const uint32_t SENSOR_SONARS__MIA_MS;
extern const SENSOR_TX_SENSOR_SONARS_t SENSOR_SONARS__MIA_MSG;
extern const uint32_t BLUETOOTH_CHECKPOINT_SEND__MIA_MS;
extern const BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_t BLUETOOTH_CHECKPOINT_SEND__MIA_MSG;
extern const uint32_t BLUETOOTH_CHECKPOINT_DATA__MIA_MS;
extern const BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_t BLUETOOTH_CHECKPOINT_DATA__MIA_MSG;
extern const uint32_t GEO_SPEED_ANGLE__MIA_MS;
extern const GEO_TX_GEO_SPEED_ANGLE_t GEO_SPEED_ANGLE__MIA_MSG;
extern const uint32_t GEO_LOC_DATA__MIA_MS;
extern const GEO_TX_GEO_LOC_DATA_t GEO_LOC_DATA__MIA_MSG;
extern const uint32_t SENSOR_LIGHT_BAT__MIA_MS;
extern const SENSOR_TX_SENSOR_LIGHT_BAT_t SENSOR_LIGHT_BAT__MIA_MSG;

/// Encode DRIVER's 'DRIVER_KILL_SWITCH' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_KILL_SWITCH_encode(uint64_t *to, DRIVER_TX_DRIVER_KILL_SWITCH_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DRIVER_KILL_SWITCH_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return DRIVER_TX_DRIVER_KILL_SWITCH_HDR;
}


/// Encode DRIVER's 'DRIVER_RESET' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_RESET_encode(uint64_t *to, DRIVER_TX_DRIVER_RESET_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DRIVER_RESET_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return DRIVER_TX_DRIVER_RESET_HDR;
}


/// Encode DRIVER's 'DRIVER_SYNC_ACK' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_SYNC_ACK_encode(uint64_t *to, DRIVER_TX_DRIVER_SYNC_ACK_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DRIVER_SYNC_ACK_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return DRIVER_TX_DRIVER_SYNC_ACK_HDR;
}


/// Not generating code for MOTORIO_TX_MOTORIO_SYNC_encode() since the sender is MOTORIO and we are DRIVER

/// Not generating code for SENSOR_TX_SENSOR_SYNC_encode() since the sender is SENSOR and we are DRIVER

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_SYNC_encode() since the sender is BLUETOOTH and we are DRIVER

/// Not generating code for GEO_TX_GEO_SYNC_encode() since the sender is GEO and we are DRIVER

/// Not generating code for MOTORIO_TX_MOTORIO_HEARTBEAT_encode() since the sender is MOTORIO and we are DRIVER

/// Not generating code for SENSOR_TX_SENSOR_HEARTBEAT_encode() since the sender is SENSOR and we are DRIVER

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_encode() since the sender is BLUETOOTH and we are DRIVER

/// Not generating code for GEO_TX_GEO_HEARTBEAT_encode() since the sender is GEO and we are DRIVER

/// Encode DRIVER's 'MOTORIO_RUNMODE' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_MOTORIO_RUNMODE_encode(uint64_t *to, DRIVER_TX_MOTORIO_RUNMODE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    if(from->MOTORIO_RUNMODE_cmd < 0) { from->MOTORIO_RUNMODE_cmd = 0; }
    if(from->MOTORIO_RUNMODE_cmd > 3) { from->MOTORIO_RUNMODE_cmd = 3; }
    raw_signal = ((uint64_t)(((from->MOTORIO_RUNMODE_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return DRIVER_TX_MOTORIO_RUNMODE_HDR;
}


/// Not generating code for SENSOR_TX_SENSOR_SONARS_encode() since the sender is SENSOR and we are DRIVER

/// Encode DRIVER's 'MOTORIO_DIRECTION' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_MOTORIO_DIRECTION_encode(uint64_t *to, DRIVER_TX_MOTORIO_DIRECTION_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    if(from->MOTORIO_DIRECTION_speed_cmd < 0) { from->MOTORIO_DIRECTION_speed_cmd = 0; }
    if(from->MOTORIO_DIRECTION_speed_cmd > 4) { from->MOTORIO_DIRECTION_speed_cmd = 4; }
    raw_signal = ((uint64_t)(((from->MOTORIO_DIRECTION_speed_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    if(from->MOTORIO_DIRECTION_turn_cmd < 0) { from->MOTORIO_DIRECTION_turn_cmd = 0; }
    if(from->MOTORIO_DIRECTION_turn_cmd > 5) { from->MOTORIO_DIRECTION_turn_cmd = 5; }
    raw_signal = ((uint64_t)(((from->MOTORIO_DIRECTION_turn_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    return DRIVER_TX_MOTORIO_DIRECTION_HDR;
}


/// Encode DRIVER's 'DRIVER_CHECKPOINT_REQ' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_CHECKPOINT_REQ_encode(uint64_t *to, DRIVER_TX_DRIVER_CHECKPOINT_REQ_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    if(from->DRIVER_CHECKPOINT_REQ_cmd < 0) { from->DRIVER_CHECKPOINT_REQ_cmd = 0; }
    if(from->DRIVER_CHECKPOINT_REQ_cmd > 3) { from->DRIVER_CHECKPOINT_REQ_cmd = 3; }
    raw_signal = ((uint64_t)(((from->DRIVER_CHECKPOINT_REQ_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return DRIVER_TX_DRIVER_CHECKPOINT_REQ_HDR;
}


/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_encode() since the sender is BLUETOOTH and we are DRIVER

/// Not generating code for BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_encode() since the sender is BLUETOOTH and we are DRIVER

/// Encode DRIVER's 'DRIVER_LOC_UPDATE' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_LOC_UPDATE_encode(uint64_t *to, DRIVER_TX_DRIVER_LOC_UPDATE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DRIVER_LOC_UPDATE_LAT_cmd - (0)) / 0.0001) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->DRIVER_LOC_UPDATE_LONG_cmd - (0)) / 0.0001) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56

    return DRIVER_TX_DRIVER_LOC_UPDATE_HDR;
}


/// Not generating code for GEO_TX_GEO_SPEED_ANGLE_encode() since the sender is GEO and we are DRIVER

/// Not generating code for GEO_TX_GEO_LOC_DATA_encode() since the sender is GEO and we are DRIVER

/// Not generating code for SENSOR_TX_SENSOR_LIGHT_BAT_encode() since the sender is SENSOR and we are DRIVER

/// Not generating code for DRIVER_TX_DRIVER_KILL_SWITCH_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_RESET_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_SYNC_ACK_decode() since we are not the recipient of any of its signals

/// Decode MOTORIO's 'MOTORIO_SYNC' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTORIO_TX_MOTORIO_SYNC_decode(MOTORIO_TX_MOTORIO_SYNC_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTORIO_TX_MOTORIO_SYNC_HDR.dlc || hdr->mid != MOTORIO_TX_MOTORIO_SYNC_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->MOTORIO_SYNC_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'SENSOR_SYNC' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SENSOR_SYNC_decode(SENSOR_TX_SENSOR_SYNC_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SENSOR_SYNC_HDR.dlc || hdr->mid != SENSOR_TX_SENSOR_SYNC_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->SENSOR_SYNC_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode BLUETOOTH's 'BLUETOOTH_SYNC' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool BLUETOOTH_TX_BLUETOOTH_SYNC_decode(BLUETOOTH_TX_BLUETOOTH_SYNC_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != BLUETOOTH_TX_BLUETOOTH_SYNC_HDR.dlc || hdr->mid != BLUETOOTH_TX_BLUETOOTH_SYNC_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BLUETOOTH_SYNC_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'GEO_SYNC' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_GEO_SYNC_decode(GEO_TX_GEO_SYNC_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_GEO_SYNC_HDR.dlc || hdr->mid != GEO_TX_GEO_SYNC_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->GEO_SYNC_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MOTORIO's 'MOTORIO_HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTORIO_TX_MOTORIO_HEARTBEAT_decode(MOTORIO_TX_MOTORIO_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTORIO_TX_MOTORIO_HEARTBEAT_HDR.dlc || hdr->mid != MOTORIO_TX_MOTORIO_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->MOTORIO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'SENSOR_HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SENSOR_HEARTBEAT_decode(SENSOR_TX_SENSOR_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SENSOR_HEARTBEAT_HDR.dlc || hdr->mid != SENSOR_TX_SENSOR_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->SENSOR_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode BLUETOOTH's 'BLUETOOTH_HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_decode(BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_HDR.dlc || hdr->mid != BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BLUETOOTH_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'GEO_HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_GEO_HEARTBEAT_decode(GEO_TX_GEO_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_GEO_HEARTBEAT_HDR.dlc || hdr->mid != GEO_TX_GEO_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->GEO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Not generating code for DRIVER_TX_MOTORIO_RUNMODE_decode() since we are not the recipient of any of its signals

/// Decode SENSOR's 'SENSOR_SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SENSOR_SONARS_decode(SENSOR_TX_SENSOR_SONARS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SENSOR_SONARS_HDR.dlc || hdr->mid != SENSOR_TX_SENSOR_SONARS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_front_left = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_front_right = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_front_center = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_left = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[4] >> 0) & 0xff); ///< 8 bit(s) from B32
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_right = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[5] >> 0) & 0xff); ///< 8 bit(s) from B40
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_back = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Not generating code for DRIVER_TX_MOTORIO_DIRECTION_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_DRIVER_CHECKPOINT_REQ_decode() since we are not the recipient of any of its signals

/// Decode BLUETOOTH's 'BLUETOOTH_CHECKPOINT_SEND' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_decode(BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_HDR.dlc || hdr->mid != BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->BLUETOOTH_CHECKPOINT_SEND_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode BLUETOOTH's 'BLUETOOTH_CHECKPOINT_DATA' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_decode(BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_HDR.dlc || hdr->mid != BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->BLUETOOTH_CHECKPOINT_DATA_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Not generating code for DRIVER_TX_DRIVER_LOC_UPDATE_decode() since we are not the recipient of any of its signals

/// Decode GEO's 'GEO_SPEED_ANGLE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_GEO_SPEED_ANGLE_decode(GEO_TX_GEO_SPEED_ANGLE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_GEO_SPEED_ANGLE_HDR.dlc || hdr->mid != GEO_TX_GEO_SPEED_ANGLE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->GEO_SPEED_cmd = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 8);
    to->GEO_ANGLE_heading_cmd = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[4] >> 0) & 0xff); ///< 8 bit(s) from B32
    raw_signal    |= (bits_from_byte << 8);
    to->GEO_ANGLE_bearing_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'GEO_LOC_DATA' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_GEO_LOC_DATA_decode(GEO_TX_GEO_LOC_DATA_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_GEO_LOC_DATA_HDR.dlc || hdr->mid != GEO_TX_GEO_LOC_DATA_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 24);
    to->GEO_LOC_LAT_cmd = (raw_signal * 0.0001) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[4] >> 0) & 0xff); ///< 8 bit(s) from B32
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[5] >> 0) & 0xff); ///< 8 bit(s) from B40
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[6] >> 0) & 0xff); ///< 8 bit(s) from B48
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[7] >> 0) & 0xff); ///< 8 bit(s) from B56
    raw_signal    |= (bits_from_byte << 24);
    to->GEO_LOC_LONG_cmd = (raw_signal * 0.0001) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'SENSOR_LIGHT_BAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SENSOR_LIGHT_BAT_decode(SENSOR_TX_SENSOR_LIGHT_BAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SENSOR_LIGHT_BAT_HDR.dlc || hdr->mid != SENSOR_TX_SENSOR_LIGHT_BAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_LIGHT_cmd = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_BAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Handle the MIA for MOTORIO's 'MOTORIO_SYNC' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTORIO_TX_MOTORIO_SYNC_handle_mia(MOTORIO_TX_MOTORIO_SYNC_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTORIO_SYNC__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTORIO_SYNC__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTORIO_SYNC__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SENSOR_SYNC' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_SYNC_handle_mia(SENSOR_TX_SENSOR_SYNC_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SYNC__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_SYNC__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_SYNC__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BLUETOOTH's 'BLUETOOTH_SYNC' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool BLUETOOTH_TX_BLUETOOTH_SYNC_handle_mia(BLUETOOTH_TX_BLUETOOTH_SYNC_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BLUETOOTH_SYNC__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BLUETOOTH_SYNC__MIA_MSG;
        msg->mia_info.mia_counter_ms = BLUETOOTH_SYNC__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'GEO_SYNC' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_GEO_SYNC_handle_mia(GEO_TX_GEO_SYNC_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_SYNC__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_SYNC__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_SYNC__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTORIO's 'MOTORIO_HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTORIO_TX_MOTORIO_HEARTBEAT_handle_mia(MOTORIO_TX_MOTORIO_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTORIO_HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTORIO_HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTORIO_HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SENSOR_HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_HEARTBEAT_handle_mia(SENSOR_TX_SENSOR_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BLUETOOTH's 'BLUETOOTH_HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_handle_mia(BLUETOOTH_TX_BLUETOOTH_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BLUETOOTH_HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BLUETOOTH_HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = BLUETOOTH_HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'GEO_HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_GEO_HEARTBEAT_handle_mia(GEO_TX_GEO_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SENSOR_SONARS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_SONARS_handle_mia(SENSOR_TX_SENSOR_SONARS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SONARS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_SONARS__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_SONARS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BLUETOOTH's 'BLUETOOTH_CHECKPOINT_SEND' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_handle_mia(BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_SEND_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BLUETOOTH_CHECKPOINT_SEND__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BLUETOOTH_CHECKPOINT_SEND__MIA_MSG;
        msg->mia_info.mia_counter_ms = BLUETOOTH_CHECKPOINT_SEND__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BLUETOOTH's 'BLUETOOTH_CHECKPOINT_DATA' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_handle_mia(BLUETOOTH_TX_BLUETOOTH_CHECKPOINT_DATA_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BLUETOOTH_CHECKPOINT_DATA__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BLUETOOTH_CHECKPOINT_DATA__MIA_MSG;
        msg->mia_info.mia_counter_ms = BLUETOOTH_CHECKPOINT_DATA__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'GEO_SPEED_ANGLE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_GEO_SPEED_ANGLE_handle_mia(GEO_TX_GEO_SPEED_ANGLE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_SPEED_ANGLE__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_SPEED_ANGLE__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_SPEED_ANGLE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'GEO_LOC_DATA' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_GEO_LOC_DATA_handle_mia(GEO_TX_GEO_LOC_DATA_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GEO_LOC_DATA__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GEO_LOC_DATA__MIA_MSG;
        msg->mia_info.mia_counter_ms = GEO_LOC_DATA__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SENSOR_LIGHT_BAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_LIGHT_BAT_handle_mia(SENSOR_TX_SENSOR_LIGHT_BAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_LIGHT_BAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_LIGHT_BAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_LIGHT_BAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

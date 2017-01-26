//
// Copyright (c) 2017 The nano Framework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#ifndef _WIREPROTOCOL_H_
#define _WIREPROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#define MARKER_DEBUGGER_V1 "MSdbgV1" // Used to identify the debugger at boot time.
#define MARKER_PACKET_V1   "MSpktV1" // Used to identify the start of a packet.

// enum with Wire Protocol flags
// backwards compatible with .NETMF
typedef enum WP_Flags
{
    WP_Flags_c_NonCritical = 0x0001, // This doesn't need an acknowledge.
    WP_Flags_c_Reply       = 0x0002, // This is the result of a command.
    WP_Flags_c_BadHeader   = 0x0004,
    WP_Flags_c_BadPayload  = 0x0008,
    WP_Flags_c_Spare0010   = 0x0010,
    WP_Flags_c_Spare0020   = 0x0020,
    WP_Flags_c_Spare0040   = 0x0040,
    WP_Flags_c_Spare0080   = 0x0080,
    WP_Flags_c_Spare0100   = 0x0100,
    WP_Flags_c_Spare0200   = 0x0200,
    WP_Flags_c_Spare0400   = 0x0400,
    WP_Flags_c_Spare0800   = 0x0800,
    WP_Flags_c_Spare1000   = 0x1000,
    WP_Flags_c_NoCaching   = 0x2000,
    WP_Flags_c_NACK        = 0x4000,
    WP_Flags_c_ACK         = 0x8000
}WP_Flags;

// enum with machine states for Wire Procotol receiver
typedef enum ReceiveState
{
    ReceiveState_Idle             = (1 << 0),
    ReceiveState_Initialize       = (2 << 0),
    ReceiveState_WaitingForHeader = (3 << 0),
    ReceiveState_ReadingHeader    = (4 << 0),
    ReceiveState_CompleteHeader   = (5 << 0),
    ReceiveState_ReadingPayload   = (6 << 0),
    ReceiveState_CompletePayload  = (7 << 0),
}ReceiveState;

// enum with CLR and debugger commands
// backwards compatible with .NETMF
typedef enum CLR_DBG_Commands
{
    CLR_DBG_Commands_c_Monitor_Ping               = 0x00000000, // The payload is empty, this command is used to let the other side know we are here...
    CLR_DBG_Commands_c_Monitor_Message            = 0x00000001, // The payload is composed of the string characters, no zero at the end.
    CLR_DBG_Commands_c_Monitor_ReadMemory         = 0x00000002,
    CLR_DBG_Commands_c_Monitor_WriteMemory        = 0x00000003,
    CLR_DBG_Commands_c_Monitor_CheckMemory        = 0x00000004,
    CLR_DBG_Commands_c_Monitor_EraseMemory        = 0x00000005,
    CLR_DBG_Commands_c_Monitor_Execute            = 0x00000006,
    CLR_DBG_Commands_c_Monitor_Reboot             = 0x00000007,
    CLR_DBG_Commands_c_Monitor_MemoryMap          = 0x00000008,
    CLR_DBG_Commands_c_Monitor_ProgramExit        = 0x00000009, // The payload is empty, this command is used to tell the PC of a program termination
    CLR_DBG_Commands_c_Monitor_CheckSignature     = 0x0000000A,
    CLR_DBG_Commands_c_Monitor_DeploymentMap      = 0x0000000B,
    CLR_DBG_Commands_c_Monitor_FlashSectorMap     = 0x0000000C,
    CLR_DBG_Commands_c_Monitor_SignatureKeyUpdate = 0x0000000D,
    CLR_DBG_Commands_c_Monitor_OemInfo            = 0x0000000E,
}CLR_DBG_Commands;

// structure for Wire Protocol packet
// backwards compatible with .NETMF
typedef struct WP_Packet
{
    uint8_t  m_signature[8];
    uint32_t m_crcHeader;
    uint32_t m_crcData;

    uint32_t m_cmd;
    uint16_t m_seq;
    uint16_t m_seqReply;
    uint32_t m_flags;
    uint32_t m_size;
}WP_Packet;

// structure for Wire Protocol message
// backwards compatible with .NETMF equivalent in names and types to help code reuse
typedef struct WP_Message
{
    WP_Packet      m_header;
    uint8_t*       m_payload;

    uint8_t*       m_pos;
    uint16_t       m_size;
    int            m_rxState;
}WP_Message;

// enum with flags for Monitor ping source and debugger flags
// backwards compatible with .NETMF in debugger flags only
// adds NEW flags for nanoBooter and nanoCLR
typedef enum Monitor_Ping_Source_Flags
{
    Monitor_Ping_c_Ping_Source_NanoCLR    = 0x00010000,
    Monitor_Ping_c_Ping_Source_NanoBooter = 0x00010001,

    Monitor_Ping_c_Ping_DbgFlag_Stop      = 0x00000001,
    Monitor_Ping_c_Ping_DbgFlag_AppExit   = 0x00000004,
}Monitor_Ping_Source_Flags;


// structure to hold 'standard' version information
// equivalent with .NETMF MFVersion
typedef struct VersionInfo
{
    unsigned short usMajor;
    unsigned short usMinor;
    unsigned short usBuild;
    unsigned short usRevision;

}VersionInfo;

// structure to hold nano Framework release information
// equivalent with .NETMF MfReleaseInfo
typedef struct ReleaseInfo
{

    VersionInfo version;

    uint8_t infoString[64-sizeof(VersionInfo)];

}ReleaseInfo;

// structure for Wire Protocol command handler lookup
typedef struct CommandHandlerLookup
{
    // command code goes here
    uint32_t command;
    
    // pointer to handler function 
    void* handler;

}CommandHandlerLookup;

// This structure is never used, its purpose is to generate a compiler error in case the size of any structure changes.
struct WP_CompileCheck
{
    char buf1[ sizeof(WP_Packet) == 8 * 4 ? 1 : -1 ];
};

#endif // _WIREPROTOCOL_H_
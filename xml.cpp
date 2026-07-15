#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <amx/amx.h>

#ifdef _WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

typedef void (*logprintf_t)(const char* format, ...);
typedef void (*SendClientPacket_t)(int playerid, const unsigned char* data, int len);

logprintf_t logprintf;
SendClientPacket_t SendClientPacket;

cell SendXML(AMX *amx, cell *params) {
    int playerid = params[1];
    char *xml;
    amx_GetAddr(amx, params[2], (cell**)&xml);
    
    int len = strlen(xml);
    unsigned char packet[512];
    int pos = 0;
    packet[pos++] = 0x80; // ID_RPC
    packet[pos++] = 220;  // RPC ID
    packet[pos++] = 0x01; // sub-command
    packet[pos++] = (len >> 8) & 0xFF;
    packet[pos++] = len & 0xFF;
    memcpy(&packet[pos], xml, len);
    pos += len;
    
    SendClientPacket(playerid, packet, pos);
    return 1;
}

AMX_NATIVE_INFO natives[] = {
    {"SendXML", SendXML},
    {NULL, NULL}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {
    return amx_Register(amx, natives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
    return AMX_ERR_NONE;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
    logprintf = (logprintf_t)ppData[0];
    SendClientPacket = (SendClientPacket_t)ppData[2];
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {}
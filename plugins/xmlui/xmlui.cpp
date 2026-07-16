#include <cstring>
#include "amx/amx.h"
#include "plugincommon.h"
#include "RakNet/RakServerInterface.h"
#include "RakNet/BitStream.h"

extern void *pAMXFunctions;
typedef void (*logprintf_t)(const char* format, ...);
logprintf_t logprintf;
RakServerInterface *pRakServer = NULL;

// === MOHIM: had function khassha tkon mawjouda, wla l plugin ma تلقاش ===
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
    pRakServer = (RakServerInterface*)ppData[PLUGIN_DATA_RAKSERVER];
    logprintf(" [xmlui] Plugin loaded.");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {}

cell AMX_NATIVE_CALL SendXML(AMX *amx, cell *params) {
    int playerid = static_cast<int>(params[1]);

    cell *addr;
    amx_GetAddr(amx, params[2], &addr);
    int len;
    amx_StrLen(addr, &len);
    char *xml = new char[len + 1];
    amx_GetString(xml, addr, 0, len + 1);

    RakNet::BitStream bs;
    bs.Write((unsigned char)0x01);        // sub-command
    bs.Write((unsigned short)len);
    bs.Write(xml, len);

    unsigned int rpcId = 220;
    pRakServer->RPC(&rpcId, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
                     pRakServer->GetPlayerIDFromIndex(playerid), false, false);

    delete[] xml;
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

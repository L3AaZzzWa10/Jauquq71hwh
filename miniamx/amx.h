#ifndef AMX_H
#define AMX_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AMX_ERR_NONE 0
#define AMX_ERR_NOTFOUND 1

typedef int32_t cell;
typedef struct AMX AMX;
typedef struct AMX_NATIVE_INFO AMX_NATIVE_INFO;
typedef cell (*AMX_NATIVE)(AMX *amx, cell *params);

struct AMX {
    cell* base;
    cell* data;
    cell* heap;
    cell* stack;
};

struct AMX_NATIVE_INFO {
    const char *name;
    AMX_NATIVE func;
};

cell amx_Register(AMX *amx, const AMX_NATIVE_INFO *nativelist, int num);
cell amx_GetAddr(AMX *amx, cell index, cell **addr);
cell amx_FindPublic(AMX *amx, const char *name, int *index);
cell amx_Exec(AMX *amx, cell *retval, int index);

#ifdef __cplusplus
}
#endif

#endif

/*************************************************************************
*
* Driver interface
*
*************************************************************************/

#include "stdafx.h"

#define IO_SET_HOOK						CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0711, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_UNSET_HOOK					CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0713, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST
{
    uint64_t Reserved_1;
    uint64_t Reserved_2;
    uint64_t Reserved_3;
    uint64_t Reserved_4;
    uint64_t Status;
    uint64_t Reserved_5;
    uint64_t Reserved_6;

} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
    uint64_t Reserved_1;
    uint64_t Reserved_2;
    uint64_t Reserved_3;
    uint64_t Reserved_4;
    uint64_t Status;
    uint64_t Reserved_5;
    uint64_t Reserved_6;

} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

class KI
{
public:
    HANDLE hDriver = INVALID_HANDLE_VALUE;

    KI(LPCWSTR RegistryPath);

    BOOL SetHook();

    BOOL UnsetHook();

};
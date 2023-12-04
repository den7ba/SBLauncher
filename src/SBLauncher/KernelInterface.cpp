/*************************************************************************
*
* Driver interface
*
*************************************************************************/

#include "stdafx.h"

KI::KI(LPCWSTR RegistryPath)
{
    if (wcscmp(RegistryPath, L"None") == 0)
        return;

    this->hDriver = CreateFileW(RegistryPath, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

    if (this->hDriver == INVALID_HANDLE_VALUE)
        SoftExit("Driver not found");
}

BOOL KI::SetHook()
{
    if (this->hDriver == INVALID_HANDLE_VALUE)
        return false;

    KERNEL_WRITE_REQUEST WriteRequest;

    WriteRequest.Status = TRUE;

    if (DeviceIoControl(this->hDriver, IO_SET_HOOK, &WriteRequest,
        sizeof(WriteRequest), &WriteRequest, sizeof(WriteRequest), NULL, NULL)) {

        return WriteRequest.Status == 0;
    }
    else
        return FALSE;
}

BOOL KI::UnsetHook()
{
    if (this->hDriver == INVALID_HANDLE_VALUE)
        return false;

    KERNEL_READ_REQUEST ReadRequest;

    ReadRequest.Status = TRUE;

    if (DeviceIoControl(this->hDriver, IO_UNSET_HOOK, &ReadRequest,
        sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), NULL, NULL)) {
        return ReadRequest.Status == 0;
    }
    else
        return FALSE;
}
/*************************************************************************
*
* Sandboxie interface
*
*************************************************************************/

#include "stdafx.h"

VOID AddForceProcess(wchar_t* boxName, wchar_t* procName);
VOID DeleteForceProcess(wchar_t* boxName, wchar_t* procName);
VOID ReloadSandboxConfiguration();
BOOL CreateSandbox(wchar_t* boxName);
VOID DeleteSandbox(wchar_t* boxName);
BOOL IsBoxHaveProcesses(wchar_t* boxName);
VOID AddCredentialsToBox(wchar_t* boxName, wchar_t* login, wchar_t* pass, UINT LocId);
VOID SbieDllInit();
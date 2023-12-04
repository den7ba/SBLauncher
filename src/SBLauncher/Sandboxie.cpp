/*************************************************************************
*
* Sandboxie interface
*
*************************************************************************/

#include "stdafx.h"
#include "SbiedllApi.h"

P_SbieDll_UpdateConf __sys_SbieDll_UpdateConf = NULL;
P_SbieApi_ReloadConf __sys_SbieApi_ReloadConf = NULL;
P_SbieApi_EnumBoxes __sys_SbieApi_EnumBoxes = NULL;
P_SbieApi_EnumProcessEx __sys_SbieApi_EnumProcessEx = NULL;

VOID SbieDllInit() {
    HMODULE hModule = LoadLibrary(Conf()->SbieDllPath);
    if (hModule) {
        __sys_SbieApi_ReloadConf = (P_SbieApi_ReloadConf)GetProcAddress(hModule, "SbieApi_ReloadConf");
        __sys_SbieDll_UpdateConf = (P_SbieDll_UpdateConf)GetProcAddress(hModule, "SbieDll_UpdateConf");
        __sys_SbieApi_EnumBoxes = (P_SbieApi_EnumBoxes)GetProcAddress(hModule, "SbieApi_EnumBoxes");
        __sys_SbieApi_EnumProcessEx = (P_SbieApi_EnumProcessEx)GetProcAddress(hModule, "SbieApi_EnumProcessEx");
    }
    else {
        SoftExit("SbieDll.dll not found\n");
    }

    if (!__sys_SbieApi_ReloadConf || !__sys_SbieDll_UpdateConf || !__sys_SbieApi_EnumBoxes || !__sys_SbieApi_EnumProcessEx)
        SoftExit("SbieDll procs not found");
}

VOID AddForceProcess(wchar_t* boxName, wchar_t* procName) {
    __sys_SbieDll_UpdateConf('d', NULL, boxName, L"ForceProcessDisabled", NULL);
    __sys_SbieDll_UpdateConf('a', NULL, boxName, L"ForceProcess", procName);
}

VOID DeleteForceProcess(wchar_t* boxName, wchar_t* procName) {
    __sys_SbieDll_UpdateConf('d', NULL, boxName, L"ForceProcess", NULL);
    __sys_SbieDll_UpdateConf('a', NULL, boxName, L"ForceProcessDisabled", procName);
}

VOID ReloadSandboxConfiguration() {
    __sys_SbieApi_ReloadConf(NULL);
}

BOOL CreateSandbox(wchar_t* boxName) {

    WCHAR name[34];
    int index = -1;
    while (TRUE) {
        index = __sys_SbieApi_EnumBoxes(index, name);
        if (index == -1)
            break;
         
        if (wcscmp(name, boxName) == 0) {
            wprintf(L"CreateSandbox: sandbox %s is exists!\n", name);
            return FALSE;
        }
    }

    for (auto const& x : *(Conf()->SandboxConfig))
    {
        __sys_SbieDll_UpdateConf('a', NULL, boxName, x.first, x.second);
    }
    return TRUE;
}

VOID DeleteSandbox(wchar_t* boxName) {
    wchar_t buffer[300];
    wsprintfW(buffer, L"\"%s\" /box:%s delete_sandbox", Conf()->SbieStartPath, boxName);
    _wsystem(buffer);
    Sleep(1000);
    __sys_SbieDll_UpdateConf('s', NULL, boxName, L"*", NULL);
    wprintf(L"Sandbox %s was deleted\n", boxName);
    Sleep(4000);
}

BOOL IsBoxHaveProcesses(wchar_t* boxName) {
    ULONG Pids[512];
    LONG res = __sys_SbieApi_EnumProcessEx(boxName, TRUE, -1, Pids, NULL);
    return (BOOL)Pids[0];
}

VOID AddCredentialsToBox(wchar_t* boxName, wchar_t* login, wchar_t* pass, UINT LocId) {
    srand((UINT)time(0));

    if (LocId == 0x999) {
        LocId = rand() % 6 + 1;
    }

    wchar_t LocIdW[10];
    wsprintfW(LocIdW, L"%d", LocId);

    __sys_SbieDll_UpdateConf('a', NULL, boxName, L"LauncherNum", LocIdW);
    __sys_SbieDll_UpdateConf('a', NULL, boxName, L"LauncherLogin", login);
    __sys_SbieDll_UpdateConf('a', NULL, boxName, L"LauncherPassword", pass);
}
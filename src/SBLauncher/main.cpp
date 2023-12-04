/*************************************************************************
*
* SBLauncher
*
*************************************************************************/

#include "stdafx.h"

Config* __sys_Config = NULL; // global configuration
KI* __sys_Driver = NULL; //is external service. needed for specific actions after running all sandboxes

INT main() {
    wprintf(L"Main: Start\n");

    SbieDllInit();

    Test();

    Worker();

    printf("Main: End\n");
}

VOID Worker() {
    HANDLE hMutex = 0;
    DWORD dwWaitResult;

    while (TRUE) {
        if (!hMutex) {
            hMutex = OpenMutex(
                MUTEX_ALL_ACCESS,
                FALSE,
                TEXT("SBLauncher"));

            if (hMutex == NULL) {
                hMutex = CreateMutex(
                    NULL,
                    FALSE,
                    TEXT("SBLauncher"));
                if (hMutex == NULL)
                    wprintf(L"Worker: CreateMutex error: %d\n", GetLastError());
            }
        }

        if (hMutex) {
            dwWaitResult = WaitForSingleObject(
                hMutex,
                INFINITE);

            switch (dwWaitResult)
            {
            case WAIT_OBJECT_0:
                RunALL();
                ReleaseMutex(hMutex);
                break;
            case WAIT_ABANDONED:
                wprintf(L"Worker: ABANDONED\n");
                ReleaseMutex(hMutex);
                break;
            case WAIT_TIMEOUT:
                wprintf(L"Worker: WAIT_TIMEOUT\n");
                break;
            case WAIT_FAILED:
                wprintf(L"Worker: WAIT_FAILED\n");
                hMutex = 0;
                break;
            }
        }
        else {
            wprintf(L"Worker: No mutex\n");
        }

        //end work
        Sleep(30000);
    }
}

void RunALL() {
    wprintf(L"RunALL: Start\n");
    BOOL ProcessWasLaunched = FALSE;

    char*    LoginA;
    wchar_t* LoginW;

    for (auto const& x : *Conf()->Accounts)
    {
        LoginA = x.first;
        LoginW = x.second;

        std::wstring SandboxNameC = LoginW;
        std::replace(SandboxNameC.begin(), SandboxNameC.end(), '.', '_');
        wchar_t* SandboxName = (wchar_t*)SandboxNameC.c_str();

        wprintf(L"RunALL: Box %s launch...\n", SandboxName);

        wchar_t Password[50];
        wsprintfW(Password, L"%S", Conf()->AccountsData->get(LoginA).get("password").c_str());
        UINT LocationId = StrToIntA(Conf()->AccountsData->get(LoginA).get("farmlocid").c_str());

        for (UINT i = 0; i < 2; i++) {
            if (CreateSandbox(SandboxName)) {
                AddCredentialsToBox(SandboxName, LoginW, Password, LocationId);
                Launch(SandboxName);
                ProcessWasLaunched = TRUE;
                wprintf(L"RunALL: Launched\n");
                break;
            } else {
                if (i) {
                    wprintf(L"RunALL: Cant create the sandbox\n");
                    break;
                }

                if (IsBoxHaveProcesses(SandboxName)) {
                    //todo: check the process
                    break;
                } else {
                    DeleteSandbox(SandboxName);
                    wprintf(L"RunALL: Sandbox deleted\n");
                }
            }
        }
    }

    if (ProcessWasLaunched)
        PerformAdditionalActions();
}

VOID PerformAdditionalActions() {
    Sleep(20000);
    printf("RunALL: SetHook\n");
    Driver()->SetHook();
    execAdditionalCommand();
    Sleep(4000);
    printf("RunALL: UnsetHook\n");
    Driver()->UnsetHook();
    printf("RunALL: Done\n");
}

BOOL Launch(wchar_t* boxName) {
    AddForceProcess(boxName, (wchar_t*)Conf()->LaunchProcess);
    ReloadSandboxConfiguration();
    Sleep(500);
    
    execProgram();
    
    DeleteForceProcess(boxName, (wchar_t*)Conf()->LaunchProcess);
    ReloadSandboxConfiguration();

    Sleep(500);

    return IsBoxHaveProcesses(boxName);
}

VOID SoftExit(const char* reason) {
    printf("SoftExit: %s\n", reason);
    Sleep(15000);
    abort();
}

VOID execAdditionalCommand() {
    if (wcscmp(Conf()->AdditionalCommand, L"None") != 0)
        _wsystem(Conf()->AdditionalCommand);
}

VOID execProgram() {
    _wsystem(Conf()->LaunchPath);
}

VOID Test() {
    
}

Config* Conf() {
    if (!__sys_Config)
        __sys_Config = new Config;

    return __sys_Config;
}

KI* Driver() {
    if (!__sys_Driver)
        __sys_Driver = new KI(Conf()->DriverPath);

    return __sys_Driver;
}
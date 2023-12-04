/*************************************************************************
*
* Configuration
*
*************************************************************************/

#include "stdafx.h"
#include "../../mINI/src/mini/ini.h"

typedef std::map<char*, wchar_t*> AccountsMap;
typedef std::map<const wchar_t*, const wchar_t*> TextConfig;

class Config {
public:
    // the name of the process that will be hooked by the sandbox
    wchar_t* LaunchProcess = NULL;

    // path to the file to be launched (not same as "LaunchProcess". see "Force process" mechanics of sandboxie)
    wchar_t* LaunchPath = NULL;

    /*
        "username" is the name of the sandbox where the process will run. 
        also this data will be written to the sandbox parameters as 
        "LauncherLogin" and "LauncherPassword"
    */
    AccountsMap* Accounts = NULL;

    mINI::INIStructure* AccountsData = NULL;

    // default parameters of new sandbox
    TextConfig* SandboxConfig = NULL;

    wchar_t* AdditionalCommand = NULL;

    wchar_t* DriverPath = NULL;

    wchar_t* SbieDllPath = NULL;

    wchar_t* SbieStartPath = NULL;

    Config();

    VOID LoadConfigs();

    VOID LoadAccounts();

private:
    ~Config() {}
};


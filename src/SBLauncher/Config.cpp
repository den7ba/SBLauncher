/*************************************************************************
*
* Configuration
*
*************************************************************************/

#include "stdafx.h"

Config::Config() {
    this->LoadConfigs();
    this->LoadAccounts();
}

VOID Config::LoadConfigs() {
    mINI::INIFile MainConfigurationFile("SB.ini");

    mINI::INIStructure MainConfiguration;

    if (!MainConfigurationFile.read(MainConfiguration)) {
        SoftExit("No config SB.ini");
    }

    this->LaunchProcess = new wchar_t[256];
    this->LaunchPath = new wchar_t[256];
    wchar_t* LaunchFolderPath = new wchar_t[256];
    wchar_t* SboxHostPath = new wchar_t[256];
    this->AdditionalCommand = new wchar_t[256];
    this->DriverPath = new wchar_t[256];
    this->SbieDllPath = new wchar_t[256];
    this->SbieStartPath = new wchar_t[256];

    wsprintfW(this->LaunchProcess, L"%S", MainConfiguration.get("main").get("LaunchProcess").c_str());
    wsprintfW(this->LaunchPath, L"%S", MainConfiguration.get("main").get("LaunchPath").c_str());
    wsprintfW(LaunchFolderPath, L"%S", MainConfiguration.get("main").get("LaunchFolderPath").c_str());
    wsprintfW(SboxHostPath, L"%S", MainConfiguration.get("main").get("SboxHostPath").c_str());
    wsprintfW(this->AdditionalCommand, L"%S", MainConfiguration.get("main").get("AdditionalCommand").c_str());
    wsprintfW(this->DriverPath, L"%S", MainConfiguration.get("main").get("DriverPath").c_str());
    wsprintfW(this->SbieDllPath, L"%S", MainConfiguration.get("main").get("SbieDllPath").c_str());
    wsprintfW(this->SbieStartPath, L"%S", MainConfiguration.get("main").get("SbieStartPath").c_str());

    this->SandboxConfig = new TextConfig;
    *(this->SandboxConfig) = {
        {L"Enabled",L"y"},
        {L"RecoverFolder",L"%{374DE290-123F-4565-9164-39C4925E467B}%"},
        {L"RecoverFolder",L"%Personal%"},
        {L"RecoverFolder",L"%Desktop%"},
        {L"BorderColor",L"#00ff00,off,6"},
        {L"Template",L"RpcPortBindingsExt"},
        {L"Template",L"OpenBluetooth"},
        {L"Template",L"SkipHook"},
        {L"Template",L"FileCopy"},
        {L"Template",L"qWave"},
        {L"Template",L"BlockPorts"},
        {L"Template",L"LingerPrograms"},
        {L"Template",L"AutoRecoverIgnore"},
        {L"ConfigLevel",L"9"},
        {L"AutoRecover",L"y"},
        {L"NoSecurityIsolation",L"y"},
        {L"InjectDll",SboxHostPath},
        {L"OpenFilePath",LaunchFolderPath},
        {L"BoxNameTitle",L"-"},
    };
}

VOID Config::LoadAccounts() {
    this->Accounts = new AccountsMap;

    mINI::INIFile AccountsFile("Accounts.ini");

    this->AccountsData = new mINI::INIStructure;

    if (!AccountsFile.read(*this->AccountsData)) {
        SoftExit("No config Accounts.ini");
    }

    size_t n_sections = this->AccountsData->size();

    if (!n_sections)
        SoftExit("Accounts not found! Check the Accounts.ini file\n");

    for (auto const& it : *this->AccountsData)
    {
        wchar_t* WideLogin = new wchar_t[50];
        wsprintfW(WideLogin, L"%S", it.first.c_str());
        this->Accounts->insert(std::pair<char*, wchar_t*>((char*)it.first.c_str(), WideLogin));
    }
}
/*************************************************************************
*
* SBLauncher
*
*************************************************************************/

#include "stdafx.h"

VOID RunALL();
VOID Worker();
BOOL Launch(wchar_t* boxName);
VOID PerformAdditionalActions();
VOID execAdditionalCommand();
VOID execProgram();
VOID SbieDllInit();
VOID Test();
Config* Conf();
KI* Driver();
VOID SoftExit(const char* reason);
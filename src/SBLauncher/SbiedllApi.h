/*************************************************************************
*
* SbieDll API
*
*************************************************************************/

#include "stdafx.h"

typedef LONG(__stdcall* P_SbieDll_UpdateConf)(
    WCHAR operation_code,
    const WCHAR* password,      // limited to 64 chars
    const WCHAR* section_name,  // limited to 32 chars
    const WCHAR* setting_name,  // limited to 64 chars
    const WCHAR* value);         // limited to 2000 chars

typedef LONG(__stdcall* P_SbieApi_ReloadConf)(
    ULONG session_id);

typedef LONG(__stdcall* P_SbieApi_EnumBoxes)(
    LONG index,                 // initialize to -1
    WCHAR* box_name);           // pointer to WCHAR [34]

typedef LONG (__stdcall* P_SbieApi_EnumProcessEx)(
    const WCHAR* box_name,          // WCHAR [34]
    BOOLEAN all_sessions,
    ULONG which_session,            // -1 for current session
    ULONG* boxed_pids,             // ULONG [512]
    ULONG* boxed_count);         // pointer to ULONG [512]
#include "pch.h"
#include "Debug.h"

#define MAX_DEBUG_MESSAGE_BUFFER    1024

void TVisu::DebugWrite(const char* pszFormat, ...)
{
    va_list arglist;
    char szMsg[MAX_DEBUG_MESSAGE_BUFFER];

    va_start(arglist, pszFormat);

    if (std::vsnprintf(szMsg, MAX_DEBUG_MESSAGE_BUFFER, pszFormat, arglist) > 0)
    {
        OutputDebugStringA(szMsg);
    }

    va_end(arglist);

    return;
}
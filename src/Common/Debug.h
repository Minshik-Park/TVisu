#pragma once
#include "ResultCode.h"
#include <string>

/// <summary>
/// Debug helper utilities
/// </summary>
namespace TVisu
{
#ifdef _DEBUG
    void DebugWrite(const char* pszFormat, ...);

    class _ScopeTrace
    {
    public:
        _ScopeTrace(const char* pFunc, result_t* pResult = nullptr) :
            m_pResult(pResult),
            m_functionName(pFunc)
        {
            DebugWrite("<%s>\n", m_functionName.c_str());
        }

        ~_ScopeTrace()
        {
            if (m_pResult)
            {
                DebugWrite("</%s Result=0x%08x>\n", m_functionName.c_str(), *m_pResult);
            }
            else
            {
                DebugWrite("</%s>\n", m_functionName.c_str());
            }
        }

        const char* GetFunctionName() const
        {
            return m_functionName.c_str();
        }

    private:
        result_t* m_pResult = nullptr;
        std::string m_functionName;
    };

#else
    inline void DebugWrite(const char*, ...) {}
#endif
}

// Debug Macros
#ifdef _DEBUG
#define SCOPETRACEN(func)       _ScopeTrace __a(func);
#define SCOPETRACE(func, pHr)   _ScopeTrace __a(func, pHr);
#else
#define SCOPETRACEN(func)
#define SCOPETRACE(func, pHr)
#endif
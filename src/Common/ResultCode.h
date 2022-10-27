#pragma once

/// <summary>
/// Result Codes and Macro
/// </summary>
#include <exception>

#ifndef WIN32
#define FACILITY_WIN32              7

#define HRESULT_FROM_WIN32(x)       (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000);

#define ERROR_ALREADY_INITIALIZE    1247
#define ERROR_INVALID_STATE         5023

#define E_OUTOFMEMORY               0x8007000E
#define E_POINTER                   0x80004003
#define E_INVALIDARG                0x80070057
#define E_NOTIMPL                   0x80004001
#define E_NOINTERFACE               0x80004002
#define E_FAIL                      0x80004005
#endif

namespace TVisu
{
    typedef int32_t result_t;

    namespace ResultCode
    {
        static const result_t outOfMemory = E_OUTOFMEMORY;
        static const result_t alreadyInitialized = HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);
        static const result_t invalidState = HRESULT_FROM_WIN32(ERROR_INVALID_STATE);
        static const result_t invalidPointer = E_POINTER;
        static const result_t invalidParameter = E_INVALIDARG;
        static const result_t notImplemented = E_NOTIMPL;
        static const result_t noInterface = E_NOINTERFACE;
        static const result_t failed = E_FAIL;

        static const result_t success = 0x00000000;
    }

    inline bool Succeeded(result_t result) { return result >= 0; }
    inline bool Failed(result_t result) { return result < 0; }

    class Exception : public std::exception
    {
    public:
        Exception(result_t result) :
            std::exception("TVisu Exception"),
            m_result(result)
        {
        }

        Exception(result_t result, char const* const pszMessage) :
            std::exception(pszMessage),
            m_result(result)
        {
        }

        result_t Result() const { return m_result; }

    private:
        result_t m_result;
    };
}

#define IF_FAILED_EXIT(s)   {\
                                result = (s);\
                                if(TVisu::Failed(result))\
                                {\
                                    DebugWrite("%s Failed (0x%08x)\n", __FUNCTION__, result);\
                                    goto Exit;\
                                }\
                            }
#define IF_FALSE_EXIT(c, r) {\
                                if(!(c))\
                                {\
                                    result = r;\
                                    DebugWrite("%s condition failed (0x%08x)\n", __FUNCTION__, result);\
                                    goto Exit;\
                                }\
                            }
#define IF_NULL_EXIT(p, r)  {\
                                if((p) == nullptr)\
                                {\
                                    result = r;\
                                    DebugWrite("%s null failed (0x%08x)\n", __FUNCTION__, result);\
                                    goto Exit;\
                                }\
                            }

inline void IF_FAILED_THROW(TVisu::result_t result, char const* const pszMessage = nullptr)
{
    if (TVisu::Failed(result))
    {
        throw new TVisu::Exception(result, pszMessage);
    }
}

inline void IF_FALSE_THROW(bool condition, const TVisu::result_t result = TVisu::ResultCode::failed, char const* const pszMessage = nullptr)
{
    if (!condition)
    {
        throw new TVisu::Exception(result, pszMessage);
    }
}

inline void IF_NULL_THROW(void* pPointer, const TVisu::result_t result = TVisu::ResultCode::failed, char const* const pszMessage = nullptr)
{
    if (pPointer == nullptr)
    {
        throw new TVisu::Exception(result, pszMessage);
    }
}

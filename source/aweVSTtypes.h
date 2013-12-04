// --------------------------------------------------------------------------
//
// Project       MeeblipVST
//
// File          Axel Werner
//
// Author        aweVSTtypes.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    19.08.2013  AWe   define some types which are defined for in VST3 as
//                      their VST2.x pendant. see also
//                       ...\VST3 SDK\pluginterfaces\base\ftypes.h
//
// --------------------------------------------------------------------------

#ifndef __aweVSTtypes__
#define __aweVSTtypes__


// --------------------------------------------------------------------------
// Integral Types
// --------------------------------------------------------------------------

   typedef char int8;
   typedef unsigned char uint8;
   typedef unsigned char uchar;

   typedef short int16;
   typedef unsigned short uint16;

#if MAC && __LP64__
   typedef int int32;
   typedef unsigned int uint32;
#else
   typedef long int32;
   typedef unsigned long uint32;
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

typedef int32  tresult;        // result code
typedef double ParamValue;     // parameter value type
typedef uint32 ParamID;        // parameter identifier
typedef int32  ProgramListID;  // program list identifier
typedef int16  CtrlNumber;     // MIDI controller number( see \ref ControllerNumbers for allowed values)


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// from ...\VST3 SDK\pluginterfaces\base\funknown.h

#if COM_COMPATIBLE
 #if WINDOWS
enum
{
   kNoInterface      = static_cast<tresult>(0x80004002L),   // E_NOINTERFACE
   kResultOk         = static_cast<tresult>(0x00000000L),   // S_OK
   kResultTrue       = kResultOk,
   kResultFalse      = static_cast<tresult>(0x00000001L),   // S_FALSE
   kInvalidArgument  = static_cast<tresult>(0x80070057L),   // E_INVALIDARG
   kNotImplemented   = static_cast<tresult>(0x80004001L),   // E_NOTIMPL
   kInternalError    = static_cast<tresult>(0x80004005L),   // E_FAIL
   kNotInitialized   = static_cast<tresult>(0x8000FFFFL),   // E_UNEXPECTED
   kOutOfMemory      = static_cast<tresult>(0x8007000EL)    // E_OUTOFMEMORY
};
 #else
enum
{
   kNoInterface      = static_cast<tresult>(0x80000004L),   // E_NOINTERFACE
   kResultOk         = static_cast<tresult>(0x00000000L),   // S_OK
   kResultTrue       = kResultOk,
   kResultFalse      = static_cast<tresult>(0x00000001L),   // S_FALSE
   kInvalidArgument  = static_cast<tresult>(0x80000003L),   // E_INVALIDARG
   kNotImplemented   = static_cast<tresult>(0x80000001L),   // E_NOTIMPL
   kInternalError    = static_cast<tresult>(0x80000008L),   // E_FAIL
   kNotInitialized   = static_cast<tresult>(0x8000FFFFL),   // E_UNEXPECTED
   kOutOfMemory      = static_cast<tresult>(0x80000002L)    // E_OUTOFMEMORY
};
 #endif
#else
enum
{
   kNoInterface      = -1,
   kResultOk,
   kResultTrue       = kResultOk,
   kResultFalse,
   kInvalidArgument,
   kNotImplemented,
   kInternalError,
   kNotInitialized,
   kOutOfMemory
};
#endif

#define kVstExtMaxParamStrLen 32

#endif // __aweVSTtypes__

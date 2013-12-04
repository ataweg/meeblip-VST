// --------------------------------------------------------------------------
//
// Project       - generic -
//
// File          Axel Werner
//
// Author        aweDBG.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    09.08.2013  AWe   define some useful simple debug macros
//
// References
//     http://c.learncodethehardway.org/book/ex20.html
//
// --------------------------------------------------------------------------

#ifndef __AWEDBG_H__
#define __AWEDBG_H__

#ifndef VERBOSITY
   #define VERBOSITY       100   // the higher, the more babble
#endif
#ifndef VERBOSITY_MIN
   #define VERBOSITY_MIN   0     //
#endif

#include <stdio.h>

#ifdef _DEBUG_CHATTY
   #ifdef _DEBUG
      #define DBG( verbose_level, msg, ...)  if(( VERBOSITY_MIN <= verbose_level) &&( verbose_level <= VERBOSITY) ) printf(stderr, "DEBUG %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
      #define DBG2( verbose_level, msg, ...) if(( VERBOSITY_MIN <= verbose_level) &&( verbose_level <= VERBOSITY) ) fprintf(stderr, "DEBUG %s:%d: " msg, __FILE__, __LINE__, ##__VA_ARGS__)
   #else
      #define DBG( verbose_level, msg, ...)
      #define DBG2( verbose_level, msg, ...)
   #endif

#else
   #ifdef _DEBUG
      #define DBG( verbose_level, msg, ...)  if(( VERBOSITY_MIN <= verbose_level) &&( verbose_level <= VERBOSITY) ) fprintf(stderr, msg "\n", ##__VA_ARGS__)
      #define DBG2( verbose_level, msg, ...) if(( VERBOSITY_MIN <= verbose_level) &&( verbose_level <= VERBOSITY) ) fprintf(stderr, msg, ##__VA_ARGS__)
   #else
      #define DBG( verbose_level, msg, ...)
      #define DBG2( verbose_level, msg, ...)
   #endif

#endif   // _DEBUG_CHATTY
#endif  // __AWEDBG_H__
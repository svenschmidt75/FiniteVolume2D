/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

// EXPORT_SYMBOLS is defined by the compiler in this project only
#ifdef SOLVER_EXPORTS
#define DECL_SYMBOLS __declspec(dllexport)
//#define EXPIMP_TEMPLATE
#else
#define DECL_SYMBOLS __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
#endif

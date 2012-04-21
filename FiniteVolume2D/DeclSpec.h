/*
 * Name  : DeclSpec
 * Path  : 
 * Use   : Export symbols into DLL
 * Author: Sven Schmidt
 * Date  : 04/21/2012
 */
#pragma once

// EXPORT_SYMBOLS is defined by the compiler in this project only
#ifdef FINITEVOLUME2D_EXPORTS
#define DECL_SYMBOLS_2D __declspec(dllexport)
//#define EXPIMP_TEMPLATE
#else
#define DECL_SYMBOLS_2D __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
#endif

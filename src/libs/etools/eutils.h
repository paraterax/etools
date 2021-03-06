/// =====================================================================================
///
///       Filename:  eutils.h
///
///    Description:  tools for etools
///
///        Version:  1.0
///        Created:  09/27/2017 05:58:18 PM
///       Revision:  none
///       Compiler:  gcc
///
///         Author:  Haitao Yang, joyhaitao@foxmail.com
///        Company:
///
/// =====================================================================================

#ifndef __EUTILS_H__
#define __EUTILS_H__

#include <string.h>
#include "etype.h"

#ifdef __cplusplus
extern "C" {
#endif

/** =====================================================
 *
 *  llog tools
 *
 * ------------------------------------------------------
 */
#ifdef  EUTILS_LLOG
#define EUTILS_LLOG_DBG 0
#define EUTILS_LLOG_INF 1
#define EUTILS_LLOG_WRN 2
#define EUTILS_LLOG_ERR 3
#ifdef _WIN32
static constr _llog_basename(constr path){static constr slash; if (slash) {return slash + 1;}else{slash = strrchr(path, '\\');}if (slash) {return slash + 1;}return 0;}
#else
static constr _llog_basename(constr path){static constr slash; if (slash) {return slash + 1;}else{slash = strrchr(path, '/');}if (slash) {return slash + 1;}return 0;}
#endif
#define _log(fmt, ...)       fprintf(stdout, "%s(%d):" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define _log_dbg(fmt, ...)   fprintf(stdout, "%s(%d) DBG:" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define _log_inf(fmt, ...)   fprintf(stdout, "%s(%d) INF:" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define _log_wrn(fmt, ...)   fprintf(stdout, "%s(%d) WRN:" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#define _log_err(fmt, ...)   fprintf(stderr, "%s(%d) ERR:" fmt "%s", _llog_basename(__FILE__), __LINE__, __VA_ARGS__)
#ifndef _WIN32
#define _llog(...)           _log(__VA_ARGS__, "\n");fflush(stdout)
#define _llog_dbg(...)       _log_dbg(__VA_ARGS__, "\n");fflush(stdout)
#define _llog_inf(...)       _log_inf(__VA_ARGS__, "\n");fflush(stdout)
#define _llog_wrn(...)       _log_wrn(__VA_ARGS__, "\n");fflush(stdout)
#define _llog_err(...)       _log_err(__VA_ARGS__, "\n");fflush(stdout)
#else
#ifdef _MSC_VER
#define _llog(fmt, ...)      fprintf(stdout, "%s(%d):" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_dbg(fmt, ...)  fprintf(stdout, "%s(%d) DBG:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_inf(fmt, ...)  fprintf(stdout, "%s(%d) INF:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_wrn(fmt, ...)  fprintf(stdout, "%s(%d) WRN:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_err(fmt, ...)  fprintf(stderr, "%s(%d) ERR:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stderr)
#else
#define _llog(fmt, ...)      fprintf(stdout, "%s(%d):" fmt "\n", _llog_basename(__FILE__), __LINE__, ##__VA_ARGS__);fflush(stdout)
#define _llog_dbg(fmt, ...)  fprintf(stdout, "%s(%d) DBG:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_inf(fmt, ...)  fprintf(stdout, "%s(%d) INF:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_wrn(fmt, ...)  fprintf(stdout, "%s(%d) WRN:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stdout)
#define _llog_err(fmt, ...)  fprintf(stderr, "%s(%d) ERR:" fmt "\n", _llog_basename(__FILE__), __LINE__, __VA_ARGS__);fflush(stderr)
#endif
#endif

#if (EUTILS_LLOG+0)
#define llog                _llog
#define llog_dbg            _llog_dbg
#define llog_inf            _llog_inf
#define llog_wrn            _llog_wrn
#define llog_err            _llog_err

#ifdef EUTILS_LLOG_LEVEL

#if EUTILS_LLOG_LEVEL + 0 > 0
#undef  llog_dbg
#define llog_dbg(...)
#endif

#if EUTILS_LLOG_LEVEL + 0 > 1
#undef  llog_inf
#define llog_inf(...)
#endif

#if EUTILS_LLOG_LEVEL + 0 > 2
#undef  llog_wrn
#define llog_wrn(...)
#endif

#if EUTILS_LLOG_LEVEL + 0 > 3
#undef  llog_err
#define llog_err(...)
#endif

#undef EUTILS_LLOG_LEVEL
#endif

#else
#define llog(...)
#define llog_dbg(...)
#define llog_inf(...)
#define llog_wrn(...)
#define llog_err(...)
#endif
#else
#define _llog(...)
#define _llog_dbg(...)
#define _llog_inf(...)
#define _llog_wrn(...)
#define _llog_err(...)
#define llog(...)
#define llog_dbg(...)
#define llog_inf(...)
#define llog_wrn(...)
#define llog_err(...)
#endif

#ifdef  __ELOG_H__
#undef  _llog
#undef  _llog_dbg
#undef  _llog_inf
#undef  _llog_wrn
#undef  _llog_err
#define _llog(...)           elog_inf(0, __VA_ARGS__);
#define _llog_dbg(...)       elog_dbg(0, __VA_ARGS__);
#define _llog_inf(...)       elog_inf(0, __VA_ARGS__);
#define _llog_wrn(...)       elog_wrn(0, __VA_ARGS__);
#define _llog_err(...)       elog_err(0, __VA_ARGS__);
#endif


/** =====================================================
 *
 *  some macro helpers
 *
 * ------------------------------------------------------
 */

#define exe_ret(expr, ret ) { expr;      return ret;}
#define is0_ret(cond, ret ) if(!(cond)){ return ret;}
#define is1_ret(cond, ret ) if( (cond)){ return ret;}
#define is0_exe(cond, expr) if(!(cond)){ expr;}
#define is1_exe(cond, expr) if( (cond)){ expr;}

#define is0_exeret(cond, expr, ret) if(!(cond)){ expr;        return ret;}
#define is1_exeret(cond, expr, ret) if( (cond)){ expr;        return ret;}
#define is0_elsret(cond, expr, ret) if(!(cond)){ expr;} else{ return ret;}
#define is1_elsret(cond, expr, ret) if( (cond)){ expr;} else{ return ret;}

#undef  container_of
#undef  offsetof
#define container_of(ptr, type, member) ((type *)( (char *)ptr - offsetof(type,member) ))   // hava a operation, minimize related operations
#define offsetof(TYPE, MEMBER)          ((size_t) &((TYPE *)0)->MEMBER)

#define is_eq(a, b) ((a) == (b))
#define un_eq(a, b) ((a) != (b))

#define E_UNUSED(p) (void)p

/** =====================================================
 *
 *  math tools
 *
 * ------------------------------------------------------
 */
static __always_inline int pow2gt(int x)	{	--x;	x|=x>>1;	x|=x>>2;	x|=x>>4;	x|=x>>8;	x|=x>>16;	return x+1;	}

/** =====================================================
 *
 *  string tools
 *
 * ------------------------------------------------------
 */

/**
 * doing the actual number -> string conversion. 's' must point
 * to a string with room for at least 21 bytes.
 *
 * The function returns the length of the null-terminated string
 * representation stored at 's'.
*/
int  ll2str(i64 v, char *s);
int ull2str(u64 v, char *s);


/** =====================================================
 *
 *  string tools
 *
 * ------------------------------------------------------
 */

i64  eutils_nowns();
i64  eutils_nowms();

int  eutils_rand();

int  eutils_nprocs();

cstr eutils_version();

#ifdef __cplusplus
}
#endif

#endif

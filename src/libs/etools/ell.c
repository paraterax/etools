/// =====================================================================================
///
///       Filename:  ell.c
///
///    Description:  an easier double link list
///
///        Version:  1.0
///        Created:  05/04/2017 04:50:18 PM
///       Revision:  none
///       Compiler:  gcc
///
///         Author:  Haitao Yang, joyhaitao@foxmail.com
///        Company:
///
/// =====================================================================================

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "eutils.h"
#include "ell.h"
#include "_eobj_header.h"

#define ELL_VERSION "ell 1.2.1"

#pragma pack(1)

typedef struct _ell_node_s*  _elln;

typedef struct _ell_node_link_s{
    _elln       prev;
    _elln       next;
}_ellnl_t;

typedef struct _ell_node_s{
    _ellnl_t    link;
    _ehdr_t     hdr;
    eobj_t      obj;
}_elln_t;

typedef struct _ell_s{
    _elln       head;
    _elln       tail;
    _elln       hisn;
    __u32       hisi;
}_ell_t;

typedef struct _ell_root_s{
    _ellnl_t    link;
    _ehdr_t     hdr;
    _ell_t      list;
}_ellr_t, * _ellr;

#pragma pack()

/// -------------------------- micros helper ---------------------------------

#define _CUR_C_TYPE         ELL

#define _DNODE_TYPE         _elln_t
#define _NDODE_LNK_FIELD
#define _DNODE_KEY_FIELD
#define _DNODE_HDR_FIELD    hdr
#define _DNODE_OBJ_FIELD    obj

#define _RNODE_TYPE         _ellr_t
#define _RNODE_TYP_FIELD    type
#define _RNODE_OBJ_FIELD    list

#define _c_r(l)             container_of(l, _RNODE_TYPE, _RNODE_OBJ_FIELD)
#define _c_head(l)          (l)->head
#define _c_tail(l)          (l)->tail
#define _c_hisn(l)          (l)->hisn
#define _c_hisi(l)          (l)->hisi
#define _c_len(l)           _eo_len(l)
#define _c_typeco(l)        _eo_typeco(l)
#define _c_typeco_set(l)    _eo_typeco(l) = _cur_type(_CUR_C_TYPE, CO_OBJ)
#define _c_free(l)          _r_free(_c_r(l))

#define _n_prev(n)          (n)->link.prev
#define _n_next(n)          (n)->link.next

#define _check_l (l)        is0_ret(l, 0);
#define _check_lo(l, o)     is1_ret(!l || !o, 0); is1_ret(_eo_linked(o) || _eo_typec(o) != ELL, 0);

#define _ell_init(l)        memset(_c_r(l), 0, sizeof(_RNODE_TYPE))


#define _elist_push(l, n)                                               \
do{                                                                     \
    _n_linked(n)  = 1;                                                  \
                                                                        \
    if(!_c_head(l)){_c_head(l) =         _c_tail(l)  = n;}              \
                                                                        \
    else           {_n_next(n) =         _c_head(l)     ;               \
                    _c_head(l) = _n_prev(_c_head(l)) = n;               \
                    if(_c_hisi(l)) _c_hisi(l)++         ;}              \
                                                                        \
    _c_len(l)++;                                                        \
}while(0)

#define _elist_takeH(l, n)                                              \
do{                                                                     \
    n = _c_head(l);                                                     \
                                                                        \
    if(1 == _c_len(l)){_c_head(l) = _c_tail(l) = NULL      ;}           \
                                                                        \
    else              {_c_head(l)              = _n_next(n);            \
                       _n_prev(_n_next(n))     = NULL      ;            \
                       if(_c_hisi(l)) _c_hisi(l)--         ;            \
                      }                                                 \
                                                                        \
    _c_len(l)--;                                                        \
                                                                        \
    _n_next(n)    = NULL;                                               \
    _n_linked(n)  = 0;                                                  \
}while(0)

#define _elist_appd(l, n)                                               \
do{                                                                     \
    _n_linked(n)  = 1;                                                  \
                                                                        \
    if(!_c_head(l)){_c_head(l) =         _c_tail(l)  = n;}              \
                                                                        \
    else           {_n_prev(n) =         _c_tail(l)     ;               \
                    _c_tail(l) = _n_next(_c_tail(l)) = n;}              \
                                                                        \
    _c_len(l)++;                                                        \
}while(0)

#define _elist_takeT(l, n)                                              \
do{                                                                     \
    n = _c_tail(l);                                                     \
                                                                        \
    if(1 == _c_len(l)){_c_head(l) = _c_tail(l) = NULL;}                 \
                                                                        \
    else              {_c_tail(l)              = _n_prev(n);            \
                       _n_next(_n_prev(n))     = NULL      ;            \
                       if(_c_hisi(l) && _c_hisn(l) == n)                \
                       {_c_hisn(l) = _n_prev(n); _c_hisi(l)--;}         \
                      }                                                 \
                                                                        \
    _c_len(l)--;                                                        \
                                                                        \
    _n_prev(n)    = NULL;                                               \
    _n_linked(n)  = 0;                                                  \
}while(0)

//! _elist_at(ell l, int idx, _elln n)
//!
//!  find the node which is at the idx in the list
//!
//!  the n is the output search node
//!
#define _elist_at(l, idx, n)                                            \
do{                                                                     \
    uint i;                                                             \
                                                                        \
    if(idx >= _c_hisi(l))   { if(idx - _c_hisi(l) < _c_len(l) - 1 - idx) { n = _c_hisi(l) ? _c_hisn(l) : _c_head(l); for(i = _c_hisi(l)   ; i != idx; i++) n = _n_next(n);}     \
                              else                                       { n = _c_tail(l);                           for(i = _c_len(l) - 1; i != idx; i--) n = _n_prev(n);}     \
                            }                                           \
    else                    { if(_c_hisi(l) - idx > idx)                 { n = _c_head(l);                           for(i = 0            ; i != idx; i++) n = _n_next(n);}     \
                              else                                       { n = _c_hisn(l);                           for(i = _c_hisi(l)   ; i != idx; i--) n = _n_prev(n);}     \
                            }                                           \
    _c_hisi(l) = idx;                                                   \
    _c_hisn(l) = n;                                                     \
                                                                        \
}while(0)

//! _elist_find(l, o, i)
//!
//!  get the idx of node o in the list
//!
//!  the i is the output idx needed
//!
#define _elist_find(l, o, i)                                            \
do{                                                                     \
    uint cnt;                                                           \
                                                                        \
    o = (eobj)_eo_dn(o);                                                \
                                                                        \
    if(_c_hisi(l)){        for(i = _c_hisi(l)    , n =         _c_hisn(l) , cnt = 0; n && n != (_elln)o; n = _n_next(n), i++) if(cnt++ > 5) break;        \
                    if(!n) for(i = _c_hisi(l) - 1, n = _n_prev(_c_hisn(l)), cnt = 0; n && n != (_elln)o; n = _n_prev(n), i--) if(cnt++ > 5) break; }      \
    else          {        for(i = 0             , n = _c_head(l)         , cnt = 0; n && n != (_elln)o; n = _n_next(n), i++); }                          \
                                                                                                                                                            \
}while(0)

/// _elist_take(ell l, _elln n, int idx)
#define _elist_take(l, n, idx)                                                                          \
do{                                                                                                     \
    if   (_n_prev(n)) _n_next(_n_prev(n)) = _n_next(n);                                                 \
    else              _c_head(l)          = _n_next(n);                                                 \
                                                                                                        \
    if   (_n_next(n)){_n_prev(_c_hisn(l) = _n_next(n)) = _n_prev(n);            _c_hisi(l) = idx    ;}  \
    else             {        _c_hisn(l) = _c_tail(l)  = _n_prev(n);    if(idx) _c_hisi(l) = idx - 1;}  \
                                                                                                        \
    _c_len(l)--;                                                                                        \
                                                                                                        \
    _n_prev(n)      =                                                                                   \
    _n_next(n)      = NULL;                                                                             \
    _n_linked(n)    = 0;                                                                                \
                                                                                                        \
}while(0)

#if 1
#define _node_update(l, o, n)                                           \
if(o != n)                                                              \
{                                                                       \
    if(_n_prev(n)){ _n_next(_n_prev(n)) = n; } else _c_head(l) = n;     \
    if(_n_next(n)){ _n_prev(_n_next(n)) = n; } else _c_tail(l) = n;     \
    if(_c_hisn(l) == o) _c_hisn(l) = n;                                 \
}
#else
#define _node_update(l, o, n)                                           \
if(o != n)                                                              \
{                                                                       \
    _elln prev = _n_prev(o),                                            \
          next = _n_next(o);                                            \
                                                                        \
    if(prev){ _n_next(prev) = n; _n_prev(n) = prev; } else _c_head(l) = n;        \
    if(next){ _n_prev(next) = n; _n_next(n) = next; } else _c_tail(l) = n;        \
}
#endif
/// -----------------------  inline compat ------------------------
#if defined(WIN32) && !defined(__cplusplus)
#define inline
#endif


/// -------------------------- elist ------------------------------
ell ell_new()
{
    _ellr r = (_ellr)_r_new();

    _r_typeco_set(r);

    //_ell_init(_r_o(r));

    return _r_o(r);
}

eobj ell_newO(etypeo type, uint len)
{
    _elln n;

    switch (type) {
        case EFALSE :  _n_newTF(n);      break;
        case ETRUE  :  _n_newTT(n);      break;
        case ENULL  :  _n_newTN(n);      break;
        case ENUM   :  _n_newIc(n);      break;
        case EPTR   :  _n_newPc(n);      break;
        case ESTR   :  _n_newSc(n, len); break;
        case ERAW   :  _n_newRc(n, len); break;
        case EOBJ   :  _n_newOc(n);      break;

        default     :   return 0;

    }

    return _n_o(n);
}

inline uint  ell_len    (ell  l) { return l ?  _c_len(l) :  0 ; }
inline uint  ell_size   (ell  l) { return l ?  _c_len(l) :  0 ; }
inline bool  ell_isEmpty(ell  l) { return l ? !_c_len(l) :  0 ; }

ell ell_clear  (ell l) { return ell_clearEx(l, 0);}
ell ell_clearEx(ell l, eobj_rls_cb rls)
{
    _elln n, itr;

    is0_ret(l  , 0);

    if(rls)
    {
        for(n = _c_head(l); (itr = n); )
        {
            n = _n_next(n);

            switch (_n_typeo(itr))
            {
                case EPTR:
                case ERAW: rls(_n_o(itr));
                           break;

                case EOBJ: ell_freeEx((ell)_n_o(itr), rls);
                           continue;
            }

            _n_free(itr);
        }
    }
    else
    {
        for(n = _c_head(l); (itr = n); )
        {
            n = _n_next(n);

            if(_n_typeo(itr) == EOBJ)
                ell_free((ell)_n_o(itr));
            else
                _n_free(itr);
        }
    }

    _ell_init(l);

    return l;
}

int ell_free(ell l)
{
    _elln n, itr;

    is0_ret(l, 0);

    for(n = _c_head(l); (itr = n); )
    {
        n = _n_next(n);
        _n_free(itr);
    }

    _c_free(l);

    return 1;
}

int ell_freeEx(ell l, eobj_rls_cb rls)
{
    _elln n, itr;

    is0_ret(rls, ell_free(l));
    is0_ret(l  , 0);

    for(n = _c_head(l); (itr = n); )
    {
        n = _n_next(n);

        rls(_n_o(itr));
        _n_free(itr);
    }

    _c_free(l);

    return 1;
}

eobj  ell_pushI(ell l, i64    val){ _elln n; is0_ret(l, 0);     _n_newI (n, val); _elist_push(l, n); return _n_o(n);}
eobj  ell_pushF(ell l, f64    val){ _elln n; is0_ret(l, 0);     _n_newF (n, val); _elist_push(l, n); return _n_o(n);}
eobj  ell_pushS(ell l, constr str){ _elln n; is0_ret(l, 0);     _n_newS (n, str); _elist_push(l, n); return _n_o(n);}
eobj  ell_pushP(ell l, conptr ptr){ _elln n; is0_ret(l, 0);     _n_newP (n, ptr); _elist_push(l, n); return _n_o(n);}
eobj  ell_pushR(ell l, size_t len){ _elln n; is0_ret(l, 0);     _n_newRc(n, len); _elist_push(l, n); return _n_o(n);}
eobj  ell_pushO(ell l, eobj   obj){ _elln n; _check_lo(l, obj); n = _eo_dn(obj);  _elist_push(l, n); return obj    ;}

eobj  ell_appdI(ell l, i64    val){ _elln n; is0_ret(l, 0);     _n_newI (n, val); _elist_appd(l, n); return _n_o(n);}
eobj  ell_appdF(ell l, f64    val){ _elln n; is0_ret(l, 0);     _n_newF (n, val); _elist_appd(l, n); return _n_o(n);}
eobj  ell_appdS(ell l, constr str){ _elln n; is0_ret(l, 0);     _n_newS (n, str); _elist_appd(l, n); return _n_o(n);}
eobj  ell_appdP(ell l, conptr ptr){ _elln n; is0_ret(l, 0);     _n_newP (n, ptr); _elist_appd(l, n); return _n_o(n);}
eobj  ell_appdR(ell l, size_t len){ _elln n; is0_ret(l, 0);     _n_newRc(n, len); _elist_appd(l, n); return _n_o(n);}
eobj  ell_appdO(ell l, eobj   obj){ _elln n; _check_lo(l, obj); n = _eo_dn(obj);  _elist_appd(l, n); return obj    ;}

static eobj _ell_setRaw(ell l, int idx, eval v, uint need_len, _eotype type)
{
    _elln n; uint cur_len;

    is0_ret(l, 0);

    if(idx < 0) idx = idx + _c_len(l);

    is1_ret(idx < 0 || (uint)idx > (_c_len(l) - 1), 0);

    _elist_at(l, (uint)idx, n);

    switch(_n_typeo(n))
    {
        case EFALSE:
        case ETRUE :
        case ENULL : cur_len = _n_len(n); break;
        case ENUM  : cur_len = 8; break;
        case EPTR  : cur_len = 8; break;
        case ESTR  : cur_len = _n_len(n); break;
        case ERAW  : cur_len = _n_len(n); break;
        case EOBJ  : if(_EOBJ == type)   return _n_o(n);
                     cur_len = 8;
                     ell_free(_n_valP(n)); break;
        default:
            return 0;
    }

    if(cur_len < need_len)
    {
        _elln newn = _n_newr(n, need_len);

        if(newn != n)
        {
            _node_update(l, n, newn);
            n = newn;
        }
    }

    switch((int)type)
    {
        case _ELL_COE_NUM_I:
        case _ELL_COE_NUM_F:
        case _ELL_COE_PTR  : _n_setV (n, v            ); break;
        case _ELL_COE_STR  : _n_setS (n, v.s, need_len); break;
        case _ELL_COE_RAW  : _n_wipeR(n,      need_len); _n_len(n) = need_len; break;
    }

    _n_typecoe(n) = type;

    return _n_o(n);
}

eobj ell_setI(ell l, int idx, i64    val) { return _ell_setRaw(l, idx, *(eval*)&val,                     8, _ELL_COE_NUM_I); }
eobj ell_setF(ell l, int idx, f64    val) { return _ell_setRaw(l, idx, *(eval*)&val,                     8, _ELL_COE_NUM_F); }
eobj ell_setP(ell l, int idx, conptr val) { return _ell_setRaw(l, idx, *(eval*)&val,                     8, _ELL_COE_PTR  ); }
eobj ell_setS(ell l, int idx, constr str) { return _ell_setRaw(l, idx, *(eval*)&str, str ? strlen(str) : 0, _ELL_COE_STR  ); }
eobj ell_setR(ell l, int idx, size_t len) { return _ell_setRaw(l, idx, *(eval*)&len,                   len, _ELL_COE_RAW  ); }


inline eobj  ell_first(ell    l) { return _c_head(l)           ? _n_o(_c_head(l))           : 0; }
inline eobj  ell_last (ell    l) { return _c_tail(l)           ? _n_o(_c_tail(l))           : 0; }
inline eobj  ell_next (eobj obj) { return _n_next(_eo_dn(obj)) ? _n_o(_n_next(_eo_dn(obj))) : 0; }
inline eobj  ell_prev (eobj obj) { return _n_prev(_eo_dn(obj)) ? _n_o(_n_prev(_eo_dn(obj))) : 0; }

eobj ell_takeH (ell l)           { _elln n;                  is1_ret(!l || !_c_len(l), 0);                               _elist_takeH(l, n);                     return _n_o(n); }
eobj ell_takeT (ell l)           { _elln n;                  is1_ret(!l || !_c_len(l), 0);                               _elist_takeT(l, n);                     return _n_o(n); }
eobj ell_takeAt(ell l, uint idx) { _elln n;                  is1_ret(!l || idx >= _c_len(l), 0); _elist_at  (l, idx, n); _elist_take(l, n, idx);                 return _n_o(n); }
eobj ell_takeO (ell l, eobj obj) { _elln n; register uint i; is1_ret(!l || !obj, 0);             _elist_find(l, obj, i); is1_elsret(n, _elist_take(l, n, i), 0); return _n_o(n); }

eobj  ell_at  (ell l, uint idx) { _elln n; is1_ret(!l || idx >= _c_len(l), 0); _elist_at(l, idx, n); return _n_o(n); }
eobj  ell_val (ell l, uint idx) { _elln n; is1_ret(!l || idx >= _c_len(l), 0); _elist_at(l, idx, n); return _n_o(n); }
i64   ell_valI(ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retI(o); }
f64   ell_valF(ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retF(o); }
cstr  ell_valS(ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retS(o); }
cptr  ell_valP(ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retP(o); }
cptr  ell_valR(ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retR(o); }

etypeo ell_valType  (ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retT(o); }
uint   ell_valLen   (ell l, uint idx) { eobj o = ell_at(l, idx); _eo_retL(o); }
bool   ell_valIsTrue(ell l, uint idx) { return __eobj_isTrue(ell_at(l, idx)); }

int   ell_freeH(ell l)          { cptr o = ell_takeH (l     ); if(o) { _n_free(_eo_dn(o)); return 1; } return 0; }
int   ell_freeT(ell l)          { cptr o = ell_takeT (l     ); if(o) { _n_free(_eo_dn(o)); return 1; } return 0; }
int   ell_freeI(ell l, uint idx){ cptr o = ell_takeAt(l, idx); if(o) { _n_free(_eo_dn(o)); return 1; } return 0; }
int   ell_freeO(ell l, eobj obj)
{
    is0_ret(obj, 0);

    if(!l)
    {
        is1_ret(_eo_linked(obj) || _eo_typec(obj) != ELL, 0);
        exe_ret(_eo_free(obj), 1);
    }

    is1_exeret(obj = ell_takeO(l, obj), _eo_free(obj), 1);

    return 0;
}

constr ell_version()
{
    return ELL_VERSION;
}



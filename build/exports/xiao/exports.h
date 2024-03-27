
#ifndef XIAO_EXPORT_H
#define XIAO_EXPORT_H

#ifdef XIAO_STATIC_DEFINE
#  define XIAO_EXPORT
#  define XIAO_NO_EXPORT
#else
#  ifndef XIAO_EXPORT
#    ifdef xiao_EXPORTS
        /* We are building this library */
#      define XIAO_EXPORT 
#    else
        /* We are using this library */
#      define XIAO_EXPORT 
#    endif
#  endif

#  ifndef XIAO_NO_EXPORT
#    define XIAO_NO_EXPORT 
#  endif
#endif

#ifndef XIAO_DEPRECATED
#  define XIAO_DEPRECATED __declspec(deprecated)
#endif

#ifndef XIAO_DEPRECATED_EXPORT
#  define XIAO_DEPRECATED_EXPORT XIAO_EXPORT XIAO_DEPRECATED
#endif

#ifndef XIAO_DEPRECATED_NO_EXPORT
#  define XIAO_DEPRECATED_NO_EXPORT XIAO_NO_EXPORT XIAO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef XIAO_NO_DEPRECATED
#    define XIAO_NO_DEPRECATED
#  endif
#endif

#endif /* XIAO_EXPORT_H */

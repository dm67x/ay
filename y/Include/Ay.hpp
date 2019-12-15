
#ifndef AY_API_H
#define AY_API_H

#ifdef AY_STATIC_DEFINE
#  define AY_API
#  define AY_NO_EXPORT
#else
#  ifndef AY_API
#    ifdef ay_EXPORTS
        /* We are building this library */
#      define AY_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define AY_API __declspec(dllimport)
#    endif
#  endif

#  ifndef AY_NO_EXPORT
#    define AY_NO_EXPORT 
#  endif
#endif

#ifndef AY_DEPRECATED
#  define AY_DEPRECATED __declspec(deprecated)
#endif

#ifndef AY_DEPRECATED_EXPORT
#  define AY_DEPRECATED_EXPORT AY_API AY_DEPRECATED
#endif

#ifndef AY_DEPRECATED_NO_EXPORT
#  define AY_DEPRECATED_NO_EXPORT AY_NO_EXPORT AY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef AY_NO_DEPRECATED
#    define AY_NO_DEPRECATED
#  endif
#endif

#endif /* AY_API_H */

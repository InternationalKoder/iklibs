
#ifndef IKCONF_EXPORT_H
#define IKCONF_EXPORT_H

#ifdef IKCONF_BUILT_AS_STATIC
#  define IKCONF_EXPORT
#  define IKCONF_NO_EXPORT
#else
#  ifndef IKCONF_EXPORT
#    ifdef ikconf_EXPORTS
        /* We are building this library */
#      define IKCONF_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define IKCONF_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef IKCONF_NO_EXPORT
#    define IKCONF_NO_EXPORT 
#  endif
#endif

#ifndef IKCONF_DEPRECATED
#  define IKCONF_DEPRECATED __declspec(deprecated)
#endif

#ifndef IKCONF_DEPRECATED_EXPORT
#  define IKCONF_DEPRECATED_EXPORT IKCONF_EXPORT IKCONF_DEPRECATED
#endif

#ifndef IKCONF_DEPRECATED_NO_EXPORT
#  define IKCONF_DEPRECATED_NO_EXPORT IKCONF_NO_EXPORT IKCONF_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IKCONF_NO_DEPRECATED
#    define IKCONF_NO_DEPRECATED
#  endif
#endif

#endif /* IKCONF_EXPORT_H */

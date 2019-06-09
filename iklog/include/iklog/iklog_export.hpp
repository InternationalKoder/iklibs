
#ifndef IKLOG_EXPORT_H
#define IKLOG_EXPORT_H

#ifdef IKLOG_BUILT_AS_STATIC
#  define IKLOG_EXPORT
#  define IKLOG_NO_EXPORT
#else
#  ifndef IKLOG_EXPORT
#    ifdef iklog_EXPORTS
        /* We are building this library */
#      define IKLOG_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define IKLOG_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef IKLOG_NO_EXPORT
#    define IKLOG_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef IKLOG_DEPRECATED
#  define IKLOG_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef IKLOG_DEPRECATED_EXPORT
#  define IKLOG_DEPRECATED_EXPORT IKLOG_EXPORT IKLOG_DEPRECATED
#endif

#ifndef IKLOG_DEPRECATED_NO_EXPORT
#  define IKLOG_DEPRECATED_NO_EXPORT IKLOG_NO_EXPORT IKLOG_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IKLOG_NO_DEPRECATED
#    define IKLOG_NO_DEPRECATED
#  endif
#endif

#endif /* IKLOG_EXPORT_H */

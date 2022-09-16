#ifndef _VERSION_H_
#define _VERSION_H_
/******************************************
 * @brief IT'S AUTOMATICAL GENERATED FILE!
 *  DON'T change version.h file!!!
 ******************************************/

#define _VERSION_MAJOR 0
#define _VERSION_MINOR 0

#define _CHECK_VERSION(a,b) ((_VERSION_MAJOR == (a)) &&  (_VERSION_MINOR >=(b)))

#if _CHECK_VERSION(0,0)
#ifndef _VERSION_0_0
#define _VERSION_0_0
#endif
#elif _CHECK_VERSION(0,1)
#ifndef _VERSION_0_1
#define _VERSION_0_1
#endif
#else
#error "VERSION INVALID!!!"
#endif

#define VERSION_STR "0.0"
// const char *g_version="0.0";



#endif // ! _VERSION_H_

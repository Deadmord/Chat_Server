#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MESSAGESAVER_SERVICE_LIB)
#  define MESSAGESAVER_SERVICE_EXPORT Q_DECL_EXPORT
# else
#  define MESSAGESAVER_SERVICE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MESSAGESAVER_SERVICE_EXPORT
#endif


#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
#  define CHAT_SERVER_LOCALSTORAGE_SERVICE_EXPORT Q_DECL_EXPORT
#else
# define CHAT_SERVER_LOCALSTORAGE_SERVICE_EXPORT
#endif
#ifndef DEFINES_H
#define DEFINES_H

#include <util/include/Logger.h>
#include <core/include/SubsystemList.h>
#include <core/include/Application.h>
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

extern unique_ptr<Core::Application> pApplication;
extern Logger LOGGER;
#endif // DEFINES_H

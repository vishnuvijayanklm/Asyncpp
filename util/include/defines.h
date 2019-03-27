#ifndef DEFINES_H
#define DEFINES_H

#include <Logger.h>
#include <SubsystemList.h>
#include <Application.h>
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

extern unique_ptr<Core::Application> pApplication;
extern Logger LOGGER;
#endif // DEFINES_H

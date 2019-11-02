#ifndef DEFINES_H
#define DEFINES_H

#include <util/include/Logger.h>

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define IPC_ERROR -1

#define make_ptr(X) typedef\
	std::shared_ptr<X>\
	X##Ptr

extern Logger LOGGER;
#endif // DEFINES_H

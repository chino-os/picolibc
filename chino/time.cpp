// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <chino/os/ke_services.h>
#include <chino/os/processapi.h>
#include <unistd.h>
#include <time.h>

using namespace chino;
using namespace chino::os;

int
timespec_get(struct timespec *ts, int base)
{
    if (base == TIME_UTC && clock_gettime(CLOCK_REALTIME, ts) == 0)
        return base;
    return 0;
}

int
clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    return ke_services().clock_gettime(clock_id, tp);
}

unsigned
sleep(unsigned int __seconds)
{
    timespec rq{ .tv_sec = __seconds, .tv_nsec = 0 };
    auto r = nanosleep(&rq, nullptr);
    return r ? r : 0;
}

int
nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    return ke_services().nanosleep(rqtp, rmtp);
}

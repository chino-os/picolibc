// Copyright (c) SunnyCase. All rights reserved.
// Licensed under the Apache license. See LICENSE file in the project root for full license information.
#include <chino/os/kernel/ke_services.h>
#include <chino/os/processapi.h>

using namespace chino;
using namespace chino::os;
using namespace chino::os::kernel;

result<void>
os::atomic_wait(std::atomic<uint32_t> &atomic, uint32_t old,
                std::optional<std::chrono::milliseconds> timeout) noexcept
{
    if (atomic.load(std::memory_order_acquire) != old) {
        // 1. Fast path
        return ok();
    } else {
        // 2. Slow path
        return kernel::ke_services().atomic_wait(atomic, old, timeout);
    }
}

void
os::atomic_notify_one(std::atomic<uint32_t> &atomic) noexcept
{
    kernel::ke_services().atomic_notify_one(atomic);
}

void
os::atomic_notify_all(std::atomic<uint32_t> &atomic) noexcept
{
    kernel::ke_services().atomic_notify_all(atomic);
}

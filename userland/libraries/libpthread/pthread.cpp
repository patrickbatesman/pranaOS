/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// inlcudes
#include <basea/Assertions.h>
#include <base/Atomic.h>
#include <base/Debug.h>
#include <base/Format.h>
#include <base/StdLibExtras.h>
#include <kernel/api/Syscall.h>
#include <libsystem/syscall.h>
#include <bits/pthread_integration.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <pranaos.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <syscall.h>
#include <time.h>
#include <unistd.h>

namespace {
using PthreadAttrImpl = Syscall::SC_create_thread_params;

} 

static constexpr size_t required_stack_alignment = 4 * MiB;
static constexpr size_t highest_reasonable_guard_size = 32 * PAGE_SIZE;
static constexpr size_t highest_reasonable_stack_size = 8 * MiB; 

__thread void* s_stack_location;
__thread size_t s_stack_size;

#define __RETURN_PTHREAD_ERROR(rc) \
    return ((rc) < 0 ? -(rc) : 0)

extern "C" {

static void* pthread_create_helper(void* (*routine)(void*), void* argument, void* stack_location, size_t stack_size)
{
    s_stack_location = stack_location;
    s_stack_size = stack_size;
    void* ret_val = routine(argument);
    pthread_exit(ret_val);
}

static int create_thread(pthread_t* thread, void* (*entry)(void*), void* argument, PthreadAttrImpl* thread_params)
{
    void** stack = (void**)((uintptr_t)thread_params->stack_location + thread_params->stack_size);

    auto push_on_stack = [&](void* data) {
        stack--;
        *stack = data;
        thread_params->stack_size -= sizeof(void*);
    };

    while (((uintptr_t)stack - 16) % 16 != 0)
        push_on_stack(nullptr);

#if ARCH(I386)
    push_on_stack((void*)(uintptr_t)thread_params->stack_size);
    push_on_stack(thread_params->stack_location);
    push_on_stack(argument);
    push_on_stack((void*)entry);
#else
    thread_params->rdi = (FlatPtr)entry;
    thread_params->rsi = (FlatPtr)argument;
    thread_params->rdx = (FlatPtr)thread_params->stack_location;
    thread_params->rcx = thread_params->stack_size;
#endif
    VERIFY((uintptr_t)stack % 16 == 0);

    push_on_stack(nullptr);

    int rc = syscall(SC_create_thread, pthread_create_helper, thread_params);
    if (rc >= 0)
        *thread = rc;
    __RETURN_PTHREAD_ERROR(rc);
}

[[noreturn]] static void exit_thread(void* code, void* stack_location, size_t stack_size)
{
    __pthread_key_destroy_for_current_thread();
    syscall(SC_exit_thread, code, stack_location, stack_size);
    VERIFY_NOT_REACHED();
}

int pthread_self()
{
    return __pthread_self();
}

int pthread_create(pthread_t* thread, pthread_attr_t* attributes, void* (*start_routine)(void*), void* argument_to_start_routine)
{
    if (!thread)
        return -EINVAL;

    PthreadAttrImpl default_attributes {};
    PthreadAttrImpl** arg_attributes = reinterpret_cast<PthreadAttrImpl**>(attributes);

    PthreadAttrImpl* used_attributes = arg_attributes ? *arg_attributes : &default_attributes;

    if (!used_attributes->stack_location) {

        if (0 != (used_attributes->stack_size % required_stack_alignment))
            used_attributes->stack_size += required_stack_alignment - (used_attributes->stack_size % required_stack_alignment);

        used_attributes->stack_location = mmap_with_name(nullptr, used_attributes->stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, 0, 0, "Thread stack");
        if (!used_attributes->stack_location)
            return -1;
    }

    dbgln_if(PTHREAD_DEBUG, "pthread_create: Creating thread with attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        used_attributes,
        (PTHREAD_CREATE_JOINABLE == used_attributes->detach_state) ? "joinable" : "detached",
        used_attributes->schedule_priority,
        used_attributes->guard_page_size,
        used_attributes->stack_size,
        used_attributes->stack_location);

    return create_thread(thread, start_routine, argument_to_start_routine, used_attributes);
}

void pthread_exit(void* value_ptr)
{
    exit_thread(value_ptr, s_stack_location, s_stack_size);
}

void pthread_cleanup_push([[maybe_unused]] void (*routine)(void*), [[maybe_unused]] void* arg)
{
    TODO();
}

void pthread_cleanup_pop([[maybe_unused]] int execute)
{
    TODO();
}

int pthread_join(pthread_t thread, void** exit_value_ptr)
{
    int rc = syscall(SC_join_thread, thread, exit_value_ptr);
    __RETURN_PTHREAD_ERROR(rc);
}

int pthread_kill(pthread_t thread, int sig)
{
    int rc = syscall(SC_kill_thread, thread, sig);
    __RETURN_PTHREAD_ERROR(rc);
}

int pthread_detach(pthread_t thread)
{
    int rc = syscall(SC_detach_thread, thread);
    __RETURN_PTHREAD_ERROR(rc);
}

int pthread_sigmask(int how, const sigset_t* set, sigset_t* old_set)
{
    if (sigprocmask(how, set, old_set))
        return errno;
    return 0;
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attributes)
{
    return __pthread_mutex_init(mutex, attributes);
}

int pthread_mutex_destroy(pthread_mutex_t*)
{
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex)
{
    return __pthread_mutex_lock(mutex);
}

int pthread_mutex_trylock(pthread_mutex_t* mutex)
{
    return __pthread_mutex_trylock(mutex);
}

int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    return __pthread_mutex_unlock(mutex);
}

int pthread_mutexattr_init(pthread_mutexattr_t* attr)
{
    attr->type = PTHREAD_MUTEX_NORMAL;
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t*)
{
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type)
{
    if (!attr)
        return EINVAL;
    if (type != PTHREAD_MUTEX_NORMAL && type != PTHREAD_MUTEX_RECURSIVE)
        return EINVAL;
    attr->type = type;
    return 0;
}

int pthread_mutexattr_gettype(pthread_mutexattr_t* attr, int* type)
{
    *type = attr->type;
    return 0;
}

int pthread_attr_init(pthread_attr_t* attributes)
{
    auto* impl = new PthreadAttrImpl {};
    *attributes = impl;

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_init: New thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        impl,
        (PTHREAD_CREATE_JOINABLE == impl->detach_state) ? "joinable" : "detached",
        impl->schedule_priority,
        impl->guard_page_size,
        impl->stack_size,
        impl->stack_location);

    return 0;
}

int pthread_attr_destroy(pthread_attr_t* attributes)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));
    delete attributes_impl;
    return 0;
}

int pthread_attr_getdetachstate(const pthread_attr_t* attributes, int* p_detach_state)
{
    auto* attributes_impl = *(reinterpret_cast<const PthreadAttrImpl* const*>(attributes));

    if (!attributes_impl || !p_detach_state)
        return EINVAL;

    *p_detach_state = attributes_impl->detach_state;
    return 0;
}

int pthread_attr_setdetachstate(pthread_attr_t* attributes, int detach_state)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));

    if (!attributes_impl)
        return EINVAL;

    if (detach_state != PTHREAD_CREATE_JOINABLE && detach_state != PTHREAD_CREATE_DETACHED)
        return EINVAL;

    attributes_impl->detach_state = detach_state;

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_setdetachstate: Thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        attributes_impl,
        (PTHREAD_CREATE_JOINABLE == attributes_impl->detach_state) ? "joinable" : "detached",
        attributes_impl->schedule_priority,
        attributes_impl->guard_page_size,
        attributes_impl->stack_size,
        attributes_impl->stack_location);

    return 0;
}

int pthread_attr_getguardsize(const pthread_attr_t* attributes, size_t* p_guard_size)
{
    auto* attributes_impl = *(reinterpret_cast<const PthreadAttrImpl* const*>(attributes));

    if (!attributes_impl || !p_guard_size)
        return EINVAL;

    *p_guard_size = attributes_impl->reported_guard_page_size;
    return 0;
}

int pthread_attr_setguardsize(pthread_attr_t* attributes, size_t guard_size)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));

    if (!attributes_impl)
        return EINVAL;

    size_t actual_guard_size = guard_size;

    if (0 != (guard_size % PAGE_SIZE))
        actual_guard_size += PAGE_SIZE - (guard_size % PAGE_SIZE);


    if (actual_guard_size > highest_reasonable_guard_size) {
        return EINVAL;
    }

    attributes_impl->guard_page_size = actual_guard_size;
    attributes_impl->reported_guard_page_size = guard_size; 

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_setguardsize: Thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        attributes_impl,
        (PTHREAD_CREATE_JOINABLE == attributes_impl->detach_state) ? "joinable" : "detached",
        attributes_impl->schedule_priority,
        attributes_impl->guard_page_size,
        attributes_impl->stack_size,
        attributes_impl->stack_location);

    return 0;
}

int pthread_attr_getschedparam(const pthread_attr_t* attributes, struct sched_param* p_sched_param)
{
    auto* attributes_impl = *(reinterpret_cast<const PthreadAttrImpl* const*>(attributes));

    if (!attributes_impl || !p_sched_param)
        return EINVAL;

    p_sched_param->sched_priority = attributes_impl->schedule_priority;
    return 0;
}

int pthread_attr_setschedparam(pthread_attr_t* attributes, const struct sched_param* p_sched_param)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));
    if (!attributes_impl || !p_sched_param)
        return EINVAL;

    if (p_sched_param->sched_priority < THREAD_PRIORITY_MIN || p_sched_param->sched_priority > THREAD_PRIORITY_MAX)
        return ENOTSUP;

    attributes_impl->schedule_priority = p_sched_param->sched_priority;

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_setschedparam: Thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        attributes_impl,
        (PTHREAD_CREATE_JOINABLE == attributes_impl->detach_state) ? "joinable" : "detached",
        attributes_impl->schedule_priority,
        attributes_impl->guard_page_size,
        attributes_impl->stack_size,
        attributes_impl->stack_location);

    return 0;
}

int pthread_attr_getstack(const pthread_attr_t* attributes, void** p_stack_ptr, size_t* p_stack_size)
{
    auto* attributes_impl = *(reinterpret_cast<const PthreadAttrImpl* const*>(attributes));

    if (!attributes_impl || !p_stack_ptr || !p_stack_size)
        return EINVAL;

    *p_stack_ptr = attributes_impl->stack_location;
    *p_stack_size = attributes_impl->stack_size;

    return 0;
}

int pthread_attr_setstack(pthread_attr_t* attributes, void* p_stack, size_t stack_size)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));

    if (!attributes_impl || !p_stack)
        return EINVAL;

    if (0 != (stack_size % required_stack_alignment))
        return EINVAL;


    attributes_impl->stack_size = stack_size;
    attributes_impl->stack_location = p_stack;

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_setstack: Thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        attributes_impl,
        (PTHREAD_CREATE_JOINABLE == attributes_impl->detach_state) ? "joinable" : "detached",
        attributes_impl->schedule_priority,
        attributes_impl->guard_page_size,
        attributes_impl->stack_size,
        attributes_impl->stack_location);

    return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t* attributes, size_t* p_stack_size)
{
    auto* attributes_impl = *(reinterpret_cast<const PthreadAttrImpl* const*>(attributes));

    if (!attributes_impl || !p_stack_size)
        return EINVAL;

    *p_stack_size = attributes_impl->stack_size;
    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t* attributes, size_t stack_size)
{
    auto* attributes_impl = *(reinterpret_cast<PthreadAttrImpl**>(attributes));

    if (!attributes_impl)
        return EINVAL;

    if ((stack_size < PTHREAD_STACK_MIN) || stack_size > highest_reasonable_stack_size)
        return EINVAL;

    attributes_impl->stack_size = stack_size;

    dbgln_if(PTHREAD_DEBUG, "pthread_attr_setstacksize: Thread attributes at {}, detach state {}, priority {}, guard page size {}, stack size {}, stack location {}",
        attributes_impl,
        (PTHREAD_CREATE_JOINABLE == attributes_impl->detach_state) ? "joinable" : "detached",
        attributes_impl->schedule_priority,
        attributes_impl->guard_page_size,
        attributes_impl->stack_size,
        attributes_impl->stack_location);

    return 0;
}

int pthread_attr_getscope([[maybe_unused]] const pthread_attr_t* attributes, [[maybe_unused]] int* contention_scope)
{
    return 0;
}

int pthread_attr_setscope([[maybe_unused]] pthread_attr_t* attributes, [[maybe_unused]] int contention_scope)
{
    return 0;
}

int pthread_getschedparam([[maybe_unused]] pthread_t thread, [[maybe_unused]] int* policy, [[maybe_unused]] struct sched_param* param)
{
    return 0;
}

int pthread_setschedparam([[maybe_unused]] pthread_t thread, [[maybe_unused]] int policy, [[maybe_unused]] const struct sched_param* param)
{
    return 0;
}

int pthread_cancel(pthread_t)
{
    TODO();
}

int pthread_key_create(pthread_key_t* key, KeyDestructor destructor)
{
    return __pthread_key_create(key, destructor);
}

int pthread_key_delete(pthread_key_t key)
{
    return __pthread_key_delete(key);
}

void* pthread_getspecific(pthread_key_t key)
{
    return __pthread_getspecific(key);
}

int pthread_setspecific(pthread_key_t key, const void* value)
{
    return __pthread_setspecific(key, value);
}

int pthread_setname_np(pthread_t thread, const char* name)
{
    if (!name)
        return EFAULT;
    int rc = syscall(SC_set_thread_name, thread, name, strlen(name));
    __RETURN_PTHREAD_ERROR(rc);
}

int pthread_getname_np(pthread_t thread, char* buffer, size_t buffer_size)
{
    int rc = syscall(SC_get_thread_name, thread, buffer, buffer_size);
    __RETURN_PTHREAD_ERROR(rc);
}

int pthread_setcancelstate(int state, int* oldstate)
{
    if (oldstate)
        *oldstate = PTHREAD_CANCEL_DISABLE;
    dbgln("FIXME: Implement pthread_setcancelstate({}, ...)", state);
    if (state != PTHREAD_CANCEL_DISABLE)
        return EINVAL;
    return 0;
}

int pthread_setcanceltype(int type, int* oldtype)
{
    if (oldtype)
        *oldtype = PTHREAD_CANCEL_DEFERRED;
    dbgln("FIXME: Implement pthread_setcanceltype({}, ...)", type);
    if (type != PTHREAD_CANCEL_DEFERRED)
        return EINVAL;
    return 0;
}

constexpr static pid_t spinlock_unlock_sentinel = 0;
int pthread_spin_destroy(pthread_spinlock_t* lock)
{
    auto current = Base::atomic_load(&lock->m_lock);

    if (current != spinlock_unlock_sentinel)
        return EBUSY;

    return 0;
}

int pthread_spin_init(pthread_spinlock_t* lock, [[maybe_unused]] int shared)
{
    lock->m_lock = spinlock_unlock_sentinel;
    return 0;
}

int pthread_spin_lock(pthread_spinlock_t* lock)
{
    const auto desired = gettid();
    while (true) {
        auto current = Base::atomic_load(&lock->m_lock);

        if (current == desired)
            return EDEADLK;

        if (Base::atomic_compare_exchange_strong(&lock->m_lock, current, desired, Base::MemoryOrder::memory_order_acquire))
            break;
    }

    return 0;
}

int pthread_spin_trylock(pthread_spinlock_t* lock)
{

    auto current = spinlock_unlock_sentinel;
    auto desired = gettid();

    if (Base::atomic_compare_exchange_strong(&lock->m_lock, current, desired, Base::MemoryOrder::memory_order_acquire)) {
        return 0;
    } else {
        return EBUSY;
    }
}

int pthread_spin_unlock(pthread_spinlock_t* lock)
{
    auto current = Base::atomic_load(&lock->m_lock);

    if (gettid() != current)
        return EPERM;

    Base::atomic_store(&lock->m_lock, spinlock_unlock_sentinel);
    return 0;
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
    return t1 == t2;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rl)
{
    if (!rl)
        return 0;
    return 0;
}

constexpr static u32 reader_wake_mask = 1 << 30;
constexpr static u32 writer_wake_mask = 1 << 31;
constexpr static u32 writer_locked_mask = 1 << 17;
constexpr static u32 writer_intent_mask = 1 << 16;
int pthread_rwlock_init(pthread_rwlock_t* __restrict lockp, const pthread_rwlockattr_t* __restrict attr)
{
    (void)attr;

    *lockp = 0;
    return 0;
}

static int rwlock_rdlock_maybe_timed(u32* lockp, const struct timespec* timeout = nullptr, bool only_once = false, int value_if_timeout = -1, int value_if_okay = -2)
{
    auto current = Base::atomic_load(lockp);
    for (; !only_once;) {        
    
        if (!(current & writer_locked_mask)) {
            auto count = (u16)current;
            if (!(current & writer_intent_mask) || count > 1) {
                ++count;
                auto desired = (current << 16) | count;
                auto did_exchange = Base::atomic_compare_exchange_strong(lockp, current, desired, Base::MemoryOrder::memory_order_acquire);
                if (!did_exchange)
                    continue; 
                return value_if_okay;
            }
        }

        if (!(current & reader_wake_mask)) {
            auto desired = current | reader_wake_mask;
            auto did_exchange = Base::atomic_compare_exchange_strong(lockp, current, desired, Base::MemoryOrder::memory_order_acquire);
            if (!did_exchange)
                continue; 

            current = desired;
        }

        auto rc = futex(lockp, FUTEX_WAIT_BITSET, current, timeout, nullptr, reader_wake_mask);
        if (rc < 0 && errno == ETIMEDOUT && timeout) {
            return value_if_timeout;
        }
        if (rc < 0 && errno != EAGAIN) {

            return errno;
        }
        errno = 0;

        current = Base::atomic_load(lockp);
    }
    return value_if_timeout;
}

static int rwlock_wrlock_maybe_timed(pthread_rwlock_t* lockval_p, const struct timespec* timeout = nullptr, bool only_once = false, int value_if_timeout = -1, int value_if_okay = -2)
{
    u32* lockp = reinterpret_cast<u32*>(lockval_p);
    auto current = Base::atomic_load(lockp);
    for (; !only_once;) {

        if (!(current & writer_locked_mask) && ((u16)current) == 0) {
            if (!(current & writer_intent_mask)) {
                auto desired = current | writer_locked_mask | writer_intent_mask;
                auto did_exchange = Base::atomic_compare_exchange_strong(lockp, current, desired, Base::MemoryOrder::memory_order_acquire);
                if (!did_exchange)
                    continue;

                Base::atomic_store(reinterpret_cast<i32*>(lockval_p) + 1, pthread_self());
                return value_if_okay;
            }
        }


        if (!(current & writer_wake_mask)) {
            auto desired = current | writer_wake_mask | writer_intent_mask;
            auto did_exchange = Base::atomic_compare_exchange_strong(lockp, current, desired, Base::MemoryOrder::memory_order_acquire);
            if (!did_exchange)
                continue; 

            current = desired;
        }

        
        auto rc = futex(lockp, FUTEX_WAIT_BITSET, current, timeout, nullptr, writer_wake_mask);
        if (rc < 0 && errno == ETIMEDOUT && timeout) {
            return value_if_timeout;
        }
        if (rc < 0 && errno != EAGAIN) {

            return errno;
        }
        errno = 0;

        current = Base::atomic_load(lockp);
    }

    return value_if_timeout;
}

int pthread_rwlock_rdlock(pthread_rwlock_t* lockp)
{
    if (!lockp)
        return EINVAL;

    return rwlock_rdlock_maybe_timed(reinterpret_cast<u32*>(lockp), nullptr, false, 0, 0);
}
int pthread_rwlock_timedrdlock(pthread_rwlock_t* __restrict lockp, const struct timespec* __restrict timespec)
{
    if (!lockp)
        return EINVAL;

    auto rc = rwlock_rdlock_maybe_timed(reinterpret_cast<u32*>(lockp), timespec);
    if (rc == -1) 
        return 0;
    if (rc == -2) 
        return 1;
    return rc;
}
int pthread_rwlock_timedwrlock(pthread_rwlock_t* __restrict lockp, const struct timespec* __restrict timespec)
{
    if (!lockp)
        return EINVAL;

    auto rc = rwlock_wrlock_maybe_timed(lockp, timespec);
    if (rc == -1) 
        return 0;
    if (rc == -2) 
        return 1;
    return rc;
}
int pthread_rwlock_tryrdlock(pthread_rwlock_t* lockp)
{
    if (!lockp)
        return EINVAL;

    return rwlock_rdlock_maybe_timed(reinterpret_cast<u32*>(lockp), nullptr, true, EBUSY, 0);
}
int pthread_rwlock_trywrlock(pthread_rwlock_t* lockp)
{
    if (!lockp)
        return EINVAL;

    return rwlock_wrlock_maybe_timed(lockp, nullptr, true, EBUSY, 0);
}
int pthread_rwlock_unlock(pthread_rwlock_t* lockval_p)
{
    if (!lockval_p)
        return EINVAL;

    auto lockp = reinterpret_cast<u32*>(lockval_p);
    auto current = Base::atomic_load(lockp, Base::MemoryOrder::memory_order_relaxed);
    if (current & writer_locked_mask) {

        auto owner_id = Base::atomic_load(reinterpret_cast<i32*>(lockval_p) + 1);
        auto my_id = pthread_self();
        if (owner_id != my_id)
            return EINVAL; 

        auto desired = current & ~(writer_locked_mask | writer_intent_mask);
        Base::atomic_store(lockp, desired, Base::MemoryOrder::memory_order_release);

        auto rc = futex(lockp, FUTEX_WAKE_BITSET, current, nullptr, nullptr, (current & writer_wake_mask) | reader_wake_mask);
        if (rc < 0)
            return errno;
        return 0;
    }

    for (;;) {
        auto count = (u16)current;
        if (!count) {

            return EINVAL;
        }
        --count;
        auto desired = (current << 16) | count;
        auto did_exchange = Base::atomic_compare_exchange_strong(lockp, current, desired, Base::MemoryOrder::memory_order_release);
        if (!did_exchange)
            continue; 
    }

    return 0;
}
int pthread_rwlock_wrlock(pthread_rwlock_t* lockp)
{
    if (!lockp)
        return EINVAL;

    return rwlock_wrlock_maybe_timed(lockp, nullptr, false, 0, 0);
}
int pthread_rwlockattr_destroy(pthread_rwlockattr_t*)
{
    return 0;
}
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t* __restrict, int* __restrict)
{
    VERIFY_NOT_REACHED();
}
int pthread_rwlockattr_init(pthread_rwlockattr_t*)
{
    VERIFY_NOT_REACHED();
}
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t*, int)
{
    VERIFY_NOT_REACHED();
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    if (prepare)
        __pthread_fork_atfork_register_prepare(prepare);
    if (parent)
        __pthread_fork_atfork_register_parent(parent);
    if (child)
        __pthread_fork_atfork_register_child(child);
    return 0;
}

} 

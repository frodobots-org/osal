/*
 * Copyright 2023 frodobots.ai. All rights reserved.
 */

#include "osal.h"
#include <stdarg.h>

int os_thread_create(os_thread_t *thread, os_thread_func_t func, void *arg) {
#if defined(__RTTHREAD__)
    thread->thread = rt_thread_create("thread_name", func, arg, 1024, 10, 10);
    return thread->thread ? rt_thread_startup(thread->thread) : -1;
#elif defined(__linux__)
    return pthread_create(&thread->thread, NULL, func, arg);
#endif
}

int os_thread_destroy(os_thread_t *thread) {
#if defined(__RTTHREAD__)
    return rt_thread_delete(thread->thread);
#elif defined(__linux__)
    return pthread_cancel(thread->thread);
#endif
}

int os_mutex_init(os_mutex_t *mutex) {
#if defined(__RTTHREAD__)
    mutex->mutex = rt_mutex_create("mutex_name", RT_IPC_FLAG_PRIO);
    return mutex->mutex ? 0 : -1;
#elif defined(__linux__)
    return pthread_mutex_init(&mutex->mutex, NULL);
#endif
}

int os_mutex_destroy(os_mutex_t *mutex) {
#if defined(__RTTHREAD__)
    return rt_mutex_delete(mutex->mutex);
#elif defined(__linux__)
    return pthread_mutex_destroy(&mutex->mutex);
#endif
}

int os_mutex_lock(os_mutex_t *mutex) {
#if defined(__RTTHREAD__)
    return rt_mutex_take(mutex->mutex, RT_WAITING_FOREVER);
#elif defined(__linux__)
    return pthread_mutex_lock(&mutex->mutex);
#endif
}

int os_mutex_unlock(os_mutex_t *mutex) {
#if defined(__RTTHREAD__)
    return rt_mutex_release(mutex->mutex);
#elif defined(__linux__)
    return pthread_mutex_unlock(&mutex->mutex);
#endif
}

void* os_malloc(size_t size) {
#if defined(__RTTHREAD__) || defined(__linux__)
    return malloc(size);
#endif
}

void os_free(void* ptr) {
#if defined(__RTTHREAD__) || defined(__linux__)
    free(ptr);
#endif
}

void os_msleep(unsigned int milliseconds) {
#if defined(__RTTHREAD__)
    rt_thread_mdelay(milliseconds);
#elif defined(__linux__)
    usleep(milliseconds * 1000);
#endif
}

int os_open(const char *pathname, int flags, ...) {
    int fd;
    va_list args;
    va_start(args, flags);
#if defined(__RTTHREAD__)
    fd = open(pathname, flags, va_arg(args, int));
#elif defined(__linux__)
    fd = open(pathname, flags, va_arg(args, mode_t));
#endif
    va_end(args);
    return fd;
}

int os_close(int fd) {
#if defined(__RTTHREAD__) || defined(__linux__)
    return close(fd);
#endif
}

ssize_t os_read(int fd, void *buf, size_t count) {
#if defined(__RTTHREAD__) || defined(__linux__)
    return read(fd, buf, count);
#endif
}

ssize_t os_write(int fd, const void *buf, size_t count) {
#if defined(__RTTHREAD__) || defined(__linux__)
    return write(fd, buf, count);
#endif
}

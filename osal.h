/*
 * Copyright 2023 frodobots.ai. All rights reserved.
 */

#ifndef OS_ABSTRACTION_H
#define OS_ABSTRACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__RTTHREAD__)
    #include <rtthread.h>
    #include <dfs_posix.h>
#elif defined(__linux__)
    #include <pthread.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #error "Unsupported platform"
#endif

typedef struct {
#if defined(__RTTHREAD__)
    rt_thread_t thread;
#elif defined(__linux__)
    pthread_t thread;
#endif
} os_thread_t;

typedef struct {
#if defined(__RTTHREAD__)
    rt_mutex_t mutex;
#elif defined(__linux__)
    pthread_mutex_t mutex;
#endif
} os_mutex_t;

typedef void* (*os_thread_func_t)(void* arg);

int os_thread_create(os_thread_t *thread, os_thread_func_t func, void *arg);

int os_thread_destroy(os_thread_t *thread);

int os_mutex_init(os_mutex_t *mutex);

int os_mutex_destroy(os_mutex_t *mutex);

int os_mutex_lock(os_mutex_t *mutex);

void* os_malloc(size_t size);
void os_free(void* ptr);

void os_msleep(unsigned int milliseconds);

int os_open(const char *pathname, int flags, ...);
int os_close(int fd);
ssize_t os_read(int fd, void *buf, size_t count);
ssize_t os_write(int fd, const void *buf, size_t count);

#ifdef __cplusplus
}
#endif

#endif // OS_ABSTRACTION_H

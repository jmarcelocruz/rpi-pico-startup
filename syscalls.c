/*
 * Copyright 2024 José Marcelo Marques da Cruz
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <errno.h>
#undef errno
extern int errno;
#include <sys/stat.h>

__attribute__((weak)) char stdin_getc(void) { return 0; }
__attribute__((weak)) void stdout_putc(char c) { ; }

int _close(int fd) {
    return -1;
}

void _exit(int code) {
    ;
}

int _fstat(int fd, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid(void) {
    return 1;
}

int _isatty(int fd) {
    return 1;
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int _lseek(int file, int offset, int whence) {
    return 0;
}

int _read(int fd, char *buf, int len) {
    for (int i = 0; i < len; i++) {
        buf[i] = stdin_getc();
    }

    return len;
}

char *_sbrk(int incr) {
    extern char _sheap;
    static char *eheap = &_sheap;
    char *prev_eheap = eheap;
    eheap += incr; /* TODO: may lead to heap and stack collisions */

    return prev_eheap;
}

int _write(int file, char *buf, int len) {
    for (int i = 0; i < len; i++) {
        stdout_putc(buf[i]);
    }

    return len;
}

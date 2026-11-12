```c run id=run-bytes-as-a-function
#define _DARWIN_C_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#if defined(__APPLE__)
#include <libkern/OSCacheControl.h>
#include <pthread.h>
#endif

/* The machine code of a function that returns 42, for two processors. */
#if defined(__aarch64__)
static const unsigned char code[] = {
    0x40, 0x05, 0x80, 0xd2, /* movz x0, #42 */
    0xc0, 0x03, 0x5f, 0xd6, /* ret */
};
#elif defined(__x86_64__)
static const unsigned char code[] = {
    0xb8, 0x2a, 0x00, 0x00, 0x00, /* mov eax, 42 */
    0xc3,                         /* ret */
};
#else
#error "no machine code for this processor"
#endif

int main(void) {
    int flags = MAP_PRIVATE | MAP_ANON;
#if defined(__APPLE__)
    flags |= MAP_JIT; /* macOS allows writable and executable memory only with MAP_JIT */
#endif
    void *mem = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, flags, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
#if defined(__APPLE__)
    pthread_jit_write_protect_np(0); /* make the page writable for this thread */
#endif
    memcpy(mem, code, sizeof code);
#if defined(__APPLE__)
    pthread_jit_write_protect_np(1); /* and executable again */
    sys_icache_invalidate(mem, sizeof code);
#else
    __builtin___clear_cache((char *)mem, (char *)mem + sizeof code);
#endif
    int (*f)(void) = (int (*)(void))mem; /* treat the bytes as a function */
    printf("f() = %d\n", f());
    munmap(mem, 4096);
    return 0;
}
```

```output for=run-bytes-as-a-function
f() = 42
```

- Verdict: native. The program holds the machine code of a function that returns 42, for ARM64 and for x86-64, and copies it into a page of memory that can run code. On macOS the page needs `MAP_JIT` and a switch from writable to executable, because of the W^X rule, and on ARM the instruction cache must be cleared. Casting the address to a function pointer and calling it runs the bytes. ISO C does not allow this cast; POSIX systems support it.

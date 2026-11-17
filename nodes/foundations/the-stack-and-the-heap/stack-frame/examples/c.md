```c run id=frames
#include <stdio.h>

static const int *addrs[3]; /* the address of here in each call */
static int depth = 0;

static void count(int n) {
    int here = n * 10; /* a local: it lives in the frame of this call */
    addrs[depth++] = &here;
    printf("enter n=%d here=%d\n", n, here);
    if (n > 1) {
        count(n - 1);
    } else {
        int distinct = addrs[0] != addrs[1] && addrs[1] != addrs[2] && addrs[0] != addrs[2];
        printf("%d frames, %d %s addresses\n", depth, depth, distinct ? "different" : "shared");
    }
    printf("leave n=%d here=%d\n", n, here);
}

int main(void) {
    count(3);
    return 0;
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
3 frames, 3 different addresses
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has a frame with its own `n` and `here`. The platform ABI sets the layout of the frame, and the C standard does not describe it.
- The saved addresses are only compared while all three calls are active. After `count` returns, a pointer to its `here` points into a dead frame.

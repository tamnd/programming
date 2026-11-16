```c run id=function-call-c
#include <stdio.h>

static int rectangle_area(int width, int height) {
    printf("computing %d by %d\n", width, height);
    return width * height;
}

int main(void) {
    int area = rectangle_area(3, 4);
    printf("area %d\n", area);
    return 0;
}
```

```output for=function-call-c
computing 3 by 4
area 12
```

- Verdict: native. A call is an expression with the function, then the arguments in parentheses. Its value is the value that the function returns.

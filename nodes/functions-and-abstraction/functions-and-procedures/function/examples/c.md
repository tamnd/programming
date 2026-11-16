```c run id=function-c
#include <stdio.h>

int rectangle_area(int width, int height) {
    return width * height;
}

void print_greeting(const char *name) {
    printf("Hello, %s!\n", name);
}

int main(void) {
    printf("%d\n", rectangle_area(3, 4));
    print_greeting("Ada");
    return 0;
}
```

```output for=function-c
12
Hello, Ada!
```

- Verdict: native. A C function has a result type before its name. `print_greeting` prints the text, because a C function cannot easily return a new string without memory management.

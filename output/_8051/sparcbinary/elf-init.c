// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    union { __size8 x; struct { int newInt; float newFloat; } m; } r5; 		// r5
    __size8 r7; 		// r7

    r5.x = 1;
    a = cc(r7);
    return a;
}


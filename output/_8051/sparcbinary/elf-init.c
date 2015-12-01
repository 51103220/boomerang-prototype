// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    __size8 p0; 		// r13

    a = 20;
    if (*p0 == 1) {
        a = 40;
    }
    return a + 30;
}


// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8

    a = 100 - *(__size8 *)10;
    return a;
}


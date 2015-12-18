// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    char a; 		// r8
    unsigned char r0; 		// r0

    *(__size8*)(r0) = 200;
    a = 100 ^ *(__size8 *)(r0);
    return a;
}


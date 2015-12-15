// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    __size8 *p0; 		// r13

    *(__size8*)p0 = 29;
    a = *p0;
    *(__size8*)p0 = 100;
    return a;
}


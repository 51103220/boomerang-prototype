// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8

    *(__size8*)484 = 56;
    a = *484;
    return a;
}


// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    char a; 		// r8

    *(__size8*)20 = 200;
    a = 100 ^ *(__size8 *)20;
    return a;
}


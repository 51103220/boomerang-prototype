// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8

    a = *(__size8 *)599;
    *(__size8*)599 = 100;
    return a;
}


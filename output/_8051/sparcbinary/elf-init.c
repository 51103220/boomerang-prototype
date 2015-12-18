// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    *(__size8*)20 = *(__size8 *)20 - 1;
    return 100;
}


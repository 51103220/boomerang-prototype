// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size32 *sp; 		// r88

    *(__size32*)(sp + 1) = *7;
    *(__size32*)69 = *sp;
    return 50;
}


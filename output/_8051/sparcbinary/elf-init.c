// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    unsigned short dptr; 		// r11

    a = 56;
    dptr = 484;
    *(__size8*)986 = 1;
    if (*986 == 1) {
        *(__size8*)484 = 56;
        if (*1015 == 0) {
            dptr = 100;
            *(char*)9 = *9 | 5;
        }
        *(__size8*)986 = 0;
        a = *(dptr);
    }
    return a;
}


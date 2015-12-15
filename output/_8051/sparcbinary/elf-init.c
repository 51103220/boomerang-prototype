// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    union { __size8 x; struct { __size8 bit1:1; __size8 bit2:1; __size8 bit3:1; __size8 bit4:1; __size8 bit5:1; __size8 bit6:1; __size8 bit7:1; __size8 bit8:1; } m; } c; 		// r10

    c.x.m.bit7 = 1;
    a += *c.x + *20;
    return a;
}


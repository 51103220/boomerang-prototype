// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    union { __size8 x; struct { __size8 bit1:1; __size8 bit2:1; __size8 bit3:1; __size8 bit4:1; __size8 bit5:1; __size8 bit6:1; __size8 bit7:1; __size8 bit8:1; } m; } r0; 		// r0
    union { __size8 x; struct { __size8 bit1:1; __size8 bit2:1; __size8 bit3:1; __size8 bit4:1; __size8 bit5:1; __size8 bit6:1; __size8 bit7:1; __size8 bit8:1; } m; } r1; 		// r1

    r0.x.m.bit5 = 1;
    r1.x.m.bit3 = 0;
    if (r0.x.m.bit5 == 1) {
        a = 150;
        if (r0.x.m.bit5 == 0) {
            a = 200;
        }
    }
    if (r1.x.m.bit3 == 0) {
        a = 200;
    }
    return a;
}


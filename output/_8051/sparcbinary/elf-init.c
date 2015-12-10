// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    union { __size8 x; struct { __size8 bit1; __size8 bit2; __size8 bit3; __size8 bit4; __size8 bit5; __size8 bit6; __size8 bit7; __size8 bit8; } m; } a; 		// r8
    __size8 r0; 		// r0

    a.x.m.bit7 = 0;
    *(__size8*)(r1) = r0 + 1;
    return;
}


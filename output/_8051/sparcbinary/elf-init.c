// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8
    union { __size8 x; struct { __size8 bit1; __size8 bit2; __size8 bit3; __size8 bit4; __size8 bit5; __size8 bit6; __size8 bit7; __size8 bit8; } m; } p1; 		// r14
    __size8 r7; 		// r7

    a = *(r7 + 1);
    p1.x.m.bit8 = 1;
    *(__size8*)p0 = *p1.x;
    return a;
}


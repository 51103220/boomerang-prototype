// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    union { __size8 x; struct { __size8 bit1:1; __size8 bit2:1; __size8 bit3:1; __size8 bit4:1; __size8 bit5:1; __size8 bit6:1; __size8 bit7:1; __size8 bit8:1; } m; } a; 		// r8
    __size8 *p0; 		// r13
    __size16 r4; 		// r4
    __size16 r5; 		// r5
    __size16 r6; 		// r6
    __size8 r7; 		// r7

    a.x = *(r7 + 1);
    if (a.x.m.bit1 == 0) {
        a.x = *(r6 + 20);
        a.x.m.bit5 = 1;
        *(__size8*)(r6 + 20) = a.x;
        a.x.m.bit3 = 0;
        if (a.x.m.bit3 == 1) {
            a.x = *(r5 + 20);
            if (a.x.m.bit5 == 0) {
            }
            a.x = *(r5 + 6);
            a.x.m.bit7 = 0;
            a.x.m.bit6 = 0;
            *(__size8*)(r5 + 6) = a.x;
        }
        a.x = *(r6 + 20);
        if (a.x.m.bit5 == 0) {
        }
        a.x = *(r6 + 6);
        a.x.m.bit7 = 0;
        a.x.m.bit6 = 0;
        *(__size8*)(r6 + 6) = a.x;
    }
    a.x = *56;
    if (a.x.m.bit6 == 1) {
        a.x = *p0;
        if (a.x.m.bit2 == 1) {
            a.x = *(r4 + 4);
            a.x = a.x & 0xc0;
            a.x = *(r6 + 6);
            a.x = a.x & 0xc0;
            a.x = a.x;
            *(__size8*)(r6 + 6) = a.x;
        }
        a.x = *(r5 + 20);
        if (a.x.m.bit4 == 1) {
            a.x = *(r4 + 4);
            a.x = a.x & 0xc0;
            a.x = *(r6 + 6);
            a.x = a.x & 0xc0;
            a.x = a.x;
            *(__size8*)(r6 + 6) = a.x;
        }
    }
    return a.x;
}


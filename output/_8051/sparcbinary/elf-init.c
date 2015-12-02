// address: 0x10474
int main(int argc, char *argv[], char *envp[]) {
    __size8 a; 		// r8

    if (*630 == 0) {
        a = *1006;
        *(__size8*)223 = 1;
        *(__size8*)1006 = a;
        *(__size8*)1029 = 0;
        if (*1029 == 1) {
            if (*223 == 0) {
            }
            a = *883;
            *(__size8*)158 = 0;
            *(__size8*)962 = 0;
            *(__size8*)883 = a;
        }
        if (*223 == 0) {
        }
        a = *992;
        *(__size8*)158 = 0;
        *(__size8*)962 = 0;
        *(__size8*)992 = a;
    }
    a = *56;
    if (*962 == 1) {
        if (*167 == 1) {
            a = *1019;
            *(char*)9 = a & 0xc0;
            a = *992;
            a = a & 0xc0 | *9;
            *(char*)992 = a;
        }
        a = *897;
        if (*235 == 1) {
            a = *1019;
            *(char*)9 = a & 0xc0;
            a = *992;
            a = a & 0xc0 | *9;
            *(char*)992 = a;
        }
    }
    return a;
}


#include <stdio.h>
#include <stdint.h>
  
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x19521025, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

int main() {
    uint32_t v[4]={509568428, 972337464, 1169251202, 832286518};
    uint32_t const k[4]={0x68017634, 0x5ADB5C5D, 0x9278D9D2, 0xFA3EEAA3};
    unsigned int r = 32;
    decipher(r, v, k);
    printf("%x %x ",v[0],v[1]);
    decipher(r, v + 2, k);
    printf("%x %x\n",v[2],v[3]);
    return 0;
}

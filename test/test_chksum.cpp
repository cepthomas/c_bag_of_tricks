#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "pnut.h"

extern "C"
{
#include "chksum.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(CHKSUM_CRC, "Test crc32 generation.")
{
    // FILE* fp = fopen("..\\test\\files\\hemingway_short.txt", "r");
    FILE* fp = fopen("..\\test\\files\\hemingway.txt", "r");

    UT_NOT_NULL(fp);

    uint8_t buf[0xFFFF] = {0};
    size_t num_read = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);

    UT_EQUAL(num_read, 0xFFFF);

    //0xEDB88320  3,988,292,384

    // UT_EQUAL(sizeof(buf), 0);
    // printf("---%d\n", buf[10000]);

    // UT_EQUAL((int)buf[1000], 0xFFFF);
    // UT_EQUAL((int)buf[2000], 0xFFFF);
    // UT_EQUAL((int)buf[3000], 0xFFFF);

    // uint32_t chksum_crc32(uint32_t crc, const uint8_t* buf, size_t len, bool adjust_zero);

    uint32_t chksum = chksum_crc32(0, buf, sizeof(buf), true);
    UT_EQUAL(chksum, 2828471870);

    chksum = chksum_crc32(0, buf, 100, true);
    UT_EQUAL(chksum, 429695998);

    chksum = chksum_crc32(0, buf, 9000, true);
    UT_EQUAL(chksum, 3299620001);

    chksum = chksum_crc32(0, buf, 50000, true);
    UT_EQUAL(chksum, 3408412887);

    return 0;
}    

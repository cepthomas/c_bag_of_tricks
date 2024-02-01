#include <cstdio>
#include <cstring>

#include "pnut.h"

extern "C"
{
#include "mathutils.h"
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(MATHUTILS_STATS, "Test gen statistics.")
{
    // Some weather data.
    double vals[] = { 0.418, 3.299, 10.27, 19.1, 11.03, 8.63, 4.25, 16.57, 18.21, 17.61, 2.896, 8.89, 5.27, 9.66, 11.78, 14.08, 19.16, 24.07, 7.157, 16.08, 22.55, 23.43, 13.03, 6.148, 8.48, 12.66, 0.469, 1.207, 13.94, 18.13, 19.24, 12.78, 21.41, 20.03, 15.72, 17.83, 21.65, 11.86, 21.52, 17.75, 10.6, 8.05, 7.864, 4.835 };
    int num = (int)(sizeof(vals) / sizeof(double));

    stat_results_t res;
    mathutils_CalcStats(vals, num, &res);
    UT_EQUAL(res.num_vals, num);
    UT_CLOSE(res.mean, 12.718, 0.001);
    UT_CLOSE(res.min, 0.418, 0.001);
    UT_CLOSE(res.max, 24.07, 0.001);
    UT_CLOSE(res.sd, 6.693, 0.001);

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
UT_SUITE(MATHUTILS_CRC, "Test crc32 generation.")
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

    // uint32_t mathutils_crc32(uint32_t crc, const uint8_t* buf, size_t len, bool adjust_zero);

    uint32_t chksum = mathutils_crc32(0, buf, sizeof(buf), true);
    UT_EQUAL(chksum, 2828471870);

    chksum = mathutils_crc32(0, buf, 100, true);
    UT_EQUAL(chksum, 429695998);

    chksum = mathutils_crc32(0, buf, 9000, true);
    UT_EQUAL(chksum, 3299620001);

    chksum = mathutils_crc32(0, buf, 50000, true);
    UT_EQUAL(chksum, 3408412887);

    return 0;
}    

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <time.h>

#define TEST_COUNT  10000
#define ARRAY_SIZE  50000
#define PARALLEL_DEGREE   4
#define PARALLEL_VALUE   1 << (PARALLEL_DEGREE)

int32_t newArray(int32_t **ppDatas, int32_t size);
int32_t deleteArray(int32_t **ppDatas, int32_t size);

typedef int32_t * (*MEMSETFUNC)(int32_t *, int32_t, int32_t size);

uint64_t getMicroTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (static_cast<uint64_t>(t.tv_sec) * 1000000ul
            + static_cast<uint64_t>(t.tv_usec));
}

//数组访问
int32_t *memsetCase0(int32_t *p, int32_t v, int32_t size)
{
    memset(p, v, sizeof(int32_t) * size);
    return p;
}

//数组访问
int32_t *memsetCase1(int32_t *p, int32_t v, int32_t size)
{
    for (int i = 0; i<size ;i++) {
        p[i] = v;
    }
    return p;
}

//数组访问优化
int32_t *memsetCase4(int32_t *p, int32_t v, int32_t size)
{
    int64_t value = v;
    int64_t *p64 = (int64_t *)p;
    int32_t num = size >> 1;
    for (int i = 0; i < num ;i++) {
        p64[i] = value;
    }
    if (size & 1) {
        p[size -1] = v;
    }
    
    return p;
}

//指针访问
int32_t *memsetCase2(int32_t *p, int32_t v, int32_t size)
{
    int32_t *begin = p;
    while(size-->0) {
        *begin++ = v;
    }
    return p;
}

//测试发现64的时候速度最快
int32_t *memsetCase3(int32_t *p, int32_t v, int32_t size)
{

#define D(x,i) x[i] = v;
#define D2(x,i) D(x,i) D(x, i+1)
#define D4(x,i) D2(x,i) D2(x, i+2)
#define D8(x,i) D4(x,i) D4(x, i+4)
#define D16(x,i) D8(x,i) D8(x, i+8)
#define D32(x,i) D16(x,i) D16(x, i+16)
#define D64(x,i) D32(x,i) D32(x, i+32)
#define D128(x,i) D64(x,i) D64(x, i+64)
#define D256(x,i) D128(x,i) D128(x, i+128)
#define D512(x,i) D256(x,i) D256(x, i+256)
#define D1024(x,i) D512(x,i) D512(x, i+512)

    int32_t num, i;
    for(i = 0, num = size >> PARALLEL_DEGREE ; num > 0 ; i += PARALLEL_VALUE) {
#if     PARALLEL_DEGREE == 1
        D2(p, i)
#elif   PARALLEL_DEGREE == 2
        D4(p, i)
#elif   PARALLEL_DEGREE == 3
        D8(p, i)
#elif   PARALLEL_DEGREE == 4
        D16(p, i)
#elif   PARALLEL_DEGREE == 5
        D32(p, i)
#elif   PARALLEL_DEGREE == 6
        D64(p, i)
#elif   PARALLEL_DEGREE == 7
        D128(p, i)
#elif   PARALLEL_DEGREE == 8
        D256(p, i)
#elif   PARALLEL_DEGREE == 9
        D512(p, i)
#elif   PARALLEL_DEGREE == 10
        D1024(p, i)
#else
#error  invalid PARALLEL_DEGREE
#endif
        num--;
    }

    for (; i < size; ++i ) {
        p[i] = v;
    }
    return p;
}

int32_t *memsetCase5(int32_t *p, int32_t v, int32_t size)
{

    //__attribute__((aligned(8)))
    while (((int64_t)p & 0x7) && (size > 0)) {
        *p++ = v;
        size--;
    }
    int64_t * p64 = (int64_t *)p;
    for(int i = 0, num = size >> (PARALLEL_DEGREE + 1) ; num > 0 ; num--) {
#if     PARALLEL_DEGREE == 1
        D2(p64, i)
#elif   PARALLEL_DEGREE == 2
        D4(p64, i)
#elif   PARALLEL_DEGREE == 3
        D8(p64, i)
#elif   PARALLEL_DEGREE == 4
        D16(p64, i)
#elif   PARALLEL_DEGREE == 5
        D32(p64, i)
#elif   PARALLEL_DEGREE == 6
        D64(p64, i)
#elif   PARALLEL_DEGREE == 7
        D128(p64, i)
#elif   PARALLEL_DEGREE == 8
        D256(p64, i)
#elif   PARALLEL_DEGREE == 9
        D512(p64, i)
#elif   PARALLEL_DEGREE == 10
        D1024(p64, i)
#else
#error  invalid PARALLEL_DEGREE
#endif
        p64 +=  PARALLEL_VALUE;
    }

    p = (int32_t *)p64;
    int32_t res = size % (PARALLEL_VALUE << 1);
    for (int i = 0 ; i < res; ++i ) {
        p[i] = v;
    }
    return p;
}

typedef unsigned char __attribute__((aligned(16))) fill_t[16];

int32_t *memsetCase6(int32_t *page, int32_t fill, int32_t size)
{  
    unsigned char *dst = (unsigned char*)page;  
    int32_t count = size << 2;
    fill_t dfill;  
    for(size_t i = 0;i<16;)  
    {  
        dfill[i++]=fill;  
    }  
    __asm__ __volatile__ (  
            " movdqa (%0),%%xmm0\n"  
            " movdqa %%xmm0,%%xmm1\n"  
            " movdqa %%xmm0,%%xmm2\n"  
            " movdqa %%xmm0,%%xmm3\n"  
            " movdqa %%xmm0,%%xmm4\n"  
            " movdqa %%xmm0,%%xmm5\n"  
            " movdqa %%xmm0,%%xmm6\n"  
            " movdqa %%xmm0,%%xmm7\n"  
            :: "r"(dfill)  
            );  
    while (((long)dst & 0xF) && (count > 0)) {  
        *dst++ = fill;  
        count--;  
    }  
    size_t m_loop = count/128;  
    size_t r = count%128;  
    for(size_t i=0;i<m_loop;++i)  
    {  
        __asm__ (  
                "  movntdq %%xmm0, (%0)\n"  
                "  movntdq %%xmm1, 16(%0)\n"  
                "  movntdq %%xmm2, 32(%0)\n"  
                "  movntdq %%xmm3, 48(%0)\n"  
                "  movntdq %%xmm4, 64(%0)\n"  
                "  movntdq %%xmm5, 80(%0)\n"  
                "  movntdq %%xmm6, 96(%0)\n"  
                "  movntdq %%xmm7, 112(%0)\n"  
                ::"r" (dst) :"memory" );  
        dst+=128;  
    }  
    for(int i=0;i<r;++i)  
    {  
        *dst++ = fill;  
    }  
    __asm__ __volatile__ (  
            " sfence \n "  
            ::  
            );  
    return (int32_t *)page;
}

void perforTestFunc(MEMSETFUNC func, const char *caseName) 
{
    uint64_t start , end;
    int32_t  *pTestDatas[TEST_COUNT] = {0};

    // 分配内存空间
    if (0 != newArray(pTestDatas, TEST_COUNT)) {
        fprintf(stderr, "new mem faile.\n"); 
        return ;
    }

    // 性能测试
    start = getMicroTime();
    for (int i = 0; i < TEST_COUNT; i++) {
        func(pTestDatas[i], 0, ARRAY_SIZE);    
    }
    end = getMicroTime();
    fprintf(stderr, "%s[%d] : %ld\n", caseName,
            pTestDatas[rand() % TEST_COUNT][rand() % ARRAY_SIZE], 
            end - start);
    deleteArray(pTestDatas, TEST_COUNT);

}

int32_t newArray(int32_t **ppDatas, int32_t size)
{
    if (!ppDatas || size <= 0) return -1;

    int32_t i = 0;
    for (; i < size; i++) {
        ppDatas[i] = (int32_t *)malloc(sizeof(int32_t)*ARRAY_SIZE);
        if (!ppDatas[i]) {
            fprintf(stderr, "malloc array fail.\n");
            goto fail;
        }
    }
    return 0;

fail:
    for(; i>0; i--) {
        free(ppDatas[i]);
        ppDatas[i] = NULL;
    }
    return -1;
}

int32_t deleteArray(int32_t **ppDatas, int32_t size)
{
    if (!ppDatas || size <= 0) return -1;

    int32_t i = 0;
    for (; i < size; i++) {
        if (!ppDatas[i]) {
            free(ppDatas[i]);
            ppDatas[i] = NULL;
        }
    }
    return 0;
}


#define PERFORTEST(func)                            \
    fprintf(stderr, "%s test begin.\n", #func);     \
    perforTestFunc(func, #func);                    \
    fprintf(stderr, "%s test end.\n\n", #func);     \

int main() 
{

    srand(time(NULL));

    // memset的基准版本
    PERFORTEST(memsetCase0);
    //end case 1


    // case 1
    PERFORTEST(memsetCase1);
    //end case 1

    // case 2
    PERFORTEST(memsetCase2);
    //end case 2

    // case 3
    PERFORTEST(memsetCase3);
    //end case 3
    
    // case 4
    PERFORTEST(memsetCase4);
    //end case 4

    // case 5
    PERFORTEST(memsetCase5);
    //end case 5

    // case 6
    PERFORTEST(memsetCase6);
    //end case 6


    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <time.h>

#define TEST_COUNT  10000
#define ARRAY_SIZE  50000
#define PARALLEL_DEGREE   5
#define PARALLEL_VALUE   1 << (PARALLEL_DEGREE)

int32_t newArray(int32_t **ppDatas, int32_t size);
int32_t deleteArray(int32_t **ppDatas, int32_t size);


struct RankerDoc {
    int32_t *p; 
    int32_t offset;
    void setDocIndex(int32_t off) { offset = off; }
    int32_t getDocIndex() { return offset; }
    void setData(int32_t *v) { p = v; }
};

typedef int32_t * (*MEMSETFUNC)(int32_t *, int32_t size, RankerDoc *pRankerDocs, RankerDoc ** ppRankerDocs);
uint64_t getMicroTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (static_cast<uint64_t>(t.tv_sec) * 1000000ul
            + static_cast<uint64_t>(t.tv_usec));
}

/*
//数组访问
int32_t *memsetCase0(int32_t size, RankerDoc *pRankerDocs, RankerDoc ** pRankerDocs)
{
    return p;
}
*/

//数组访问
int32_t *memsetCase1(int32_t *p, int32_t size, RankerDoc *pRankerDocs, RankerDoc ** ppRankerDocs)
{
    for (int i = 0; i<size ;i++) {
        pRankerDocs[i].p = &p[i];
        pRankerDocs[i].offset = i;
        ppRankerDocs[i] = &pRankerDocs[i];
    }
    return p;
}

//数组访问
int32_t *memsetCase2(int32_t *p, int32_t size, RankerDoc *pRankerDocs, RankerDoc ** ppRankerDocs)
{
    for (int i = 0; i<size ;i++) {
        pRankerDocs[i].offset = i;
        pRankerDocs[i].p = &p[i];
        ppRankerDocs[i] = &pRankerDocs[i];
    }
    return p;
}


//测试发现64的时候速度最快
int32_t *memsetCase10(int32_t *p, int32_t size, RankerDoc *pRankerDocs, RankerDoc ** ppRankerDocs)
{

#define A(i) pRankerDocs[i].setDocIndex(i);    
#define B(i) pRankerDocs[i].setData(&p[i]);
#define C(i) ppRankerDocs[i] = &pRankerDocs[i];

#define A2(i)   A(i)    A(i + 1)                       
#define A4(i)   A2(i)   A2(i + 2)                       
#define A8(i)   A4(i)   A4(i + 4)                       
#define A16(i)  A8(i)   A8(i + 8)                       
#define A32(i)  A16(i)  A16(i + 16)                       
#define A64(i)  A32(i)  A32(i + 32)                       
#define A128(i)  A64(i)  A64(i + 64)                       

#define B2(i)   B(i)    B(i + 1)                       
#define B4(i)   B2(i)   B2(i + 2)                       
#define B8(i)   B4(i)   B4(i + 4)                       
#define B16(i)  B8(i)   B8(i + 8)                       
#define B32(i)  B16(i)  B16(i + 16)                       
#define B64(i)  B32(i)  B32(i + 32)                       
#define B128(i)  B64(i)  B64(i + 64)                       

#define C2(i)   C(i)    C(i + 1)                       
#define C4(i)   C2(i)   C2(i + 2)                       
#define C8(i)   C4(i)   C4(i + 4)                       
#define C16(i)  C8(i)   C8(i + 8)                       
#define C32(i)  C16(i)  C16(i + 16)                       
#define C64(i)  C32(i)  C32(i + 32)                       
#define C128(i)  C64(i)  C64(i + 64)                       

#define D(i) A(i) B(i) C(i) 
#define D2(x,i)                                     \
    do {                                            \
        A2(i)                                       \
        B2(i)                                       \
        C2(i)                                       \
    }while(0);

#define D4(x,i)                                     \
    do {                                            \
        A4(i)                                       \
        B4(i)                                       \
        C4(i)                                       \
    }while(0);

#define D8(x,i)                                     \
    do {                                            \
        A8(i)                                       \
        B8(i)                                       \
        C8(i)                                       \
    }while(0);

#define D16(x,i)                                     \
    do {                                            \
        A16(i)                                       \
        B16(i)                                       \
        C16(i)                                       \
    }while(0);

#define D32(x,i)                                     \
    do {                                            \
        A32(i)                                       \
        B32(i)                                       \
        C32(i)                                       \
    }while(0);

#define D64(x,i)                                     \
    do {                                            \
        A64(i)                                       \
        B64(i)                                       \
        C64(i)                                       \
    }while(0);

#define D128(x,i)                                     \
    do {                                            \
        A128(i)                                       \
        B128(i)                                       \
        C128(i)                                       \
    }while(0);

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
#else
#error  invalid PARALLEL_DEGREE
#endif
        num--;
    }

    for (; i < size; ++i ) {
        pRankerDocs[i].offset = i;
        pRankerDocs[i].p = &p[i];
        ppRankerDocs[i] = &pRankerDocs[i];
    }
    return p;
}

/*
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
*/

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
    RankerDoc **ppRankerDocs = NULL;
    RankerDoc *pRankerDocs = new RankerDoc[ARRAY_SIZE];
    ppRankerDocs = new RankerDoc*[ARRAY_SIZE];
    // 分配内存空间
    if (0 != newArray(pTestDatas, TEST_COUNT)) {
        fprintf(stderr, "new mem faile.\n"); 
        return ;
    }

    // 性能测试
    start = getMicroTime();
    for (int i = 0; i < TEST_COUNT; i++) {
        func(pTestDatas[i], ARRAY_SIZE, pRankerDocs, ppRankerDocs);    
    }
    end = getMicroTime();
    fprintf(stderr, "%s[%d] : %ld\n", caseName,
            pTestDatas[rand() % TEST_COUNT][rand() % ARRAY_SIZE], 
            end - start);
    deleteArray(pTestDatas, TEST_COUNT);
    delete [] pRankerDocs;
    delete [] ppRankerDocs;

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

    // case 1
    PERFORTEST(memsetCase1);
    //end case 1

    PERFORTEST(memsetCase1);
    //end case 1


    // case 3
    PERFORTEST(memsetCase10);
    //end case 3
 
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "FastCalValue.h"
#include "HashMap.hpp"
#include "xrand.h"
#include "StringUtils.h"


typedef util::HashMap<uint64_t, int32_t> ScoreMap;
typedef ScoreMap::iterator ScoreMapIter;
typedef ScoreMap::const_iterator ScoreMapConstIter;

#define     MULTI_SEPARATOR     ';'
#define     KEYVALUE_SEPARATOR  ':'
#define     TEST_COUNT  500000
#define     ARRAY_SIZE  20

uint64_t getMicroTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (static_cast<uint64_t>(t.tv_sec) * 1000000ul
            + static_cast<uint64_t>(t.tv_usec));
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
        ppDatas[i][0] = 3000;
        for (int j = 1; j < ARRAY_SIZE; j++) {
            ppDatas[i][j] = ppDatas[i][j - 1] + xrand() % 100 + 1;
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


int32_t case1(int32_t *docDatas[], int32_t *dataCount, int32_t num, const ScoreMap &map)
{
    int32_t total = 0;
    uint64_t start , end;
    start = getMicroTime();
    for (int i = 0; i < num ; i++) {
        if (dataCount[i] <= 0 || docDatas[i] == NULL || map.size() == 0) {
            continue;
        }

        int32_t weight = 0;
        int32_t *values = docDatas[i];
        ScoreMapConstIter itr = map.begin();
        // weight ÊÇÀÛ¼ÓµÄ
        for (int i = 0; i < dataCount[i] ; ++i) {
            itr = map.find((uint64_t)values[i]);
            if (itr != map.end()) {
                weight += itr->value;
            }
        }
        total += weight;
    }
    end = getMicroTime();
    fprintf(stderr, "%s[%d] : %ld\n", __FUNCTION__, total,
            end - start);

    return total;
}



int32_t case2(int32_t *docDatas[], int32_t *dataCount, int32_t num, const std::string &arg)
{
    int32_t total = 0;
    uint64_t start , end;
    FastMatchKeyValue fastMatch(arg);

    start = getMicroTime();
    for (int i = 0; i < num ; i++) {
        if (dataCount[i] <= 0 || docDatas[i] == NULL ) {
            continue;
        }
        
        total += fastMatch(docDatas[i], dataCount[i], 0);
    }
    end = getMicroTime();
    fprintf(stderr, "%s[%d] : %ld\n", __FUNCTION__, total,
            end - start);
    return total;
}

int32_t init(const std::string &arg, ScoreMap &map)
{
    if (arg.empty()) {
        return -1;
    }

    std::vector<std::string> kv_Vec;
    std::vector<std::string> keyValue;
    util::split(MULTI_SEPARATOR, arg, kv_Vec);
    for (int i = 0; i < kv_Vec.size(); i++) {
        if (kv_Vec[i].empty()) {
            continue;
        }
        util::split(KEYVALUE_SEPARATOR, kv_Vec[i], keyValue);
        if (keyValue.size() != 2 || keyValue[0].empty() || keyValue[1].empty()) {
            return -1;
        }
        if (!util::isDigit(keyValue[0].c_str()) || !util::isDigit(keyValue[1].c_str())) {
            return -1;
        }
        map.insert((uint64_t)atoi(keyValue[0].c_str()), 
                (int32_t)atoi(keyValue[1].c_str()));
    }
    return 0;
}


int main()
{

    int32_t  *pTestDatas[TEST_COUNT] = {0};
    int32_t  datasCount[TEST_COUNT] = {0};
//    static const std::string szKeyValue = "2342:223;3212:45;321:45;13:45;23231:34;8860:29999;9374:5999;88272:987;98271:3232;726:394857;7663:876;12345:587;23456:3456;3000:87766";
    static const std::string szKeyValue = "2342:223;3212:45;321:45;13:45;23231:34;8860:29999;9374:5999;88272:987;98271:3232;726:394857;7663:876;12345:587;3200:3456;3200:87766";
    ScoreMap map;
    if (init(szKeyValue, map) != 0) {
        fprintf(stderr, "init fail.\n");
        return -1;
    }
    
    if (0 != newArray(pTestDatas, TEST_COUNT)) {
        fprintf(stderr, "new mem faile.\n");
        return -1;
    }

    for (int i = 0; i < TEST_COUNT; i++) {
        datasCount[i] = ARRAY_SIZE;
    }

    case1(pTestDatas, datasCount, TEST_COUNT, map);
    case2(pTestDatas, datasCount, TEST_COUNT, szKeyValue);
    
    deleteArray(pTestDatas, TEST_COUNT);

    return 0;
}

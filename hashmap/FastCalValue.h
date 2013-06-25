/**
 * File name: fastcalvalue.h
 * Author: fengba
 * Create time: 2013-06-21 23:59:08
 * $Id$
 * 
 * Description: ***add description here***
 * 
 */

#ifndef SORT_FRAMEWORK_PLUGIN_FASTCALVALUE_H_
#define SORT_FRAMEWORK_PLUGIN_FASTCALVALUE_H_

//*****add include headers here...

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <set>

#include "StringUtils.h"


#if __WORDSIZE == 64
#define __INT64_C(c)  c ## L
#define __UINT64_C(c) c ## UL
#else
#define __INT64_C(c)  c ## LL
#define __UINT64_C(c) c ## ULL
#endif

#ifndef UINT64_MAX
#define UINT64_MAX     (__UINT64_C(18446744073709551615))
#endif

#define     MULTI_SEPARATOR     ';'
#define     KEYVALUE_SEPARATOR  ':'

typedef struct {
    uint64_t key;
    uint32_t score;
} KeyValueStruct;

template<typename T>
    class DataSearch {
        public:
            DataSearch(const T *pdata, int32_t num) : _pData(pdata), _dataNum(num)
        {}

        uint64_t seek(uint64_t init_value) {
            T value = init_value;
            while (_dataNum > 0 && *_pData < value) {
                _pData++;
                _dataNum--;
            }
            return _dataNum > 0 ? *_pData : UINT64_MAX;
        }
        private:
            const T *_pData;    
            int32_t _dataNum;
    };

int compare(const void *no1, const void *no2)
{
    KeyValueStruct &v1 = *(KeyValueStruct *)no1;
    KeyValueStruct &v2 = *(KeyValueStruct *)no2;
    return v1.key - v2.key;
}



class FastMatchKeyValue
{
public:
    FastMatchKeyValue() : m_pData(NULL), m_dataNum(0) {}
    FastMatchKeyValue(const std::string &arg) : m_pData(NULL), m_dataNum(0) 
    { init(arg); }
    ~FastMatchKeyValue() {}
    
    int32_t init(const std::string &arg)
    {
        if (arg.empty()) {
            return -1;
        }
        int32_t index = 0;
        std::vector<std::string> kv_Vec;
        std::vector<std::string> keyValue;
        util::split(MULTI_SEPARATOR, arg, kv_Vec);
        std::set<std::string> keys;
        if ((m_dataNum = kv_Vec.size()) == 0) return 0;
        m_pData = new KeyValueStruct[m_dataNum];
        if (m_pData == NULL) {
            fprintf(stderr, "%s:%d malloc fail.\n", __FILE__, __LINE__ - 2);
            return -1;
        }
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
            if (keys.find(keyValue[0]) != keys.end()) {
                fprintf(stderr, "%s report pass .\n", keyValue[0].c_str());
                continue;
            }
            m_pData[index].key = (uint64_t)atoi(keyValue[0].c_str());
            m_pData[index].score = (uint64_t)atoi(keyValue[1].c_str());
            index++;
            keys.insert(keyValue[0]);
        }
        if (index > 1) {
            qsort(m_pData, m_dataNum, sizeof(KeyValueStruct), compare);
        }
        return 0;
    }

    template<typename T>
    int32_t operator()(T pMulValues[], int32_t valueSize, int32_t init)
    {
        if (pMulValues == NULL || valueSize <= 0) {
            return 0;
        }
        int32_t totalScore = 0;
        int32_t score = 0;
        int32_t dataNum = m_dataNum;
        KeyValueStruct *pData = m_pData;
        DataSearch<T> search(pMulValues, valueSize);

        uint64_t value = search.seek(init);
        uint64_t tmp_value;
        while(value != UINT64_MAX) {
            tmp_value = seek(pData, dataNum, value, score);
            if (tmp_value == UINT64_MAX) {
                break;
            }
            if (tmp_value == value) {
                totalScore += score; 
                tmp_value += 1;
            }
            value = search.seek(tmp_value);
        }
        return totalScore;
    }

private:
    uint64_t seek(KeyValueStruct * &pData, int32_t & dataNum, uint64_t value, int32_t &score) {
        while (dataNum > 0 && pData->key < value) {
            pData++;
            dataNum--;
        }
        if (dataNum > 0) score = pData->score;
        return dataNum > 0 ? pData->key : UINT64_MAX;
    }

private:
    KeyValueStruct *m_pData;
    int32_t m_dataNum;
};

#endif /*SORT_FRAMEWORK_PLUGIN_FASTCALVALUE_H_*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <istream.h>
#include <sys/time.h>
#include <stdlib.h>

#define TYPE_NUM   1000
#define TYPE_COUNT    10000
#define INT_DEFAULT rand()
#define STRING_DEFAULT "123456789123456789#$"

uint64_t getMicroTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (static_cast<uint64_t>(t.tv_sec) * 1000000ul
            + static_cast<uint64_t>(t.tv_usec));
}

template <typename T>
int32_t case1_new(int32_t size, T ** ret, T & value) {

    if (ret == NULL && size <= 0) return -1;
    T *p = (T *)malloc(sizeof(T) * size); 
    if (p == NULL) {
        return -2;
    }
    for (int i = 0; i < size ; i++) {
        new (&p[i]) T(value);
    }
    *ret = p;
    return 0;
}

template <typename T>
int32_t case3_new(int32_t size, T ** ret) {

    if (ret == NULL && size <= 0) return -1;
    T *p = (T *)malloc(sizeof(T) * size); 
    if (p == NULL) {
        return -2;
    }
    for (int i = 0; i < size ; i++) {
        new (&p[i]) T();
    }
    *ret = p;
    return 0;
}

template <typename T>
int32_t case2_new(int32_t size, T ** ret, T & value) {

    if (ret == NULL && size <= 0) return -1;
    T *p = new T[size]; 
    if (p == NULL) {
        return -2;
    }
    for (int i = 0; i < size ; i++) {
        p[i] = value;
    }
    *ret = p;
    return 0;
}

template <typename T>
int32_t case4_new(int32_t size, T ** ret) {

    if (ret == NULL && size <= 0) return -1;
    T *p = new T[size]; 
    if (p == NULL) {
        return -2;
    }
    *ret = p;
    return 0;
}


int32_t INT_CASE_TEST()
{

    int32_t *pInt[TYPE_COUNT] = {NULL};
    uint64_t start , end;

    srand( (unsigned)time(NULL));

    int32_t r1 = rand() % TYPE_COUNT;
    int32_t r2 = rand() % TYPE_NUM;

    /* ------------------------------------------------------*/
    /* int case begin */
    int32_t value = INT_DEFAULT;

    /* case1 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case1_new<int32_t>((int32_t)TYPE_NUM, &pInt[i], value)) {
            cout << "case1_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();
    /* case1 end */

    cout << "int case1_new process :" << end - start << " value : " << pInt[r1][r2] <<endl;

    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pInt[i]) { 
            free(pInt[i]);
            pInt[i] = NULL;
        }
    }

    /* case3 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case3_new<int32_t>((int32_t)TYPE_NUM, &pInt[i])) {
            cout << "case3_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();

    cout << "int case3_new process :" << end - start << " value : " << pInt[r1][r2] <<endl;

    /* case3 end */
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pInt[i]) { 
            free(pInt[i]);
            pInt[i] = NULL;
        }
    }

    /* case2 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case2_new<int32_t>((int32_t)TYPE_NUM, &pInt[i], value)) {
            cout << "case2_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();

    cout << "int case2_new process :" << end - start << " value : " << pInt[r1][r2] <<endl;
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pInt[i]) { 
            delete [](pInt[i]);
            pInt[i] = NULL;
        }
    }
    /* case2 end */
    return 0;
}


int32_t STRING_CASE_TEST()
{
    std::string *pString[TYPE_COUNT] = {NULL};
    uint64_t start , end;

    srand( (unsigned)time(NULL));

    int32_t r1 = rand() % TYPE_COUNT;
    int32_t r2 = rand() % TYPE_NUM;


    std::string str = STRING_DEFAULT;

    /* string case */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case1_new<std::string>((int32_t)TYPE_NUM, &pString[i], str)) {
            cout << "case1_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();
    cout << "std::string case1_new process :" << end - start << " value : " << pString[r1][r2] <<endl;
    /* case 1 end */

    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pString[i]) { 
            free(pString[i]);
            pString[i] = NULL;
        }
    }

    /* case 3 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case3_new<std::string>((int32_t)TYPE_NUM, &pString[i])) {
            cout << "case3_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();
    cout << "std::string case3_new process :" << end - start << " value : " << pString[r1][r2] <<endl;
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pString[i]) { 
            free(pString[i]);
            pString[i] = NULL;
        }
    }
    /* case 3  end*/

    /* case 2 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case2_new<std::string>((int32_t)TYPE_NUM, &pString[i], str)) {
            cout << "case2_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();
    cout << "std::string case2_new process :" << end - start << " value : " << pString[r1][r2] <<endl;
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pString[i]) { 
            delete [](pString[i]);
            pString[i] = NULL;
        }
    }

    /* case 4 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (0 != case4_new<std::string>((int32_t)TYPE_NUM, &pString[i])) {
            cout << "case4_new_std process faild.." << endl; 
            return -1;
        }
    }
    end = getMicroTime();
    cout << "std::string case4_new process :" << end - start << " value : " << pString[r1][r2] <<endl;
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        if (pString[i]) { 
            delete [](pString[i]);
            pString[i] = NULL;
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) 
{

    INT_CASE_TEST();
    STRING_CASE_TEST();
    return 0;

}

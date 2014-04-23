#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <istream.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string>

#define TYPE_NUM   1000
#define TYPE_COUNT    1000000
#define INT_DEFAULT rand()
#define STRING_DEFAULT "123456789123456789#$"

uint64_t getMicroTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (static_cast<uint64_t>(t.tv_sec) * 1000000ul
            + static_cast<uint64_t>(t.tv_usec));
}

void case1_format(int32_t value, std::string &out)
{
    std::stringstream ss;
    ss << value; 
//    out = ss.str();
}

void case2_format(int32_t value, char *output, int32_t size)
{
    snprintf(output, size, "%ld", value);
}

void case3_format(int32_t value, char *output)
{
    sprintf(output, "%d", value);
}


void case4_format(int32_t value, char *output)
{
    const char trans_char[]="0123456789";
    char *begin = output;
    do
        *output++ = trans_char[value % 10];
    while ((value /= 10) != 0);
    *output = '\0';

    char c;
    while(begin < output -1) {
        c = *--output;
        *output = *begin;
        *begin++ = c;
    }
}

void case1_sformat(const std::string &value, std::string &out)
{
    std::stringstream ss;
    ss << value; 
}

void case2_sformat(const std::string &value, char *output, int32_t size)
{
    snprintf(output, size, "%s", value.c_str());
}

void case3_sformat(const std::string &value, char *output)
{
    sprintf(output, "%s", value.c_str());
}


void case4_sformat(const std::string &value, char *output)
{
//    memcpy(output, value.c_str(), value.length());
    strcpy(output, value.c_str());
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
int32_t case6_new(int32_t size, T ** ret, T & value) {

    if (ret == NULL && size <= 0) return -1;
    T *p = (T *)malloc(sizeof(T) * size); 
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

    uint64_t start , end;

    srand( (unsigned)time(NULL));

    int32_t r1 = rand() % TYPE_COUNT;
    int32_t r2 = rand() % TYPE_NUM;
//    const std::string r1 = "zhangtidsjfksfjsfdslfjs";

    /* ------------------------------------------------------*/
    /* int case begin */
    int32_t value = INT_DEFAULT;

    /* case1 */
    std::string test;
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case1_format(r1, test);
    }
    end = getMicroTime();
    /* case1 end */

    cout << "int case1_new process :" << end - start << " value : "  <<endl;

    

    /* case3 */

    char buff[100];
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case2_format(r1, buff, 100);
    }
    end = getMicroTime();

    cout << "int case2_new process :" << end - start << " value : "<<endl;

    /* case3 end */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case3_format(r1, buff);
    }
    end = getMicroTime();
    cout << "int case3_new process :" << end - start << " value : "<<endl;

    /* case2 end */
    /* case6 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case4_format(r1, buff);
    }
    end = getMicroTime();
    cout << "int case4_new process :" << end - start << " value : "<<endl;
    
    /* case6  end*/
    return 0;
}

int32_t STRING_CASE_TEST()
{

    uint64_t start , end;

    srand( (unsigned)time(NULL));

//    int32_t r1 = rand() % TYPE_COUNT;
    int32_t r2 = rand() % TYPE_NUM;
    const std::string r1 = "zhangtidsjfksfjsfdslfjs";

    /* ------------------------------------------------------*/
    /* int case begin */
    int32_t value = INT_DEFAULT;

    /* case1 */
    std::string test;
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case1_sformat(r1, test);
    }
    end = getMicroTime();
    /* case1 end */

    cout << "int case1_new process :" << end - start << " value : "  <<endl;

    

    /* case3 */

    char buff[100];
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case2_sformat(r1, buff, 100);
    }
    end = getMicroTime();

    cout << "int case2_new process :" << end - start << " value : "<<endl;

    /* case3 end */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case3_sformat(r1, buff);
    }
    end = getMicroTime();
    cout << "int case3_new process :" << end - start << " value : "<<endl;

    /* case2 end */
    /* case6 */
    start = getMicroTime();
    for (int i = 0 ; i < TYPE_COUNT; ++i) {
        case4_sformat(r1, buff);
    }
    end = getMicroTime();
    cout << "int case4_new process :" << end - start << " value : "<<endl;
    
    /* case6  end*/
    return 0;
}

int main(int argc, const char * argv[]) 
{

    cout << "int test" << endl;
    INT_CASE_TEST();
    cout << "string test" << endl;
    STRING_CASE_TEST();
    return 0;

}

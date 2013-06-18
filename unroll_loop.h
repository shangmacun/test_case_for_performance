#ifndef  UNROLL_LOOP_INC
#define  UNROLL_LOOP_INC

//通过__VA_ARGS__计算参数数量（不能大于6个参数, 大于5时编译失败，大于6时预处理失败）
//这个宏是实现宏接口重载的利器,虽然没有c++那么灵活, 非常perfect!!!
#define _COUNT_PARMS(_1, _2, _3, _4, _5, _6, _, ...) _
#define COUNT_PARMS(...)    \
    _COUNT_PARMS(__VA_ARGS__,6, 5, 4, 3, 2, 1)

#define _JOIN(A, B)  A##B
#define JOIN(A, B)   _JOIN(A, B)

#define DEFAULT_DEGREE          5
#define DEGREE_VALUE            (1 << DEFAULT_DEGREE)

#define C1(op, i) op(i); op(i + 1);
#define C2(op, i) C1(op, i) C1(op, i + (1<<1))
#define C3(op, i) C2(op, i) C2(op, i + (1<<2))
#define C4(op, i) C3(op, i) C3(op, i + (1<<3))
#define C5(op, i) C4(op, i) C4(op, i + (1<<4))
#define C6(op, i) C5(op, i) C5(op, i + (1<<5))

//为了支持##后面的宏展开，新加了一层
#define REPEATE(op, i) JOIN(C, DEFAULT_DEGREE)(op, i)

#define UNROLL_LOOP5(start, end, op1, op2, op3, op4, op5)  \
    do { \
        int i  = start; \
        int loop_end = end - DEGREE_VALUE; \
        for(; i<loop_end; i+=DEGREE_VALUE) {  \
            REPEATE(op1, i) \
            REPEATE(op2, i) \
            REPEATE(op3, i) \
            REPEATE(op4, i) \
            REPEATE(op5, i) \
        } \
        for (; i<end; ++i) { \
            op1(i); \
            op2(i); \
            op3(i); \
            op4(i); \
            op5(i); \
        } \
    } while(0);

#define UNROLL_LOOP4(start, end, op1, op2, op3, op4)  \
    do { \
        int i  = start; \
        int loop_end = end - DEGREE_VALUE; \
        for(; i<loop_end; i+=DEGREE_VALUE) {  \
            REPEATE(op1, i) \
            REPEATE(op2, i) \
            REPEATE(op3, i) \
            REPEATE(op4, i) \
        } \
        for (; i<end; ++i) { \
            op1(i); \
            op2(i); \
            op3(i); \
            op4(i); \
        } \
    } while(0);

#define UNROLL_LOOP3(start, end, op1, op2, op3)  \
    do { \
        int i  = start; \
        int loop_end = end - DEGREE_VALUE; \
        for(; i<loop_end; i+=DEGREE_VALUE) {  \
            REPEATE(op1, i) \
            REPEATE(op2, i) \
            REPEATE(op3, i) \
        } \
        for (; i<end; ++i) { \
            op1(i); \
            op2(i); \
            op3(i); \
        } \
    } while(0);

#define UNROLL_LOOP2(start, end, op1, op2)  \
    do { \
        int i  = start; \
        int loop_end = end - DEGREE_VALUE; \
        for(; i<loop_end; i+=DEGREE_VALUE) {  \
            REPEATE(op1, i) \
            REPEATE(op2, i) \
        } \
        for (; i<end; ++i) { \
            op1(i); \
            op2(i); \
        } \
    } while(0);

#define UNROLL_LOOP1(start, end, op1)  \
    do { \
        int i  = start; \
        int loop_end = end - DEGREE_VALUE; \
        for(; i<loop_end; i+=DEGREE_VALUE) {  \
            REPEATE(op1, i) \
        } \
        for (; i<end; ++i) { \
            op1(i); \
        } \
    } while(0);



#define UNROLL_LOOP(start, end, ...)  \
    JOIN(UNROLL_LOOP, COUNT_PARMS(__VA_ARGS__))(start, end, __VA_ARGS__)

/* example */
// 先定义操作的宏, 然后根据op的个数使用UNROLL_LOOP1.. UNROLL_LOOP4
// 最多可以支持5个操作
//#define OP1(i) barray[i] = array[i];
//#define OP2(i) barray[i] *= 2;
//#define OP3(i) barray[i] /= 2;
//#define OP4(i) barray[i] -= 1;
//使用方式：
//UNROLL_LOOP(0, 200, OP1, OP2, OP3, OP4);
// 第一个参数, 表示开始下标: 0
// 第二个参数, 表示结束下标: 200
// 第4, 5, 6, 7个参数, 表示循环中要展开的操作, 由前面的#define定义


#endif   /* ----- #ifndef UNROLL_LOOP_INC  ----- */
//vim:set tabstop=4 expandtab fileencoding=utf-8 ff=unix ft=cpp norl:


/*
 * =====================================================================================
 * 
 *       Filename:  xrand.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  2013-06-14 20:12:24
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  yiyan.jyj (search engine group), yiyan.jyj@taobao.com
 *        Company:  www.taobao.com
 * 
 * =====================================================================================
 */

#ifndef  XRAND_INC
#define  XRAND_INC

#include "xrand.h"
#include <stdint.h>

uint32_t xrand() {
    uint32_t time_high, time_low;
    __asm__(
            "rdtsc;"
            "movl %%edx,%0;"
            "movl %%eax,%1;"
            :"=m"(time_high),"=m"(time_low)
            :
            :"%edx","%eax"
           );
    return (time_high ^ time_low);
}

#endif   /* ----- #ifndef XRAND_INC  ----- */


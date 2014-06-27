/*
 * Copyright (c) 2010-2014 Dali Wang, All Rights Reserved.
 *
 * CodeName: PiXiu
 *  Authors:
 *      Dali Wang <wangdali@qq.com>
 *
 *
 */
#ifndef _STRTOKEN_H_
#define _STRTOKEN_H_
#include <string.h>
#include <stddef.h>

typedef struct TOKEN{
	char *data; 
	size_t len; 
	char *ptr;  
	char str[10240];
}TOKEN_T;

struct TOKEN * strinit();


char * strtoken(struct TOKEN * t, char * str, char * token);
#endif

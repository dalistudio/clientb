/*
 * Copyright (c) 2010-2014 Dali Wang, All Rights Reserved.
 *
 * CodeName: PiXiu 
 *
 *  Authors:
 *      Dali Wang <wangdali@qq.com>
 *
 *
 */
#include "stdafx.h"
#include "strtoken.h"

char * strtoken(TOKEN * t, char * str, char * token) {
	const char *tmp_str, *tmp_token;
	int token_len = strlen(token); 
	tmp_token = token;

	if (str == NULL) {
		str = t->ptr;
	}

	for (tmp_str = str; *tmp_str != '\0'; tmp_str++) { 
		if (*tmp_str == *tmp_token) { 
	if (token_len > 1) { 
			for (; *tmp_token != '\0'; tmp_token++) { 
		tmp_str++;
					tmp_token++;
					if (*tmp_str != *tmp_token) { 
			break;
					}
				} // for tmp_token
			}
			//
			t->ptr = (char*) (tmp_str + 1); 
			t->data = (char*) str; 
	t->len = (size_t) (t->ptr - t->data - token_len); 
			memset(t->str, 0, t->len + 1);
			memcpy(t->str, t->data, t->len);
			return t->str;
		}
	} // for
	t->data = t->ptr;
	if(t->data==NULL)
		t->len = 0;
	else
		t->len = (size_t) (tmp_str - str);
	t->ptr = NULL;
	memset(t->str, 0, t->len + 1);
	memcpy(t->str, t->data, t->len);
	t->data = NULL;
	return t->str;
}

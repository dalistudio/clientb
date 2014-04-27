#include "stdafx.h"
#include "uni2utf8.h"

char* EncodeToUTF8(const char* mbcsStr) 
{ 
 wchar_t*  wideStr; 
 char*   utf8Str; 
 int   charLen;

 charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0); 
 wideStr = (wchar_t*) malloc(sizeof(wchar_t)*charLen); 
 MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, wideStr, charLen);

 charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);

 utf8Str = (char*) malloc(charLen);

 WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

 free(wideStr); 
 return utf8Str;

} 

char* UTF8ToEncode(const char* mbcsStr)
{
 wchar_t*  wideStr; 
 char*   unicodeStr; 
 int   charLen;

 charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);   
 wideStr = (wchar_t*) malloc(sizeof(wchar_t)*charLen); 
 MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, wideStr, charLen); 

 charLen =WideCharToMultiByte(CP_ACP, 0, wideStr, -1, NULL, 0, NULL, NULL);  
 unicodeStr = (char*)malloc(charLen);
    WideCharToMultiByte(CP_ACP, 0, wideStr, -1, unicodeStr, charLen, NULL, NULL); 

 free(wideStr); 
 return unicodeStr;
}

#include "StringEscape.h"
#include <malloc.h>
#include <string.h>

char StringEscapeGetChar(const char*& p) {
	++p; char c;
	switch(*p) {
		case 'a': c = '\a'; break;
		case 'b': c = '\b'; break;
		case 'f': c = '\f'; break;
		case 'n': c = '\n'; break;
		case 'r': c = '\r'; break;
		case 't': c = '\t'; break;
		case 'v': c = '\v'; break;
		case '0': c = '\0'; break;
		default: c = *p; 
	}
	return c;
}

char* CharEscape(const char* str) {
	int len = strlen(str);
	char* ret = new char[len+1];
	char* ans = ret;
	for (const char* p = str; *p != 0; ++p, ++ans) {
		if (*p != '\\')
			*ans = *p; 
		else
			*ans = StringEscapeGetChar(p);
	}
	*ans = 0;
	return ret;
}

string StringEscape(const string& str) {
	char* c = CharEscape(str.c_str());
	string s = c;
	delete[] c;
	return s;
}
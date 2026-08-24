#include "wchartype.hpp"
int wsum(const wchar_t* s, int n){ int t=0; for(int i=0;i<n;i++) t+=(int)s[i]; return t; }
wchar_t wat(const wchar_t* s, int i){ return s[i]; }

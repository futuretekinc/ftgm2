#ifndef	STRING_UTILS_H_
#define	STRING_UTILS_H_

#include <string>

inline bool caseInsCharCompareN(char a, char b) 
{
	return(toupper(a) == toupper(b));
}

inline bool caseInsCompare(const std::string& s1, const std::string& s2) 
{
	return((s1.size() == s2.size()) && equal(s1.begin(), s1.end(), s2.begin(), caseInsCharCompareN));
}

#endif

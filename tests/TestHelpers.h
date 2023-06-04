#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <PAT.h>

enum MatchResult
{
	Success = 0,
	WhitespaceMismatch,
	TokenMismatch,
	ChildrenMismatch
};

MatchResult VerifyMatch(const PAT& a, const PAT& b);

#endif // TESTHELPERS_H

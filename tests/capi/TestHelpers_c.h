#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <pat.h>

enum MatchResult
{
	Success = 0,
	WhitespaceMismatch,
	TokenMismatch,
	ChildrenMismatch
};

enum MatchResult VerifyMatch(const struct PAT *a, const struct PAT *b);

#endif // TESTHELPERS_H

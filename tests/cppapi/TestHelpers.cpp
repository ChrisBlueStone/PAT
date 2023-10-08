#include "TestHelpers.h"

#include <iostream>
#include <numeric>
#include <assert.h>

using std::accumulate;
using std::cout;
using std::string;

int TestHelpers(int argc, char** argv)
{
	PAT a{"","", {{"", "root", {{}, {"    ", "child", {}}}}}};
	PAT b{"","", {{"", "root", {{}, {"    ", "child2", {}}}}}};
	PAT c{"","", {{"", "root", {{}, {"", "child", {}}}}}};
	PAT d{"","", {{"", "root", {{}, {"    ", "child", {}}, {}}}}};

	assert(VerifyMatch(a, a) == Success);
	assert(VerifyMatch(a, b) == TokenMismatch);
	assert(VerifyMatch(a, c) == WhitespaceMismatch);
	assert(VerifyMatch(a, d) == ChildrenMismatch);

	return 0;
}

MatchResult VerifyMatch(const PAT& a, const PAT& b)
{
	if (a.whitespace != b.whitespace)
	{
		cout << "Whitespace mismatch.\n"
			<< "Expected: \"" << b.whitespace << "\"\n"
			<< "Read:     \"" << a.whitespace << "\"\n";

		return WhitespaceMismatch;
	}
	else if (a.token != b.token)
	{
		cout << "Token mismatch.\n"
			<< "Expected: \"" << b.token << "\"\n"
			<< "Read:     \"" << a.token << "\"\n";

		return TokenMismatch;
	}
	else if (a.children.size() != b.children.size())
	{
		cout << "Children mismatch.\n"
			<< "Expected: " << accumulate(b.children.cbegin(), b.children.cend(), string(), [](auto&& a, auto&& b){ return (a.length() ? a + ", " : a) + "\"" + b.token + "\""; }) << '\n'
			<< "Read:     " << accumulate(a.children.cbegin(), a.children.cend(), string(), [](auto&& a, auto&& b){ return (a.length() ? a + ", " : a) + "\"" + b.token + "\""; }) << '\n';

		return ChildrenMismatch;
	}
	else
	{
		for (auto i = a.children.cbegin(), j = b.children.cbegin();
			i != a.children.cend() && j != b.children.cend();
			i++, j++)
		{
			MatchResult result = VerifyMatch(*i, *j);
			if (result != Success)
			{
				cout << "From parent \"" << a.token << "\"\n";
				return result;
			}
		}

		return Success;
	}
}

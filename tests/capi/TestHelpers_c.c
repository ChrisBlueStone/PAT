#include "TestHelpers_c.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static PAT a = {
	"", "", 1, (PAT[]){
		{
			"",  "root", 2, (PAT[]){
				{NULL, NULL, 0, NULL},
				{"    ", "child", 0, NULL}
			}
		}
	}
};

static PAT b = {
	"", "", 1, (PAT[]){
		{
			"",  "root", 2, (PAT[]){
				{NULL, NULL, 0, NULL},
				{"    ", "child2", 0, NULL}
			}
		}
	}
};

static PAT c = {
	"", "", 1, (PAT[]){
		{
			"",  "root", 2, (PAT[]){
				{NULL, NULL, 0, NULL},
				{"", "child", 0, NULL}
			}
		}
	}
};

static PAT d = {
	"", "", 1, (PAT[]){
		{
			"",  "root", 3, (PAT[]){
				{NULL, NULL, 0, NULL},
				{"    ", "child", 0, NULL},
				{NULL, NULL, 0, NULL}
			}
		}
	}
};

int TestHelpers_c(int argc, char **argv)
{
	assert(VerifyMatch(&a, &a) == Success);
	assert(VerifyMatch(&a, &b) == TokenMismatch);
	assert(VerifyMatch(&a, &c) == WhitespaceMismatch);
	assert(VerifyMatch(&a, &d) == ChildrenMismatch);

	return 0;
}

enum MatchResult VerifyMatch(const PAT *a, const PAT *b)
{
	size_t a_ws_length = a->whitespace ? strlen(a->whitespace) : 0;
	size_t b_ws_length = b->whitespace ? strlen(b->whitespace) : 0;
	size_t a_token_length = a->token ? strlen(a->token) : 0;
	size_t b_token_length = b->token ? strlen(b->token) : 0;
	if (a_ws_length != b_ws_length
		|| (
			a->whitespace
			&& b->whitespace
			&& 0 != memcmp(a->whitespace, b->whitespace, a_ws_length)))
	{
		fprintf(stderr,
			"Whitespace mismatch.\n"
			"Expected: \"%s\"\n"
			"Read:     \"%s\"\n",
			b->whitespace, a->whitespace);

		return WhitespaceMismatch;
	}
	else if (a_token_length != b_token_length
		|| (
			a->token
			&& b->token
			&& 0 != memcmp(a->token, b->token, a_token_length)))
	{
		fprintf(stderr,
			"Token mismatch.\n"
			"Expected: \"%s\"\n"
			"Read:     \"%s\"\n",
			b->token, a->token);

		return TokenMismatch;
	}
	else if (a->num_children != b->num_children)
	{
		fprintf(stderr,
			"Children mismatch.\n"
			"Expected: \"%d\"\n"
			"Read:     \"%d\"\n",
			b->num_children, a->num_children);

		return ChildrenMismatch;
	}
	else
	{
		for (int i = 0;
			i < a->num_children && i < b->num_children;
			i++)
		{
			enum MatchResult result = VerifyMatch(&a->children[i], &b->children[i]);
			if (result != Success)
			{
				fprintf(stderr, "From parent \"%s\"\n", a->token);
				return result;
			}
		}

		return Success;
	}
}

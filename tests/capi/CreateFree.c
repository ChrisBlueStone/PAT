#include "pat.h"
#include <assert.h>
#include <string.h>

void test_empty(void);
void test_whitespace(void);
void test_token(void);

int CreateFree(int argc, char **argv) {
	test_empty();
	test_whitespace();
	test_token();

	return 0;
}

void test_empty(void) {
	PAT *p = pat_create(NULL, NULL);
	assert(NULL != p);
	assert(NULL == p->whitespace);
	assert(NULL == p->token);
	pat_free(p);
}

void test_whitespace(void) {
	const char whitespace[] = "    ";

	PAT *p = pat_create(whitespace, NULL);
	assert(NULL != p);
	assert(NULL != p->whitespace);
	assert(0 == memcmp(whitespace, p->whitespace, sizeof whitespace));
	pat_free(p);
}

void test_token(void) {
	const char token[] = "token";

	PAT *p = pat_create(NULL, token);
	assert(NULL != p);
	assert(NULL != p->token);
	assert(0 == memcmp(token, p->token, sizeof token));
	pat_free(p);
}

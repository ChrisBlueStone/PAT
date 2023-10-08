#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "pat.h"

PAT* pat_create(
	const char *whitespace,
	const char *token)
{
	PAT *pat = malloc(sizeof(PAT));

	if (pat) {
		pat->whitespace = whitespace ? strdup(whitespace) : NULL;
		if (!whitespace || pat->whitespace) {
			pat->token = token ? strdup(token) : NULL;
			if (!token || pat->token) {
				pat->children = 0;
				pat->num_children = 0;
			} else {
				free(pat->whitespace);
				free(pat);
				pat = NULL;
			}
		} else {
			free(pat);
			pat = NULL;
		}
	}

	return pat;
}

void pat_clear(PAT *pat)
{
	for (unsigned int i = 0; i < pat->num_children; i++)
		pat_clear(&pat->children[i]);
	pat->num_children = 0;

	free(pat->whitespace);
	free(pat->token);
	free(pat->children);

	pat->whitespace = NULL;
	pat->token = NULL;
	pat->children = NULL;
}

void pat_free(PAT *pat) {
	pat_clear(pat);
	free(pat);
}

PAT* pat_add_child(PAT *pat, const char *token) {
	PAT *node = NULL;
	if (pat) {
		char *token_copy = strdup(token);

		if (NULL == token_copy) {
			errno = ENOMEM;
		} else {
			PAT *children = (pat->num_children == 0) ?
				malloc(sizeof(PAT))
				: realloc(pat->children, sizeof(PAT) * (pat->num_children + 1));

			if (NULL == children) {
				free(token_copy);
				errno = ENOMEM;
			} else {
				pat->children = children;

				node = &pat->children[pat->num_children++];
				node->whitespace = NULL;
				node->token = token_copy;
				node->num_children = 0;
				node->children = NULL;
			}
		}
	}

	return node;
}

PAT* pat_remove_child(PAT *pat, size_t child) {
	if (pat && pat->num_children > 0) {
		if (child == pat->num_children - 1) {
			pat->num_children--;
		} else if (child < pat->num_children - 1) {
			memmove(
				pat->children + child,
				pat->children + child + 1,
				sizeof(PAT) * (--pat->num_children - child));
		}
	}

	return pat;
}

int pat_try_get_value(const PAT *pat, const char **token) {
	if (pat->num_children > 0) {
		*token = pat->children[pat->num_children - 1].token;
	}

	return pat->num_children - 1;
}

const char* pat_get_value(const PAT *pat) {
	return pat->num_children > 0 ?
		pat->children[pat->num_children - 1].token
		: NULL;
}

const PAT* pat_first(const PAT *pat, const char *token) {
	if (pat) {
		for (size_t index = 0; index < pat->num_children; index++) {
			if (0 == strcmp(pat->children[index].token, token))
				return &pat->children[index];
		}
	}

	return NULL;
}

const PAT* pat_last(const PAT *pat, const char *token) {
	if (pat) {
		for (size_t index = pat->num_children - 1; index < pat->num_children; index--) {
			if (0 == strcmp(pat->children[index].token, token))
				return &pat->children[index];
		}
	}

	return NULL;
}

#include "patimport.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_whitespace(FILE *file, char **whitespace_ptr, size_t *length_ptr) {
	char *whitespace = *whitespace_ptr = NULL;
	size_t length = *length_ptr = 0;

	int c = fgetc(file);
	for (; c != EOF && isblank(c); c = fgetc(file)) {
		// Increment whitespace allocation in chunks of 16 bytes
		if ((length % 16) == 0) {
			if (NULL == whitespace) {
				whitespace = malloc(length + 17);
			} else {
				char *new_whitespace = realloc(whitespace, length + 17);
				if (NULL == new_whitespace) {
					free(whitespace);
					whitespace = NULL;
				} else {
					whitespace = new_whitespace;
				}
			}
		}

		if (NULL != whitespace)
			whitespace[length++] = c;
		else
			return errno = ENOMEM;
	}
	ungetc(c, file);

	if (whitespace && length)
		whitespace[length] = '\0';

	*whitespace_ptr = whitespace;
	*length_ptr = length;

	return 0;
}

static int read_token(FILE *file, char **token_ptr, size_t *length_ptr) {
	char *token = *token_ptr = NULL;
	size_t length = *length_ptr = 0;

	for (int c = fgetc(file); c != EOF && c != '\n'; c = fgetc(file)) {
		// Increment token allocation in chunks of 16 bytes
		if ((length % 16) == 0) {
			if (NULL == token) {
				token = malloc(length + 17);
			} else {
				char *new_token = realloc(token, length + 17);
				if (NULL == new_token) {
					free(token);
					token = NULL;
				} else {
					token = new_token;
				}
			}
		}

		if (NULL != token)
			token[length++] = c;
		else
			return errno = ENOMEM;
	}
	if (token && length)
		token[length] = '\0';

	*token_ptr = token;
	*length_ptr = length;

	return 0;
}

// TODO: Optimize for cache locality.
static int fimport_pat(PAT *pat, FILE *file) {
	// Clear errno so we can use it to check for io errors.
	int prevErrno = errno;
	errno = 0;

	while (!ferror(file) && !feof(file)) {
		char *whitespace = NULL;
		char *token = NULL;
		size_t whitespace_length = 0;
		size_t token_length = 0;

		if (!errno
			&& !read_whitespace(file, &whitespace, &whitespace_length)
			&& !read_token(file, &token, &token_length)
			&& whitespace_length || token_length)
		{
			// Search tree for parent.
			PAT *parent = pat;
			PAT *target = NULL;

			while (NULL == target) {
				if (parent->num_children > 0) {
					PAT *last_child = &parent->children[parent->num_children - 1];
					unsigned int last_child_ws_length = last_child->whitespace ? strlen(last_child->whitespace) : 0;
					if (whitespace_length > last_child_ws_length
						&& 0 == memcmp(whitespace, last_child->whitespace, last_child_ws_length))
					{
						parent = last_child;
					} else {
						target = parent;
					}
				} else if (parent != pat || whitespace_length == 0) {
					target = parent;
				} else {
					break;
				}
			}

			// Add node with appropriate parent
			if (NULL != target) {
				PAT *children = NULL == target->children ?
					malloc(sizeof(PAT))
					: realloc(target->children, sizeof(PAT) * (target->num_children + 1));

				if (NULL == children) {
					return errno = ENOMEM;
				} else {
					target->children = children;

					PAT *node = &target->children[target->num_children++];
					node->whitespace = whitespace;
					node->token = token;
					node->num_children = 0;
					node->children = NULL;
				}
			}
		}
	}

	int result = errno;

	// If we've made it through without errors, restore errno.
	if (0 == errno)
		errno = prevErrno;

	return result;
}

int pat_import(PAT **pat, const char *filename) {
	int result = EXIT_FAILURE;

	if (*pat = pat_create(NULL, NULL)) {
		FILE *file = fopen(filename, "r");
		if (file) {
			result = fimport_pat(*pat, file);

			if (0 != result) {
				pat_free(*pat);
				*pat = NULL;
			}

			fclose(file);
		}
	}
	
	return result;
}

#include "pat.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int TreeModification_c(int argc, char **argv) {
	PAT *root = pat_create(NULL, NULL);

	const char token[] = "child";

	// Test adding a child
	PAT *child1 = pat_add_child(NULL, token);
	{
		assert(NULL == child1);

		child1 = pat_add_child(root, token);
		assert(child1);
		assert(root->num_children == 1);
		assert(&root->children[0] == child1);
		assert(child1->token != token);
		assert(0 == strcmp(child1->token, token));

	}
	const char *childToken = child1->token;
	const char *childWhitespace = child1->whitespace = "    ";

	// Test adding a second child
	{
		PAT *child2 = pat_add_child(root, token);
		assert(child2);
		assert(root->num_children == 2);
		assert(&root->children[1] == child2);

		// Pointers of first child will be unchanged
		assert(root->children[0].token == childToken);
		assert(root->children[0].whitespace == childWhitespace);
	}

	// Test removing children
	{
		// Test removing from NULL
		assert(NULL == pat_remove_child(NULL, 0));
		assert(root->num_children == 2);

		// Test removing an invalid child.
		assert(root == pat_remove_child(root, 2));
		assert(root->num_children == 2);

		// Test removing a valid child
		assert(root == pat_remove_child(root, 0));
		assert(root->num_children == 1);
	}

	pat_clear(root);
	assert(0 == root->num_children);
	assert(NULL == root->children);
	assert(NULL == root->token);
	assert(NULL == root->whitespace);

	// Test chain adding
	{
		pat_add_child(pat_add_child(pat_add_child(root, "child"), "grandchild"), "great grandchild");
		pat_add_child(pat_add_child(&root->children[0], "grandchild2"), "great grandchild2");

		assert(1 == root->num_children);
		assert(NULL != root->children);
		assert(0 == strcmp(root->children[0].token, "child"));

		assert(2 == root->children[0].num_children);
		assert(NULL != root->children[0].children);
		assert(0 == strcmp(root->children[0].children[0].token, "grandchild"));
		assert(0 == strcmp(root->children[0].children[1].token, "grandchild2"));

		assert(1 == root->children[0].children[0].num_children);
		assert(NULL != root->children[0].children[0].children);
		assert(0 == strcmp(root->children[0].children[0].children[0].token, "great grandchild"));

		assert(1 == root->children[0].children[1].num_children);
		assert(NULL != root->children[0].children[1].children);
		assert(0 == strcmp(root->children[0].children[1].children[0].token, "great grandchild2"));
	}

	pat_free(root);

	return EXIT_SUCCESS;
}

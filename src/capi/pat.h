#ifndef PAT_H
#define PAT_H

#include <stddef.h>

typedef struct PAT {
	char *whitespace;
	char *token;
	unsigned int num_children;
	struct PAT *children;
} PAT;

/// \brief Allocates a PAT object in memory.
/// \param whitespace Pointer to a string to copy to the whitespace property.
/// \param token Pointer to the string to copy to the token property.
/// \return Returns a pointer to the nearly created PAT object, or NULL if allocation fails.
PAT* pat_create(
	const char *whitespace,
	const char *token);

/// \brief Deallocates the whitespace, token, and children members from a PAT object including its children.
/// \param pat Pointer to the PAT object to clear.
void pat_clear(PAT *pat);

/// \brief Deallocates a PAT object from memory.
/// \param pat The PAT object to deallocate.
void pat_free(PAT *pat);

/// \brief Adds a child node with a specific token. All existing pointers to child nodes will be invalidated, but pointers to their contents will not be invalidated.
/// To avoid a memory leak, the child should be removed directly with \see pat_remove_child or indirectly with \see pat_clear on the parent.
/// \param pat The node to which to add a child.
/// \param token The token to use in the new child node.
/// \return Returns a pointer to the newly added node. Returns `NULL` if \a pat is NULL or if a memory error occurs.
PAT* pat_add_child(PAT *pat, const char *token);

/// \brief Removes a child from \a pat based on its index. All existing pointers to child nodes will be invalidated, but pointers to their contents will not be invalidated.
/// \param pat The PAT object from which to remove a child.
/// \param index The index of the child to remove.
/// \return Returns a copy of \a pat.
PAT* pat_remove_child(PAT *pat, size_t index);

/// \brief Attempts to get the pointer to the token string of the last child of \a pat and stores the result in \a token.
/// If no children are present, `token` remains unchanged.
/// \param pat The node from which to fetch the last child.
/// \param token Pointer to the location where the token's address will be stored if found.
/// \return Returns the index of the child from which `token` was fetched. Returns -1 if no children exist.
int pat_try_get_value(const PAT *pat, const char **token);

/// \brief Attempts to get the pointer to the token string of the last child of `pat` and returns the result. Returns NULL if no children exist.
/// \param pat The node from which to fetch the last child.
/// \return Returns the address of the last child's token. Returns NULL if no children exist.
const char* pat_get_value(const PAT *pat);

/// \brief Finds and returns the address of the first child whose token matches \a token.
/// \param pat The node whose children are to be searched for \a token.
/// \param token The token used to match against \a pat's children's tokens.
/// \return Returns the address of the child whose token matches \token. If none exist, returns NULL.
const PAT* pat_first(const PAT *pat, const char *token);

/// \brief Finds and returns the address of the last child whose token matches \a token.
/// \param pat The node whose children are to be searched for \a token.
/// \param token The token used to match against \a pat's children's tokens.
/// \return Returns the address of the child whose token matches \token. If none exist, returns NULL.
const PAT* pat_last(const PAT *pat, const char *token);

#endif // PAT_H

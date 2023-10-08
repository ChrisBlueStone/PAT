# PAT C API
The PAT C API provides a struct that defines a PAt object and a few simple functions for loading, managing, and fetching data from PAT objects.

## `struct PAT`
### Members
#### `char *whitespace`
Pointer to a string containing the leading whitespace of the node. Populated by the `pat_import` function when parsing a PAT file, but otherwise is otherwise not a necessary property.

#### `char *token`
Pointer to a string containing the token value of the node.

#### `unsigned int num_children`
Stores the number of children allocated within the `PAT *children` property.

#### `PAT *children`
Pointer to an array of children. It should be NULL if no children are allocated.

## Functions
### `PAT* pat_create(const char *whitespace, const char *token)`
Allocates storage for a single PAT object and initializes its `whitespace` and `token` properties with copies of the strings that were passed in the parameters.

### `void pat_clear(PAT *pat)`
Deallocates the `whitespace`, `token`, and `children` members from `pat` including its children and all sub-children.

### `void pat_free(PAT *pat)`
Frees all the memory allocated by either `create_pat` or `import_pat`.

### `PAT* pat_add_child(PAT *pat, const char *token)`
Adds a new child node with a copy of the string `token` to `pat`.
Returns a pointer to the newly added child or NULL if allocation fails.
All pointers to any children are invalidated after calling this function, but pointers to their contents are still valid.

### `PAT* pat_remove_child(PAT *pat, size_t index)`
Removes a child from `pat` specified by the provided `index`.
Returns back the pointer to `pat`.
All pointers to any children are invalidated after calling this function, but pointers to their contents are still valid.

### `int pat_try_get_value(const PAT *pat, constr char** token)`
Attempts to get the token pointer of the last child of `pat` and stores the result in `token`. If no children are present, the value pointed to by `token` remains unchanged.
Returns the index of the child from which `token` was fetched or -1 if no children exist.

### `const char* pat_get_value(const PAT *pat)`
Attempts to get the pointer to the token string of the last child of `pat` and returns the result. Returns NULL if no children exist.

### `const PAT* pat_first(const PAT *pat, const char *token)`
Finds and returns the address of the first child of `pat` whose token matches `token`. Returns NULL if no matches are found.

### `const PAT* pat_last(const PAT *pat, const char *token)`
Finds and returns the address of the last child of `pat` whose token matches `token`. Returns NULL if no matches are found.

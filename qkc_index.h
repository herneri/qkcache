#ifndef QKC_INDEX_H
#define QKC_INDEX_H

#include <stdbool.h>

/* Load the last index entry data from the index file. */
struct index_entry *qkc_recent_index_entry(struct qkc_database *database);

/*
	Append an index entry. The data_size variable is needed to determine
	where the entry in the database ends.
*/
bool qkc_append_index_entry(struct qkc_database *database, const float data_size);

#endif /* QKC_INDEX_H */

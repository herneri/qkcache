/*
        NOTICE: This header file is intended to only be used by the qkcache engine
        and is not meant for use by the client program. Using these functions will
        certainly cause severe, irreversible errors if used incorrectly.
*/

#ifndef QKC_INDEX_H
#define QKC_INDEX_H

#include <stdbool.h>

enum qkc_index_return_codes {
	QKC_INDEX_OK, QKC_INDEX_EMPTY_DB,
	QKC_FAIL_LOAD_ENTRY, QKC_INDEX_FS_FAIL
};

/* Load the last index entry data from the index file. */
struct index_entry *qkc_recent_index_entry(struct qkc_database *database);

/*
	Append an index entry. The data_size variable is needed to determine
	where the entry in the database ends.
*/
bool qkc_append_index_entry(struct qkc_database *database, const float data_size);

/* Removes most recent index entry. Used for stack removal. */
int qkc_pop_index_entry(struct qkc_database *database);

#endif /* QKC_INDEX_H */

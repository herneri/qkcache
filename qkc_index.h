/*
	qkc_index.h: Index entry management, meant strictly for use with qkcache engine.

	Copyright (C) 2025  Eric Hernandez

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

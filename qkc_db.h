/*
	qkc_db.h: General cache database management.

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

#ifndef QkC_DB_H
#define QKC_DB_H

#include <stdio.h>

struct qkc_database {
	char *name;
	char *index_name;
	int entry_count;
	FILE *index_file;
	FILE *database_data;
};

struct index_entry {
	int entry_number;
	float start_bytes;
	float end_bytes;
};

enum QKC_DB_CODES {
	QKC_DB_OK, QKC_DB_CREATE_FAIL, QKC_DB_EXISTS
};

/* Opens an existing QKC database, which requires the database itself and its index file. */
struct qkc_database *qkc_open_database(const char *database_name);

/* Frees the memory associated with a current QKC database. */
void qkc_close_database(struct qkc_database *database_ptr);

/*
	Creates and initializes the binary files that
	are required for a QKC database, which consist of
	an actual data file and an index file.

	Start and end indicators are written to each file,
	while a count of entries is only written to the actual
	database file.
*/
int qkc_create_database(const char *database_name);

/* Adds one to the count of entries in the database file. */
void qkc_increment_count(struct qkc_database *database_ptr);

#endif /* QKC_DB_H */

/*
	qkc_stack.c: Stack-like CRUD operations.

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

#include "qkc_stack.h"
#include "qkc_db.h"
#include "qkc_index.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int qkc_stack_push(struct qkc_database *database, void *data, int data_size) {
	if (data == NULL) {
		return QKC_OP_NULL;
	} else if (qkc_append_index_entry(database, data_size) == false) {
		return QKC_OP_INDEX_ERR;
	}

	fseek(database->database_data, 0, SEEK_END);
	fwrite(data, data_size, 1, database->database_data);
	rewind(database->database_data);
	return QKC_OP_OK;
}

void *qkc_stack_peek(struct qkc_database *database) {
	void *retrieved_data = NULL;
	struct index_entry *recent_entry = NULL;
	int data_size = 0;

	if (database->entry_count == 0) {
		return NULL;
	} else if ((recent_entry = qkc_recent_index_entry(database)) == NULL) {
		return NULL;
	}

	data_size = recent_entry->end_bytes - recent_entry->start_bytes;

	retrieved_data = malloc(data_size);
	if (retrieved_data == NULL) {
		return NULL;
	}

	fseek(database->database_data, recent_entry->start_bytes - 1, SEEK_SET);
	fread(retrieved_data, data_size, 1, database->database_data);

	rewind(database->database_data);
	free(recent_entry);
	return retrieved_data;

}

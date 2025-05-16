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

int qkc_stack_push(struct qkc_database *database, void *data, int data_size) {
	if (data == NULL) {
		return QKC_OP_NULL;
	} else if (qkc_append_index_entry(database, data_size) == false) {
		return QKC_OP_INDEX_ERR;
	}

	fseek(database->database_data, 0, SEEK_END);
	fwrite(data, 1, data_size, database->database_data);	
	rewind(database->database_data);
	return QKC_OP_OK;
}

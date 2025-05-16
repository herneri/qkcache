/*
	qkc_stack.h: Stack-like CRUD operations.

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

#ifndef QKC_STACK_H
#define QKC_STACK_H

#include "qkc_db.h"

enum qkc_stack_error_codes {
	QKC_OP_OK, QKC_OP_NULL, QKC_OP_INDEX_ERR
};

/* Append data to a database. */
int qkc_stack_push(struct qkc_database *database, void *data, int data_size);

/*
	Retrieve most recent data from a database. Return data
	is heap-allocated and subsequently must be freed after use.
*/
void *qkc_stack_peek(struct qkc_database *database);

#endif /* QKC_STACK_H */

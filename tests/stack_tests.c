/*
	stack_tests.c: Unit tests for qkc_stack CRUD operations.

	Copyright (C) 2025  Eric Hernandez

	This file is part of qkcache.

	qkcache is free software: you can redistribute it and/or modify it under the terms
	of the GNU General Public License as published by the Free Software Foundation,
	either version 3 of the License, or (at your option) any later version.

	qkcache is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	qkcache. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <criterion/criterion.h>

#include "../qkc_db.h"
#include "../qkc_stack.h"

#include <stdio.h>
#include <unistd.h>

struct qkc_database *database = NULL;

void suite_init(void) {
	int result = qkc_create_database("stack.qkc");
	database = qkc_open_database("stack.qkc");
	return;
}

void suite_end(void) {
	qkc_close_database(database);
	unlink("stack.qkc");	
	unlink(".index_stack.qkc");	
	return;
}

TestSuite(stack_test, .init=suite_init, .fini=suite_end);

Test(stack_test, peek_empty) {
	void *result = qkc_stack_peek(database);
	cr_expect((int *)result == NULL, "qkc_stack_peek should return NULL for empty database");
}

Test(stack_test, push_empty) {
	int data = 5;
	int result = qkc_stack_push(database, &data, sizeof(int));

	cr_expect(result == QKC_OP_OK, "qkc_stack_push should return QKC_OP_OK for pushing on empty database");
}

Test(stack_test, peek_one) {
	int data = 5;
	qkc_stack_push(database, &data, sizeof(int));
	void *retrieved_data = qkc_stack_peek(database);

	cr_expect(retrieved_data != NULL, "qkc_stack_peek should return memory pointing to an integer, NULL is returned instead", *(int *)retrieved_data);
	cr_expect(*(int *)retrieved_data == data, "qkc_stack_peek should return memory pointing to an integer with the value 5, \n%d is returned instead", *(int *)retrieved_data);
	free(retrieved_data);
}

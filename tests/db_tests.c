/*
	db_tests.c: Unit tests for qkc_db functions that are used for database management.

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
#include <unistd.h>

void delete_database_files(void) {
	unlink("db.qkc");
	unlink(".index_db.qkc");
	return;
}

Test(db_test, db_create) {
	int result = qkc_create_database("db.qkc");
	cr_expect(result == QKC_DB_OK, "qkc_create_database should return QKC_DB_OK");
	delete_database_files();
}

Test(db_test, db_exists) {
	int result = qkc_create_database("db.qkc");
	result = qkc_create_database("db.qkc");

	cr_expect(result == QKC_DB_EXISTS, "qkc_create_database should return QKC_DB_EXISTS");
	delete_database_files();
}

Test(db_test, db_open_not_exist) {
	struct qkc_database *database = qkc_open_database("db.qkc");
	cr_expect(database == NULL, "qkc_open_database should return NULL when database doesn't exist");
}

Test(db_test, db_open_exist) {
	struct qkc_database correct_database = {"db.qkc", ".index_db.qkc", 0, NULL, NULL};
	struct qkc_database *database = NULL;

	qkc_create_database("db.qkc");
	database = qkc_open_database("db.qkc");

	cr_expect(database != NULL, "qkc_open_database should return a pointer to a qkc_database struct");

	cr_expect(strcmp(database->name, correct_database.name) == 0, "The loaded database's name should be %s,\ninstead it is %s", correct_database.name, database->name);
	cr_expect(strcmp(database->index_name, correct_database.index_name) == 0, "The loaded database's index name should be %s,\ninstead it is %s", correct_database.index_name, database->index_name);
	cr_expect(database->entry_count == correct_database.entry_count, "The loaded database should have 0 entries,\ninstead it has %d", database->entry_count);

	qkc_close_database(database);
	delete_database_files();
}

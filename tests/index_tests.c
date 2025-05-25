/*
	index_tests.c: Unit tests to ensure index integrity.

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
#include "../qkc_index.h"

#include <unistd.h>

struct qkc_database *database = NULL;

void suite_init(void) {
	qkc_create_database("index.qkc");
	database = qkc_open_database("index.qkc");
	return;
}

void suite_end(void) {
	qkc_close_database(database);
	unlink("index.qkc");
	unlink(".index_index.qkc");
	return;
}

TestSuite(index_test, .init=suite_init, .fini=suite_end);

Test(index_test, append_entry) {
	struct index_entry read_entry = {0, 0, 0};
	bool result = qkc_append_index_entry(database, sizeof(int));
	cr_expect(result == true, "qkc_append_index_entry should return true when appending an entry");

	fseek(database->index_file, sizeof(int), SEEK_SET);
	fread(&read_entry, sizeof(struct index_entry), 1, database->index_file);

	cr_expect(read_entry.entry_number == 0, "First index entry should have an entry number of 0 (it's %d)", read_entry.entry_number);
	cr_expect(read_entry.start_bytes == 4, "First index entry should start at the fourth byte (it's %d)", read_entry.start_bytes);
	cr_expect(read_entry.end_bytes == 8, "First index entry should end at the eight byte (it's %d)", read_entry.end_bytes);
	rewind(database->index_file);

	result = qkc_append_index_entry(database, sizeof(struct index_entry));
	cr_expect(result == true, "qkc_append_index_entry should return true when appending an entry");

	fseek(database->index_file, sizeof(int) + sizeof(struct index_entry), SEEK_SET);
	fread(&read_entry, sizeof(struct index_entry), 1, database->index_file);

	cr_expect(read_entry.entry_number == 1, "Second index entry should have an entry number of 1, (it's %d)", read_entry.entry_number);
	cr_expect(read_entry.start_bytes == 17, "Second index entry should start at the 17th byte (it's %d)", read_entry.start_bytes);
	cr_expect(read_entry.end_bytes == 29, "Second index entry should end at the 29th byte (it's %d)", read_entry.end_bytes);
	rewind(database->index_file);
}

Test(index_test, recent_entry_empty) {
	struct index_entry *loaded_entry = qkc_recent_index_entry(database);
	cr_expect(loaded_entry == NULL, "qkc_recent_index_entry is supposed to return NULL when there are no indexes");

	if (loaded_entry != NULL) {
		free(loaded_entry);
	}
}

Test(index_test, recent_entry) {
	struct index_entry *loaded_entry = NULL;
	struct index_entry correct_entry = {0, 4, 8};
	qkc_append_index_entry(database, sizeof(int));

	loaded_entry = qkc_recent_index_entry(database);
	cr_expect(loaded_entry != NULL, "qkc_recent_index_entry is supposed to return a pointer to an index_entry struct");

	cr_expect(loaded_entry->entry_number == correct_entry.entry_number, "Loaded entry's entry number should be 0 (it's %d)", loaded_entry->entry_number);
	cr_expect(loaded_entry->start_bytes == correct_entry.start_bytes, "Loaded entry's start byte should be 4 (it's %d)", loaded_entry->start_bytes);
	cr_expect(loaded_entry->end_bytes == correct_entry.end_bytes, "Loaded entry's end byte should be 8 (it's %d)", loaded_entry->end_bytes);
}

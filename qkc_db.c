/*
	qkc_db.c: General cache database management.

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

#include "qkc_db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int QKC_MAX_NAME_LEN = 255;

const int QKC_DB_HEADER = 0x12905;
const int QKC_INDEX_START = 0x24012;

struct qkc_database *qkc_open_database(const char *database_name) {
	struct qkc_database *opened_database = malloc(sizeof(struct qkc_database));
	char index_name[QKC_MAX_NAME_LEN + 7];
	int temp = 0;

	strcpy(index_name, ".index_");
	strcat(index_name, database_name);

	if (opened_database == NULL) {
		return NULL;
	}

	opened_database->name = (char *) database_name;
	opened_database->database_data = fopen(database_name, "rb+");
	opened_database->index_file = fopen(index_name, "rb+");

	if (opened_database->database_data == NULL) {
		fprintf(stderr, "qkcache: Failed to open database file\n");
		free(opened_database);
		return NULL;
	} else if (opened_database->index_file == NULL) {
		fprintf(stderr, "qkcache: Failed to open index file associated with %s\n", database_name);
		free(opened_database);
		return NULL;
	}

	fread(&temp, sizeof(int), 1, opened_database->database_data);
	if (temp != QKC_DB_HEADER) {
		fprintf(stderr, "qkcache: Database is not in the proper format\n");
		fclose(opened_database->database_data);
		free(opened_database);
		return NULL;
	}

	fread(&temp, sizeof(int), 1, opened_database->index_file);
	if (temp != QKC_INDEX_START) {
		fprintf(stderr, "qkcache: Database index file is not in the proper format\n");
		fclose(opened_database->index_file);
		free(opened_database);
		return NULL;
	}

	fread(&opened_database->entry_count, sizeof(int), 1, opened_database->database_data);

	rewind(opened_database->database_data);
	rewind(opened_database->index_file);
	return opened_database;
}

void qkc_close_database(struct qkc_database *database_ptr) {
	if (database_ptr == NULL) {
		return;
	}

	if (database_ptr->database_data != NULL) {
		fclose(database_ptr->database_data);
	}

	if (database_ptr->index_file != NULL) {
		fclose(database_ptr->index_file);
	}

	free(database_ptr);
	return;
}

int qkc_create_database(const char *database_name) {
	char index_name[QKC_MAX_NAME_LEN + 7];
	strcpy(index_name, ".index_");
	strcat(index_name, database_name);
	
	FILE *new_database = fopen(database_name, "rb");
	FILE *new_index = fopen(index_name, "rb");

	int entry_count = 0;

	if (new_database != NULL || new_index != NULL) {
		return QKC_DB_EXISTS;
	}

	new_database = fopen(database_name, "wb");
	new_index = fopen(index_name, "wb");
	if (new_database == NULL || new_index == NULL) {
		return QKC_DB_CREATE_FAIL;
	}

	/* Database file start indicator along with metadata */
	fwrite(&QKC_DB_HEADER, sizeof(int), 1, new_database);
	fwrite(&entry_count, sizeof(int), 1, new_database);

	fwrite(&QKC_INDEX_START, sizeof(int), 1, new_index);

	rewind(new_database);
	rewind(new_index);
	return QKC_DB_OK;
}

void qkc_increment_count(struct qkc_database *database_ptr) {
	database_ptr->entry_count++;

	fseek(database_ptr->database_data, sizeof(int), 1);
	fwrite(&database_ptr->entry_count, sizeof(int), 1, database_ptr->database_data);

	rewind(database_ptr->database_data);
	return;
}

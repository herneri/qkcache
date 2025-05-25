CC = gcc
SRC = qkc_db.c qkc_index.c qkc_stack.c
TEST_FLAGS = -lcriterion

db_test:
	$(CC) $(TEST_FLAGS) $(SRC) tests/db_tests.c -o tests/db_test
	cd tests
	./tests/db_test

index_test:
	$(CC) $(TEST_FLAGS) $(SRC) tests/index_tests.c -o tests/index_test
	cd tests
	./tests/index_test

stack_test:
	$(CC) $(TEST_FLAGS) $(SRC) tests/stack_tests.c -o tests/stack_test
	cd tests
	./tests/stack_test
	rm tests/stack.qkc tests/.index_stack.qkc

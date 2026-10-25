.PHONY:	all
all:	test

.PHONY: config
config:
	cmake -S . -B build

.PHONY: build
build:	config
	cmake --build build

.PHONY: test
test:	build
	./build/tests/tests

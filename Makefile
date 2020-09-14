all: page docs

.PHONY: page
page:
	cd site && hugo -D

.PHONY: docs
docs:
	@echo "Generating documentation"
	( \
		mkdir -p build; \
		cd build; \
		python3 -m venv docs_build_env; \
		cd ..; \
		. build/docs_build_env/bin/activate; pip3 install -r docs/requirements.txt; \
		cd docs; \
		make html; \
	)

menuconfig:
	( \
		mkdir -p bin; \
		cd bin; \
		srctree=.. BOARD_DIR=boards/** python3 ../scripts/kconfig/menuconfig.py Kconfig \
	)

.PHONY: build
build:
	( \
		cd bin; \
		make
	)

generate_tests:
	./run_test.sh --generate

.PHONY: test
test: generate_tests
	./run_test.sh

.PHONY: clean
clean:
	rm -rf build

all: page docs

.PHONY: page
page:
	cd site && hugo -D

.PHONY: docs
docs:
	@echo "Generating documentation"
	cd docs && make html

menuconfig:
	 scripts/kconfiglib.py $@

generate_tests:
	./run_test.sh --generate

.PHONY: test
test: generate_tests
	./run_test.sh

.PHONY: clean
clean:
	rm -rf build
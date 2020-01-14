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
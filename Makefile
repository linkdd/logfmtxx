DESTDIR := /usr/local

.PHONY: test
test:
	@make -C tests all

.PHONY: install
install:
	@cp -R include/ $(DESTDIR)

PREFIX = /usr/local
dbg = -g
opt = -O3
falloc = -DUSE_LIST_NODE_ALLOCATOR
ldpthread = -lpthread
shared = -shared
so_suffix = so
pic = -fPIC
obj = kdtree.o
lib_a = libkdtree.a
lib_so = libkdtree.$(so_suffix)

CC = gcc
CFLAGS = -pedantic -Wall -g -O3 $(pic) $(opt) $(dbg) $(falloc)
LDFLAGS = $(ldpthread)

.PHONY: all
all: $(lib_a) $(lib_so)

$(lib_a): $(obj)
	$(AR) rcs $@ $(obj)

$(lib_so): $(obj)
	$(CC) $(shared) -o $@ $(obj) $(LDFLAGS)

.PHONY: examples
examples:
	cd examples; $(MAKE)

.PHONY: clean
clean:
	rm -f $(obj) $(lib_a) $(lib_so)

.PHONY: install
install:
	cp kdtree.h $(PREFIX)/include/kdtree.h
	cp $(lib_so) $(PREFIX)/lib/$(lib_so)
	cp $(lib_a) $(PREFIX)/lib/$(lib_a)

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/include/kdtree.h
	rm -f $(PREFIX)/lib/$(lib_so)
	rm -f $(PREFIX)/lib/$(lib_a)

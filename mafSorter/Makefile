include ../inc/common.mk
SHELL:=/bin/bash
bin = ../bin
inc = ../inc
lib = ../lib
PROGS = mafSorter
dependencies = ${inc}/common.h ${inc}/sharedMaf.h ${lib}/common.c ${lib}/sharedMaf.c
objects = ${lib}/common.o ${lib}/sharedMaf.o ../external/CuTest.a src/buildVersion.o
testObjects = ./test/common.o ./test/sharedMaf.o ../external/CuTest.a test/buildVersion.o
sources = src/mafSorter.c

.PHONY: all clean test buildVersion

all: buildVersion $(foreach f,${PROGS}, ${bin}/$f)
buildVersion: src/buildVersion.c
src/buildVersion.c: ${sources} ${dependencies}
	@python ${lib}/createVersionSources.py

../lib/%.o: ../lib/%.c ../inc/%.h
	cd ../lib/ && make

${bin}/mafSorter: src/mafSorter.c ${dependencies} ${objects}
	mkdir -p $(dir $@)
	${cxx} ${cflags} -O3 $< ${objects} -o $@.tmp -lm
	mv $@.tmp $@

test/mafSorter: src/mafSorter.c ${dependencies} ${testObjects}
	mkdir -p $(dir $@)
	${cxx} ${cflags} -g -O0 $< ${testObjects} -o $@.tmp -lm
	mv $@.tmp $@

%.o: %.c %.h
	${cxx} -O3 -c ${cflags} $< -o $@.tmp
	mv $@.tmp $@
test/%.o: ${lib}/%.c ${inc}/%.h
	mkdir -p $(dir $@)
	${cxx} -g -O0 -c ${cflags} $< -o $@.tmp
	mv $@.tmp $@
test/%.o: src/%.c src/%.h
	mkdir -p $(dir $@)
	${cxx} -g -O0 -c ${cflags} $< -o $@.tmp
	mv $@.tmp $@

clean:
	rm -rf $(foreach f,${PROGS}, ${bin}/$f) src/*.o test/ src/buildVersion.c src/buildVersion.h

test: buildVersion test/mafSorter
	python2.7 src/test.mafSorter.py --verbose && rm -rf test/ && rmdir ./tempTestDir

../external/CuTest.a: ../external/CuTest.c ../external/CuTest.h
	${cxx} -c ${cflags} $<
	ar rc CuTest.a CuTest.o
	ranlib CuTest.a
	rm -f CuTest.o
	mv CuTest.a $@

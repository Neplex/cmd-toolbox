OBJS=echo.o shell.o ls.o mkdir.o rmdir.o link.o true.o false.o cat.o mv.o cp.o rm.o ln.o toolbox.o
BINS=bin/echo bin/shell bin/ls bin/mkdir bin/rmdir bin/link bin/true bin/false bin/cat bin/mv bin/cp bin/rm bin/ln
MANS=man/man1/echo.1 man/man1/false.1 man/man1/link.1 man/man1/ls.1 man/man1/mkdir.1 man/man1/rmdir.1 man/man1/shell.1 man/man1/true.1 man/man1/cat.1 man/man1/mv.1 man/man1/cp.1 man/man1/rm.1 man/man1/ln.1

CC=gcc
CFLAGS=-Wall -Werror -g #-ansi

all: $(BINS) $(MANS) $(TXT)

toolbox: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	-\rm -rf $(OBJS) toolbox bin/* man/man1/*

man/man1/%.1: %.t2t
	./txt2tags -i $? -o $@ -t man

bin/%: toolbox
	-rm $@
	ln $? $@

toolbox.o: toolbox.h
echo.o: toolbox.h
shell.o: toolbox.h
ls.o: toolbox.h
mkdir.o: toolbox.h
rmdir.o: toolbox.h
mv.o : toolbox.h
cp.o : toolbox.h
rm.o : toolbox.h
link.o: toolbox.h
ln.o : toolbox.h
true.o: toolbox.h
false.o: toolbox.h
cat.o: toolbox.h

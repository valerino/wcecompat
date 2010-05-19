!INCLUDE <wceconfig.mak>

CFLAGS=/W3 /WX /Ox /O2 /Ob2 /GF /Gy /nologo $(WCETARGETDEFS) -Zc:wchar_t- -DUNICODE -D_UNICODE -DWIN32 -DWIN32_LEAN_AND_MEAN -Iinclude -D_WINDLL -D_DLL /Foobj/ -D_MSC_VER=1200

SRC =							\
	src/args.cpp				\
	src/assert.cpp				\
	src/ChildData.cpp			\
	src/env.cpp					\
	src/errno.cpp				\
	src/io.c						\
	src/pipe.cpp				\
	src/process.c				\
	src/signal.c				\
#	src/redir.cpp				\
	src/stat.cpp				\
	src/stdio_extras.cpp		\
	src/stdlib_extras.cpp	\
	src/string_extras.cpp		\
	src/time.c				\
	src/timeb.cpp				\
	src/ts_string.cpp			\
	src/winmain.cpp				\
	src/winsock_extras.cpp	\
	src/utime.cpp	\
	src/direct.c	\
	src/dirent.c  \
	src/conio.c	\

!IF "$(WCEVERSION)"=="211"
SRC =							\
	$(SRC)						\
	src/wce211_ctype.c			\
	src/wce211_string.c
!ENDIF

OBJS = $(SRC:src=obj)
OBJS = $(OBJS:.cpp=.obj)
OBJS = $(OBJS:.c=.obj)

{src}.c{obj}.obj:
	@$(CC) $(CFLAGS) -c $<

{src}.cpp{obj}.obj:
	@$(CC) $(CFLAGS) -c $<

all: lib\wcecompat.lib lib\wcecompatex.lib
#	echo $(OBJS)

obj:
	@md obj 2> NUL

lib:
	@md lib 2> NUL

$(OBJS): makefile obj

clean:
	@echo Deleting target libraries...
	@del lib\*.lib
	@echo Deleting object files...
	@del obj\*.obj

lib\wcecompat.lib: lib $(OBJS) makefile
	@lib /nologo /out:lib\wcecompat.lib $(LFLAGS) $(OBJS)

lib\wcecompatex.lib: lib $(OBJS) makefile
	@lib /nologo /out:lib\wcecompatex.lib $(OBJS)

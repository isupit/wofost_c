OBJS	 = afgen.o assim.o astro.o cropdata.o develop.o fillvar.o lai.o lai2.o leaves.o limit.o max.o min.o meteodata.o penman.o sitedata.o soildata.o watfd.o wofost.o clean.o
EXECUTABLE = wofost
CC       = clang
CFLAGS   = -Wall -g

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) -lm


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJS) $(EXECUTABLE) 

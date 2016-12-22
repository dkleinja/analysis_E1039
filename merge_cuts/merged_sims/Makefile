LDLIBS		+= $(shell mysql_config --libs) -lmysqlclient
CPPFLAGS	+= $(shell mysql_config --include)

SRCS		+= Merged.cc

OBJS		:= $(SRCS:.cc=.o)

.cc.o:
		g++ $(CPPFLAGS) -c $< -o $@

all:		$(OBJS)
		g++ $(OBJS) $(LDLIBS) -o Merged

clean:
		rm -f Merged
		rm -f $(OBJS)

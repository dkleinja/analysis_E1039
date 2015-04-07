LDLIBS		+= $(shell mysql_config --libs) -lmysqlclient
CPPFLAGS	+= $(shell mysql_config --include)

SRCS		+= Merged_qiesum.cc

OBJS		:= $(SRCS:.cc=.o)

.cc.o:
		g++ $(CPPFLAGS) -c $< -o $@

all:		$(OBJS)
		g++ $(OBJS) $(LDLIBS) -o Merged_qiesum

clean:
		rm -f Merged_qiesum
		rm -f $(OBJS)

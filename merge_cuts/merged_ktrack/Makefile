LDLIBS		+= $(shell mysql_config --libs) -lmysqlclient
CPPFLAGS	+= $(shell mysql_config --include)

SRCS		+= Merged_ktrack.cc

OBJS		:= $(SRCS:.cc=.o)

.cc.o:
		g++ $(CPPFLAGS) -c $< -o $@

all:		$(OBJS)
		g++ $(OBJS) $(LDLIBS) -o Merged_ktrack

clean:
		rm -f Merged_ktrack
		rm -f $(OBJS)

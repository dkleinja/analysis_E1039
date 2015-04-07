LDLIBS		+= $(shell mysql_config --libs) -lmysqlclient
CPPFLAGS	+= $(shell mysql_config --include)

SRCS		+= Merged_beam.cc

OBJS		:= $(SRCS:.cc=.o)

.cc.o:
		g++ $(CPPFLAGS) -c $< -o $@

all:		$(OBJS)
		g++ $(OBJS) $(LDLIBS) -o Merged_beam

clean:
		rm -f Merged_beam
		rm -f $(OBJS)

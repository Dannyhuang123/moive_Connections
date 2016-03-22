##Lingfei Lu A91046735
##Junxi Li A91105747

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
  CPPFLAGS += -O3
  LDFLAGS += -O3
else
  CPPFLAGS += -g
  LDFLAGS += -g
endif

#all: pathfinder moviespan
all: pathfinder actorconnections extension


# include what ever source code *.hpp files pathfinder relies on (these are merely the ones thst were used in the solution)
extension: ActorGraph.o
pathfinder: ActorGraph.o
actorconnections: ActorGraph.o
ActorGraph.o: ActorGraph.hpp 
clean:
	rm -f pathfinder actorconnections extension *.o core*


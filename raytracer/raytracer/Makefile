COMMONFLAGS=-I. -std=c++14 -pthread -DELPP_CUSTOM_COUT=std::cerr -DELPP_THREAD_SAFE
# NO_SCRIPTING_FLAGS=-DEXCLUDE_SCRIPTING

OBJSDIR = .objectcache
SRC := $(shell find -type f -name "*.cpp" -printf '%P ')
OBJS := $(patsubst %.cpp, $(OBJSDIR)/%.o, $(SRC))
DEPS := $(OBJS:.o=.d)

all: rt-debug

rt-debug rt-test: CFLAGS += $(COMMONFLAGS) -g -Wall -Wextra -O0
rt-release: CFLAGS += $(COMMONFLAGS) -DNDEBUG -O3 -s
rt-test: CFLAGS += -DTEST_BUILD

rt-debug rt-release rt-test: $(OBJS)
	$(CXX) $(COMMONFLAGS) -o$@ $(OBJS) -ldl

-include $(DEPS)

$(OBJSDIR)/%.o: %.cpp
	@$(create_cache_dirs)
	$(CXX) $(CFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -o $@ -c $<

# Creates clone of directory structure in cache needed for creation of .d and .o files
create_cache_dirs = [ -d $(OBJSDIR) ] || find -type d | sed "s/\./$(OBJSDIR)/"  | xargs mkdir -p

%.chai: null
	rm -rf out
	mkdir out
	./rt-debug -s $@ | wif extract -o out/f%d.png

r: chai-scripts/mesh.chai

clean:
	rm -rf $(OBJSDIR)
	$(RM) rt-debug rt-release rt-test
 
.PHONY: clean null

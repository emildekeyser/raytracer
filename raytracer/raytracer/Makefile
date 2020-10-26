CC=g++
COMMONFLAGS=-I. -std=c++14 -pthread -DELPP_CUSTOM_OUT=std::cerr -DELPP_THREAD_SAFE
DEBUG_FLAGS=$(COMMONFLAGS) -g -Wall -Wextra -O0
RELEASE_FLAGS=$(COMMONFLAGS) -DNDEBUG -O3 -s -ort-release
TEST_FLAGS=$(DEBUG_FLAGS) -DTEST_BUILD -ort-test
NO_SCRIPTING_FLAGS=-DEXCLUDE_SCRIPTING

OBJSDIR = .objectcache
SRC := $(shell find -type f -name "*.cpp" -not -path "*ccls-cache*" -printf '%P ')
OBJS := $(patsubst %.cpp, $(OBJSDIR)/%.o, $(SRC))
DEPS := $(OBJS:.o=.d)

# all: debug_build test_build

debug_build: $(OBJS)
	$(CC) -std=c++14 -pthread -DELPP_CUSTOM_OUT=std::cerr -DELPP_THREAD_SAFE -ort-debug $(OBJS) -ldl

release_build:
	$(CC) $(RELEASE_FLAGS) $(OBJS) -ldl

test_build:
	$(CC) $(TEST_FLAGS) $(OBJS) -ldl

debug_no_scripting_build:
	$(CC) $(DEBUG_FLAGS) $(NO_SCRIPTING_FLAGS) $(OBJS) -ldl

release_no_scripting_build:
	$(CC) $(RELEASE_FLAGS) $(NO_SCRIPTING_FLAGS) $(OBJS) -ldl

test_no_scripting_build:
	$(CC) $(TEST_FLAGS) $(NO_SCRIPTING_FLAGS) $(OBJS) -ldl

d: debug_build
r: release_build
t: test_build
dn: debug_no_scripting_build
rn: release_no_scripting_build
tn: test_no_scripting_build

-include $(DEPS)

$(OBJSDIR)/%.o: %.cpp
	@$(create_cache_dirs)
	g++ $(DEBUG_FLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -o $@ -c $<

# Creates clone of directory structure in cache needed for neat creation ov .d and .o files
create_cache_dirs = [ -d $(OBJSDIR) ] || find -type d -not -path "*ccls-cache*" | sed "s/\./$(OBJSDIR)/"  | xargs mkdir -p

render:
	rm -rf out
	mkdir out
	./rt-debug -s chai-scripts/triangle.chai | tee output.txt | grep -v INFO | wif extract -o out/f%d.png

compiledb:
	compiledb make

clean:
	rm -rf $(OBJSDIR)
	$(RM) rt-debug

.PHONY: clean render compiledb

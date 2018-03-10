CC = g++
CFLAGS = -I$(INCLUDE_DIR) $(THIRD_PARTY_INCLUDE) -L$(THIRD_PARTY_LIB) -lpistache -std=gnu++11
OUTPUT_TARGET = app

INCLUDE_DIR = ../include

THIRD_PARTY_INCLUDE_DIR = ../cget/include
_THIRD_PARTY_INCLUDE = nlohmann pistache
THIRD_PARTY_INCLUDE = $(patsubst %, -I$(THIRD_PARTY_INCLUDE_DIR)/%, $(_THIRD_PARTY_INCLUDE))
THIRD_PARTY_LIB = ../cget/lib

OBJS_DIR = objects
LIBS = -pthread

_THIRD_PARTY_DEPS = nlohmann/json.hpp
THIRD_PARTY_DEPS = $(patsubst %, $(THIRD_PARTY_INCLUDE)/%, $(THIRD_PARTY_DEPS))

_DEPS = RequestHandler.h
DEPS = $(patsubst %, $(INCLUDE_DIR)/%, $(_DEPS))

_OBJS = RequestHandler.o main.o
OBJS = $(patsubst %, $(OBJS_DIR)/%, $(_OBJS))

$(OUTPUT_TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJS_DIR)/%.o: %.cpp $(DEPS) $(THIRD_PARTY)
	$(CC) -c -o $@ $< $(CFLAGS)
	
.PHONY: clean

clean: 
	rm -f $(OBJS_DIR)/*.o $(OUTPUT_TARGET) *~ core $(INCLUDE_DIR)/*~


# notes:
# 1) -c makes .o object files which are unlinked compiled files. g++ compiles and then links, but only links with already compiled files
# 2) -o names the output target file
# 3) $ dereferences the variable after it
# 4) @ is the name before the :
# 5) < is the first dependency which comes after the :
# 6) ^ is everything after the :
# 7) % is a wildcard
# 8) patsubst is patter substitution with notation $(patsubst pattern,replacement,text)
# 9) clean cleans all object files, any file ending in ~ which comes from the text editor creating a copy, core is the name of a file created
# from C seg faults where the error is dumped in to a file called core.

COMPILER = clang++
OUTPUT_NAME = main
OUTPUT_DIRECTORY = bin
COMPILER_FLAGS = -std=c++1z -c -O3
OBJS = data_member.o errors.o read_stream.o main.o

clean: build
	rm $(OBJS)

build: $(OBJS)
	$(COMPILER) $(OBJS) -o $(OUTPUT_DIRECTORY)/$(OUTPUT_NAME)

clear:
	\rm *.o $(OUTPUT_DIRECTORY)/$(OUTPUT_NAME)

data_member.o: src/data_member.cpp src/data_member.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/data_member.cpp

errors.o: src/errors.cpp src/errors.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/errors.cpp

read_stream.o: src/read_stream.cpp src/read_stream.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/read_stream.cpp

main.o: src/main.cpp src/read_stream.hpp src/data_member.hpp src/errors.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/main.cpp

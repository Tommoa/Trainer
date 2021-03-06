COMPILER = clang++
OUTPUT_NAME = main
OUTPUT_DIRECTORY = bin
COMPILER_FLAGS = -std=c++1z -c -O3
OBJS = data_member.o errors.o interpreter.o variables.o porter.o main.o

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

interpreter.o: src/interpreter.cpp src/interpreter.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/interpreter.cpp

variables.o: src/interpreter.hpp src/variables.hpp src/variables.cpp src/Snippets/defer.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/variables.cpp

porter.o: src/interpreter.hpp src/Snippets/defer.hpp src/errors.hpp src/data_member.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/porter.cpp

main.o: src/main.cpp src/interpreter.hpp src/data_member.hpp src/errors.hpp
	$(COMPILER) $(COMPILER_FLAGS) src/main.cpp

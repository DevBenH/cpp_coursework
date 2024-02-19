CC := g++
CXX_STANDARD := 20
BINARIES := test-suite

all: ${BINARIES}

library.o: library.cpp library.h
	${CC} -O3 -std=c++${CXX_STANDARD} -c $< -o $@ -Wall -Wextra

test-suite.o: test-suite.cpp library.h catch.h
	${CC} -std=c++${CXX_STANDARD} -c $< -o $@ -w

catch.o: catch.cpp catch.h
	${CC} -std=c++${CXX_STANDARD} -c $< -o $@ -w

%.o: %.cpp %.h
	${CC} -std=c++${CXX_STANDARD} -c $< -o $@

%.o: %.cpp
	${CC} -std=c++${CXX_STANDARD} -c $< -o $@ 

test-suite: catch.o library.o test-suite.o
	${CC} -std=c++${CXX_STANDARD} $^ -o $@ -Wall -Wextra

analyze:
	${CC} -O3 -std=c++${CXX_STANDARD} -c library.cpp -Wall -Wextra -fanalyzer -Wanalyzer-too-complex -Wno-analyzer-possible-null-argument

guidelines:
	${TIDY} -checks=cppcoreguidelines-*,clang-analyzer-* library.cpp -- std=c++${CXX_STANDARD}

test: test-suite
	./test-suite

clean:
	rm -rf *.o ${BINARIES} *~

CXX=g++
CXXFLAGS=-Wall -Werror -march=native
LIBS=-lcrypto
EXE=scrypt

# $(EXE): $(EXE).cpp
# 	$(CXX) $(CXXFLAGS) $(EXE).cpp $(LIBS) -o $(EXE)

test: core.cpp test.cpp utils.cpp
	$(CXX) $(CXXFLAGS) utils.cpp core.cpp test.cpp $(LIBS) -o test
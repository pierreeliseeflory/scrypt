CXX=g++
CXXFLAGS=-Wall -Werror -march=native
LIBS=-lcrypto
EXE=scrypt

$(EXE): $(EXE).cpp
	$(CXX) $(CXXFLAGS) $(EXE).cpp $(LIBS) -o $(EXE)

test: $(EXE).cpp $(EXE)_test.cpp
	$(CXX) $(CXXFLAGS) $(EXE).cpp $(EXE)_test.cpp $(LIBS) -o test
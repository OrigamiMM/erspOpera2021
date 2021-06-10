CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++11
OUTFILES=MatricesTest

all: MatricesTest

#AdjTest: AdjMatrixTest.cc AdjMatrix.cc AdjMatrix.h
#	$(CXX) $(CXXFLAGS) -o AdjTest AdjMatrixTest.cc AdjMatrix.cc AdjMatrix.h

MatricesTest: AdjMatricesTest.cc AdjMatrices.cc AdjMatrices.h
	$(CXX) $(CXXFLAGS) -o MatricesTest AdjMatricesTest.cc AdjMatrices.cc AdjMatrices.h

clean:
	$(RM) $(OUTFILES) *.o 
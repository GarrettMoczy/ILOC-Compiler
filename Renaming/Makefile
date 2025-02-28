CXX = g++
CXXFLAGS = -std=c++20 -Wall -I./includes -O3 -flto

# Specify source directory
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SOURCES:$(SRCDIR)/%.cpp=%.o)

EXEC = 434alloc

build: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ)
	rm -f ${OBJ}

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(EXEC) *.tar

.PHONY: clean
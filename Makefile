# # A Sample Makefile
# # If you wish to use a Makefile in your project, you will most likely
# # need to edit this one. 
# # For example, if you do not have a file named utilities.h or one
# # named utilities.cpp, just remove them from this Makefile.
# # If you use a file named dsexceptions.h, as the book does, and avl.cpp
# # depends on it, then add its name to the line for avl.o below.

# SRCS       := tree.cpp tree_collection.cpp AvlTree.cpp tree_species.cpp main.cpp
# OBJS       := $(patsubst %.cpp, %.o, $(SRCS))
# EXEC       := project1 
# CXX        := g++
# CXXFLAGS   += -Wall -g -std=c++11
# LIBS       := -lm

# $(EXEC): $(OBJS)
# 	$(CXX) $(CXXFLAGS) $(LIBS) -o build/$@  $(OBJS) 

# .PHONY: clean cleanall
# cleanall: clean
# 	$(RM) $(EXEC)
# clean:
# 	$(RM) $(OBJS)

# tree_species.h:    __tree_species.h
# tree_collection.h: __tree_collection.h
# AvlTree.o:         AvlTree.h dsexceptions.h 
# tree_collection.o: tree.h tree_species.h
# tree.o:            tree.h 
# tree_species.o:    tree_species.h


CXX := g++
CXXFLAGS := -Wall -g -std=c++11
LIBS := -lm
OBJS = tree.o tree_collection.o AvlTree.o tree_species.o main.o

main : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o : main.cpp command.cpp tree_collection.h tree.h

tree.o : tree.cpp tree.h

tree_collection.o : __tree_collection.h tree_collection.cpp tree_collection.h AvlTree.h tree.h tree_species.h

AvlTree.o : AvlTree.h

tree_species.o : __tree_species.h tree_species.cpp tree_species.h

.PHONY: clean

clean:
	rm -rf $(OBJS) main
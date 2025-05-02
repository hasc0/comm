CXX := g++
CXXFLAGS := -std=c++23 -g -Wall -Wextra

SRCDIR := src
OBJDIR := obj

CSRC := $(addprefix $(SRCDIR)/,client.cpp)
COBJ := $(addprefix $(OBJDIR)/,client.o)
CBIN := client

SSRC := $(addprefix $(SRCDIR)/,server.cpp)
SOBJ := $(addprefix $(OBJDIR)/,server.o)
SBIN := server

SHRHDR := $(addprefix $(SRCDIR)/,shared.hpp)
SHRSRC := $(addprefix $(SRCDIR)/,shared.cpp)
SHROBJ := $(addprefix $(OBJDIR)/,shared.o)

all: $(CBIN) $(SBIN)

$(CBIN): $(COBJ) $(SHROBJ)
	$(CXX) $(CXXFLAGS) -o $(CBIN) $(COBJ) $(SHROBJ)

$(COBJ): $(CSRC) $(SHRHDR) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(COBJ)

$(SBIN): $(SOBJ) $(SHROBJ)
	$(CXX) $(CXXFLAGS) -o $(SBIN) $(SOBJ) $(SHROBJ)

$(SOBJ): $(SSRC) $(SHRHDR) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(SOBJ)

$(SHROBJ): $(SHRSRC) $(SHRHDR) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(SHROBJ)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(CBIN) $(SBIN) $(OBJDIR)

.PHONY: all clean


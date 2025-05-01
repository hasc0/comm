CXX := g++
CXXFLAGS := -std=c++23 -g -Wall -Wextra

SRCDIR := src
OBJDIR := obj

CSRC := $(addprefix $(SRCDIR)/,client.cpp)
SSRC := $(addprefix $(SRCDIR)/,server.cpp)

COBJS := $(addprefix $(OBJDIR)/,client.o)
SOBJS := $(addprefix $(OBJDIR)/,server.o)

CBIN := client
SBIN := server

all: $(CBIN) $(SBIN)

$(CBIN): $(COBJS)
	$(CXX) $(CXXFLAGS) -o $(CBIN) $(COBJS)

$(COBJS): $(CSRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(COBJS)

$(SBIN): $(SOBJS)
	$(CXX) $(CXXFLAGS) -o $(SBIN) $(SOBJS)

$(SOBJS): $(SSRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(SOBJS)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(CBIN) $(SBIN) $(OBJDIR)

.PHONY: all clean


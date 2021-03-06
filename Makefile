CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := pi
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++17 -g -Wall -O3 -march=native -frename-registers -fopenmp -D_GLIBCXX_PARALLEL
LIB := -lgmpxx -lgmp -lpthread
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking ..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Building ..."
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning ..."; 
	@echo " $(RM) -r $(BUILDDIR); rm $(TARGET)"; $(RM) -r $(BUILDDIR); rm $(TARGET)

.PHONY: clean

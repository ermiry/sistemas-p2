TARGET      := p2

TARGETDIR   := bin

all: directories $(TARGET)

directories:
	@mkdir -p $(TARGETDIR)

$(TARGET):
	gcc main.c -o bin/p2

clean:
	@$(RM) -rf $(TARGETDIR)

.PHONY: all clean
TARGET := bouncing
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -I include
MODULES := src/bouncing.o src/animation.o

$(TARGET): $(MODULES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(MODULES)

.PHONY: clean

SRC = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(SRC))

NEW:$(TARGET)
	@echo success

%:%.c
	gcc -g $< -o $@

clean:
	-rm $(TARGET)

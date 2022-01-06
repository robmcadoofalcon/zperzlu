clean:
	rm -f puzzler

build: clean
	python make_table.py > table.c
	gcc main.c table.c -o puzzler -DNDEBUG -O2

benchmark: clean build
	./puzzler large_input.txt | pv > /dev/null

all: benchmark

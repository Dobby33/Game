.PHONY: all build utils

all:
	gcc -o out/a.out src/main.c -lm
	out/a.out

build:
	chmod +x build.sh
	./build.sh -f

utils:
	chmod +x utils/ts/build.sh
	./utils/ts/build.sh

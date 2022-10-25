# Needs to be named the same as your source code file!
obj-m += hid-compaq-sdm4540ul.o

all:
	make CFLAGS="-O0 -g3 -Wall -Wextra -Wpedantic -Werror" -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
lib = "lib/libws2811.a"

all: ${lib} bin/rainbow bin/rainblo bin/monohorn

${lib}:
	gcc -o ws2811.o -c -g -O2 -Wall -Werror ws2811.c -fPIC
	gcc -o rpihw.o -c -g -O2 -Wall -Werror rpihw.c -fPIC
	gcc -o pwm.o -c -g -O2 -Wall -Werror pwm.c -fPIC
	gcc -o dma.o -c -g -O2 -Wall -Werror dma.c -fPIC
	gcc -o mailbox.o -c -g -O2 -Wall -Werror mailbox.c -fPIC
	gcc -o board_info.o -c -g -O2 -Wall -Werror board_info.c -fPIC
	ar rc $@ ws2811.o pwm.o dma.o rpihw.o mailbox.o board_info.o
	ranlib $@

bin/rainbow: ${lib}
	gcc -o $@ -O2 -Wall -Werror $(notdir $@).c matrix.c ${lib}

bin/rainblo: ${lib}
	gcc -o $@ -O2 -Wall -Werror $(notdir $@).c ${lib} -llo

bin/monohorn: ${lib}
	gcc -o $@ -O2 -Wall -Werror $(notdir $@).c matrix.c ${lib} -llo

clean:
	-rm ws2811.o pwm.o dma.o board_info.o mailbox.o rpihw.o
	-rm ${lib}
	-rm bin/rainbow
	-rm bin/monohorn

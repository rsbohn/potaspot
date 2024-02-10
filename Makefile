CC65_HOME=/usr/share/cc65
CC65=cc65
CA65=ca65
LD65=ld65
CC65_LIB=${CC65_HOME}/lib/
CC65_CFG=${CC65_HOME}/cfg/

potaspot.xex: main.o nsio.o sio.o
	${LD65} -t atari \
		--lib-path ${CC65_LIB} \
		-o potaspot.xex \
		-m potaspot.map \
		main.o nsio.o sio.o

main.o: main.s
	${CA65} -t atari main.s -o main.o

main.s: main.c
	${CC65} -t atari main.c -o main.s
	
nsio.o: nsio.s
	${CA65} -t atari nsio.s -o nsio.o

nsio.s: nsio.c nsio.h sio.h
	${CC65} -t atari nsio.c -o nsio.s
		
sio.o:	sio.s
	${CA65} -I "${CA65_ASMINC}" -t atari sio.s -o sio.o

dist.tgz: main.c nsio.c nsio.h sio.h sio.s Makefile replit.nix
	tar cfz dist.tgz $^

clean:
	-rm dist.tgz
	-rm potaspot.xex
	-rm *.o
	-rm main.s
	-rm nsio.s

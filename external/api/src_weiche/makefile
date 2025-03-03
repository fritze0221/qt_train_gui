
CC		= gcc

SW_PORTS	= adc dpwm dumb-input magnet mc mf pwm smart-input sp xpm
SW_PORTS_SRC	= $(SW_PORTS:%=%.c)
SW_PORTS_OBJ	= $(SW_PORTS:%=%.o)

USR		= usr notify
USR_SRC		= $(USR:%=%.c)
USR_OBJ		= $(USR:%=%.o)

TOOLS		= tics stics ramp
TOOLS_SRC	= $(TOOLS:%=%.c)
TOOLS_OBJ	= $(TOOLS:%=%.o)

TEST_PROGS 	= $(SW_PORTS:%=test-%)

all:	$(TEST_PROGS)

z:
	echo $(SW_PORTS)
	echo $(SW_PORTS_SRC)
	echo $(TEST_PROGS)

switch:	switch.o hw-ports.o sw-ports.o tics.o stics.o pwm.o mf.o io-board.o
	gcc -o $@ switch.o hw-ports.o sw-ports.o tics.o stics.o pwm.o mf.o \
		  io-board.o

switch.o: switch.c tics.h stics.h pwm.h mf.h io-board.h

$(SW_PORTS_OBJ) $(TOOLS_OBJ)	: %.o: %.c %.h

$(SW_PORTS_OBJ) $(TEST_PROGS)	: hw-ports.h sw-ports.h tics.h 

sw-ports.o: sw-ports.h sw-ports.cf sw-ports.c
hw-ports.o: hw-ports.h hw-ports.c
io-board.o: io-board.h io-board.c hw-ports.h sw-ports.h $(SW_PORTS:%=%.h)
mc.o      : adc.h

hw-ports.o: hw-ports.c
	gcc -c -D LINUX_DEV hw-ports.c

#	test-* files: ports

#test-adc: test-adc.c adc.c hw-ports.o sw-ports.o 
#	gcc -o $@ test-adc.c hw-ports.o sw-ports.o 

test-dpwm: test-dpwm.c dpwm.c tics.o hw-ports.o sw-ports.o 
	gcc -o $@ test-dpwm.c tics.o hw-ports.o sw-ports.o 

#test-mc: test-mc.c mc.c tics.o hw-ports.o sw-ports.o
#	gcc -o $@ test-mc.c tics.o hw-ports.o sw-ports.o

test-mf: test-mf.c mf.c stics.o hw-ports.o sw-ports.o
	gcc -o $@ test-mf.c stics.o hw-ports.o sw-ports.o

test-pwm: test-pwm.c pwm.c tics.o hw-ports.o sw-ports.o
	gcc -o $@ test-pwm.c tics.o hw-ports.o sw-ports.o

test-sp: test-sp.c sp.c hw-ports.o sw-ports.o
	gcc -o $@ test-sp.c hw-ports.o sw-ports.o

test-xpm: test-xpm.c xpm.c tics.o hw-ports.o sw-ports.o
	gcc -o $@ test-xpwm.c tics.o hw-ports.o sw-ports.o

#	test-* files: tools

test-ramp:	test-ramp.c ramp.c
	gcc -o $@ $@.c

test-stics:	test-stics.c stics.c
	gcc -o $@ $@.c

test-tics:	test-tics.c tics.c
	gcc -o $@ $@.c

#	test-* files: usr

test-usr: test-usr.c usr.c tics.o hw-ports.o sw-ports.o
	gcc -o $@ test-usr.c tics.o hw-ports.o sw-ports.o

test-notify: test-notify.c notify.c sw-ports.o
	gcc -o $@ test-notify.c sw-ports.o

#	old 

pid-default.o: pid-default.c pid-default.h

tc.o:	tc.c tc.h tc-config.h sw-ports.h hw-ports.h tics.h mc.h \
	usr.h ramp.h usr.h pid-default.h

tc-adc.o: tc-adc.c tc-adc.h tc-config.h sw-ports.h hw-ports.h tics.h mc.h \
	  usr.h ramp.h

test-hw-ports: test-hw-ports.c hw-ports.c
	gcc -D LINUX_DEV -o $@ test-hw-ports.c

test-pid: test-pid.c pid-default.c pid-default.h
	gcc -o $@ test-pid.c

clean:
	rm $(SW_PORTS_OBJ) $(TEST_PROGS):


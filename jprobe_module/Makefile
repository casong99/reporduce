# Step 1, make module
# make
# Step 2, Insert module for kprobe_sys_open
# insmod kprobe_sys_open.ko
# Step 3, After test done, remove module for kprobe_sys_open
# rmmod kprobe_sys_open.ko
obj-m += kprobe_sock_recvmsg.o 

KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

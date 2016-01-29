/*
Find Symbol vaule in system
# cat /proc/kallsyms | grep sys_open
ffffffff8019028a t sys_open

[root@localhost module]# cat /proc/kallsyms | grep sys_recvmsg
ffffffff802f9ca7 t sys_recvmsg
ffffffff80311e5e t compat_sys_recvmsg



*/
#include <linux/module.h>
#include <linux/kprobes.h>

struct kprobe kpb;

int handler_pre_sys_open(struct kprobe* p, struct pt_regs* regs)
{
	printk("sys_open_pre_handler: p->addr=0x%p\n", p->addr);		
	dump_stack();	

	return 0;
}


void handler_post_sys_open(struct kprobe* p, struct pt_regs* regs, unsigned long flags) 
{
	printk("post_handler_sys_open: p->addr=0x%p\n", p->addr);
	dump_stack();	
}


int handler_fault_sys_open(struct kprobe* p, struct pt_regs* regs, int trapnr)
{
	printk("handler_fault_sys_open: p->addr=0x%p\n", p->addr);
	return 0;
}

int handler_break_sys_open(struct kprobe* p, struct pt_regs* regs, int trapnr)
{
	printk("handler_break_sys_open: p->addr=0x%p\n", p->addr);
	return 0;
}

int init_module(void)
{
	kpb.pre_handler = handler_pre_sys_open;
	kpb.post_handler = handler_post_sys_open;
	kpb.fault_handler = handler_fault_sys_open;
	kpb.break_handler = handler_break_sys_open;
	

	kpb.addr = (kprobe_opcode_t *) 0xffffffff802f9ca7; // symbol value 
	register_kprobe(&kpb);
	printk("register kprobe \n");
	
	return 0;
}


void cleanup_module(void)
{
	unregister_kprobe(&kpb);
	printk("unregister kprobe\n");
}

MODULE_LICENSE("GPL");

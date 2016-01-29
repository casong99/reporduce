#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

//tcp_recvmsg need parameter
#include <linux/net.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <net/icmp.h>
#include <net/tcp.h>
#include <net/xfrm.h>
#include <net/ip.h>
#include <asm/uaccess.h>
#include <asm/ioctls.h>



static void entry_tcp_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
			  size_t len, int nonblock, int flags, int *addr_len)
{
	/*
	1. Check msg_iov length
	2. Check thread status
	3. Check memory 
	4. Check socket buffer
        */
//	if(len != msg->msg_iov->iov_len)
	{
		struct task_struct *task=get_current();
		//struct sk_buff *skb = skb_peek(&sk->sk_receive_queue);
		//int truesize = tcp_win_from_space(skb->truesize)/2;
		struct tcp_opt *tp = tcp_sk(sk); 

		pr_info("\n");
		pr_info("[ task ] : pid = %d , state = %ld, comm = %s\n", task->pid, task->state, task->comm);
		pr_info("[ task ] : nvcsw = %ld , nivcsw = %ld lock_depth =%d\n ", task->nvcsw, task->nivcsw, task->lock_depth);
		pr_info("[ task ] : prio = %d , static_prio = %d\n ", task->prio, task->static_prio);
		pr_info("[ task ] : activated = %d , policy = %d, flags = %ld\n ", task->activated, task->policy, task->flags);
		pr_info("[ task ] : sleep_avg = %ld , timestamp = %lld, last_ran = %lld\n ", task->sleep_avg, task->timestamp, task->last_ran);
		pr_info("[ task ] : stime = %ld , utime = %ld\n ", task->stime, task->utime);
		pr_info("[ task ] : min_flt = %ld , maj_flt = %ld\n ", task->min_flt, task->maj_flt);
		pr_info("[ sock ] : sock state = %d\n", sk->sk_state);
		pr_info("[ sock ] : sk_rcvbuf / sysctl_tcp_rmem = %d / %d\n", sk->sk_rcvbuf, sysctl_tcp_rmem[2]);
		pr_info("[ sock ] : sk->sk_wmem_queued / SOCK_MIN_SNDBUF = %d / %d\n", sk->sk_wmem_queued , SOCK_MIN_SNDBUF);
		pr_info("[ sock ] : sk_rmem_alloc = %d \n", atomic_read(&sk->sk_rmem_alloc));
		pr_info("[ sock ] : sk_wmem_alloc = %d \n", atomic_read(&sk->sk_wmem_alloc));
		pr_info("[ sock ] : sk_omem_alloc = %d \n", atomic_read(&sk->sk_omem_alloc));
		pr_info("[ sock ] : sk_err  = %d sk_err_soft =%d sk_ack_backlog =%d\n", sk->sk_err, sk->sk_err_soft, sk->sk_ack_backlog);
		pr_info("[ sock ] : sk_max_ack_backlog  = %d sk_type =%d \n", sk->sk_max_ack_backlog, sk->sk_type);
		pr_info("[ sock ] : sk_rcvlowat  = %d sk_sndtimeo =%ld sk_rcvtimeo =%ld\n", sk->sk_rcvlowat, sk->sk_sndtimeo, sk->sk_rcvtimeo);
		pr_info("[ sock ] : sk_route_caps =%d sk_lingertime =%ld\n", sk->sk_route_caps, sk->sk_lingertime); 
		pr_info("[ sock ] : sk_rcvbuf = %d sk_wmem_queued =%d\n", sk->sk_rcvbuf, sk->sk_wmem_queued);
		pr_info("[ sock ] : sk_forward_alloc = %d sk_allocation = %d sk_sndbuf =%d\n", sk->sk_forward_alloc, sk->sk_allocation, sk->sk_sndbuf);
		pr_info("[ sock ] : sk_flags = %ld\n", sk->sk_flags);
		pr_info("[ sock ] : msg_iovlen = %zu\n", msg->msg_iov->iov_len);
		pr_info("[ tcp  ] : tcp_memory_allocated / sysctl_tcp_mem= %d / %d\n", atomic_read(&tcp_memory_allocated), sysctl_tcp_mem[2]);
		pr_info("[ tcp  ] : snd_wscale = %d, rcv_wnd = %d, keepalive_time = %d, snd_wnd = %d max_window = %d \n", tp->snd_wscale, tp->rcv_wnd, tp->keepalive_time, tp->snd_wnd, tp->max_window);
		pr_info("[ tcp  ] : len = %zu , addr_len = %ld\n", len, addr_len); 
		//pr_info("tcp_recvsmg sk_receive_queue length = %d \n", skb_queue_len(&sk->sk_receive_queue));
		//pr_info("tcp_recvsmg sk_err_queue length = %d \n", skb_queue_len(&sk->sk_error_queue));
		//pr_info("tcp_recvsmg sk_protocol = %d \n", sk->sk_protocol);
		//pr_info("tcp_recvsmg sk_rcvtimeo = %ld \n", sk->sk_rcvtimeo);
		//pr_info("tcp_recvsmg sk_socket = %p \n", sk->sk_socket);
		//pr_info("tcp_recvsmg prequeue length = %d \n", skb_queue_len(&tp->ucopy.prequeue));
		//pr_info("tcp_recvsmg tcp_win_from_space = %d \n", truesize);
		//pr_info("tcp_recvmsg tp->rto = %d \n", tp->rto);

		pr_info("\n");
	}
	
	
	
	//pr_info(" tcp_recvmsg len = %zu \n", len);
	//pr_info(" tcp_recvmsg addr_len = %zu \n", addr_len);
	//pr_info(" tcp_recvsmg flag= %d \n", flags);
	//pr_info(" tcp_recvsmg nonblock= %d \n", nonblock);

	jprobe_return();
}

static struct jprobe jprobe_tcp_recvmsg = {
	.entry = (kprobe_opcode_t*)entry_tcp_recvmsg
};

static int __init start_module(void)
{
	//jprobe_sys_recvmsg.kp.addr=(kprobe_opcode_t *)kallsys_lookup_name("tcp_recvmsg");
	jprobe_tcp_recvmsg.kp.addr= (kprobe_opcode_t *) 0xffffffff803298f8;

	register_jprobe(&jprobe_tcp_recvmsg);
	printk("jprobe_sys_recvmsg insert!\n");
	return 0;
}

static void __exit exit_module(void)
{
	printk("jprobe_sys_open.ko remove\n");
	unregister_jprobe(&jprobe_tcp_recvmsg);
}

module_init(start_module);
module_exit(exit_module);
MODULE_LICENSE("GPL");

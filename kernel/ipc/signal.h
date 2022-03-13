/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <cpu/idt.h>
#include <libkernel/string.h>
#include <stdbool.h>
#include <stdint.h>

#define NSIG 32
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPOLL SIGIO

#define SIGPWR 30
#define SIGSYS 31
#define SIGUNUSED 31

#define SIGRTMIN 32
#define SIGRTMAX NSIG

#define SA_RESTART 0x10000000u
#define SA_NODEFER 0x40000000u
#define SA_RESETHAND 0x80000000u

#define SIG_BLOCK 0	  
#define SIG_UNBLOCK 1 
#define SIG_SETMASK 2 

typedef void (*__sighandler_t)(int);

#define SIG_DFL ((__sighandler_t)0)	 
#define SIG_IGN ((__sighandler_t)1)	 
#define SIG_ERR ((__sighandler_t)-1) 

#define sigmask(sig) (1UL << ((sig)-1))
#define siginmask(sig, mask) \
	((sig) > 0 && (sig) < SIGRTMIN && (sigmask(sig) & (mask)))

#define SIG_KERNEL_ONLY_MASK (sigmask(SIGKILL) | sigmask(SIGSTOP))

#define SIG_KERNEL_STOP_MASK (sigmask(SIGSTOP) | sigmask(SIGTSTP) | sigmask(SIGTTIN) | sigmask(SIGTTOU))

#define SIG_KERNEL_TERMINATE_MASK (                         \
    sigmask(SIGHUP) | sigmask(SIGINT) | sigmask(SIGKILL) | sigmask(SIGUSR1) |       \
    sigmask(SIGUSR2)) | sigmask(SIGPIPE) | sigmask(SIGALRM) | sigmask(SIGTERM) |    \
    sigmask(SIGVTALRM) | sigmask(SIGPROF) | sigmask(SIGPOLL) | sigmask(SIGSTKFLT) | \
    sigmask(SIGPWR))

#define SIG_KERNEL_COREDUMP_MASK (                                             \
	sigmask(SIGQUIT) | sigmask(SIGILL) | sigmask(SIGTRAP) | sigmask(SIGABRT) | \
	sigmask(SIGFPE) | sigmask(SIGSEGV) | sigmask(SIGBUS) | sigmask(SIGSYS) |   \
	sigmask(SIGXCPU) | sigmask(SIGXFSZ))

#define SIG_KERNEL_IGNORE_MASK ( \
	sigmask(SIGCONT) | sigmask(SIGCHLD) | sigmask(SIGWINCH) | sigmask(SIGURG))

#define sig_kernel_only(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_ONLY_MASK))
#define sig_kernel_terminate(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_TERMINATE_MASK))
#define sig_kernel_coredump(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_COREDUMP_MASK))
#define sig_kernel_ignore(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_IGNORE_MASK))
#define sig_kernel_stop(sig) \
	(((sig) < SIGRTMIN) && siginmask(sig, SIG_KERNEL_STOP_MASK))

#define sig_user_defined(p, signr)                      \
	(((p)->sighand[(signr)-1].sa_handler != SIG_DFL) && \
	 ((p)->sighand[(signr)-1].sa_handler != SIG_IGN))

#define sig_default_action(p, signr) ((p)->sighand[(signr)-1].sa_handler == SIG_DFL)

#define sig_fatal(p, signr)                                              \
	(!siginmask(signr, SIG_KERNEL_IGNORE_MASK | SIG_KERNEL_STOP_MASK) && \
	 (p)->sighand[(signr)-1].sa_handler == SIG_DFL)

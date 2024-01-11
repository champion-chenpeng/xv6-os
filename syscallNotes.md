# Syscall trap from user program --user/sleep.c as an example
## user/sleep.c
In user/sleep.c, there is a sleep system call, sleep(n), which is defined in user/user.h

And the sleep in user/user.h is linked to sys\_sleep in user/usys.S:
```
.global sleep
sleep:
 li a7, SYS_sleep
 ecall
 ret
```
There must be some code in other files which will be inserted before this part for the arguments stack-push.

So when we use a user program system call in user/user.h, it will refer to user/usys.S, and system call throught Assembly


## sys\_sleep
It seems wired that sys\_sleep takes no argument, but that's okay for system call, since in kernel mode, it can fetch arguments through quasi assembly code, which company with the right to r/w the memory
In sys\_sleep, the key variable is `n`, which is fetched through argint(0, &n), means the 0th argument is fetched to n. argint defined as:
```
argint(int n, int *ip)
{
  *ip = argraw(n);
}
```
and partial argraw:
```
argraw(int n)
{
  struct proc *p = myproc();
  switch (n) {
  case 0:
    return p->trapframe->a0;
...
```
So, in conclusion, sys\_sleep direct fetch param in memory and finish the task through other system calls

## conclusion
user program is bridged with system calls through .asm or .S(may be asm table?) 
ex. .asm:
```
{sleep.asm before sleep call}
{code for return frame and other things(include arg prepare}
li a7, SYS\_sleep
{sys\_sleep.asm}
```
More detail should refer to link tool, which do similar thing for .lib

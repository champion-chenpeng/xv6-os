1. multithread:
	- STACK direction: from large address to lower address?? seems wired
	- that's why i + 1 thread may change i's state
	- what's in the stack? typically return frames
	- however, under stack, it is its other things in thread, such as state
		- struct elements increase address in order in which members were defined(CPP reference);
		- the stack on RISC-V grows down(OS-riscv p28, riscv-calling chap18, p91)
		- so first the context will be polluted in this case
		- and state maybe polluted by next thread, since all\_thread increasing address with index i increase,
	- observed in gdb that sp decrease with function calles and variable declaration
	- this also illustrate the importance of proctection/isolation, the pc and sp is easy to be modified by asm!
	- ra=0x2000, it seems STACK\_SIZE
	- enter into all\_thread? it seems context is written with some stack varible, such as t / next\_thread, 
2.
3. barrier
	- good example to understand sleep and wakeup strategy
	- debug
		- avoid deadlock, use exclusive condition to distinguish customer and supplier, if else, otherwise the condition varible may be changed in previous if, then still enter into next if.
			- ex. if(customer) supplier=1; if(supplier){...}, the thread will be cunstomer then be supplier
		- printf a variable not function well in deadlock
			- underlying optmization(parallel) versus lock(serial)
			- the printf may be put after, or wait for some events for variable its about to printf

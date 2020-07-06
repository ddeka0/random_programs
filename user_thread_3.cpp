/* Ubuntu 18.04 x86_64 */
/* Compiler gcc version 7.5.0 */
#include <iostream>
#include <cassert>
#include <memory>


const int _stack_size = 16*64; // 1024B


void func(int x,int y) {
	std::cout <<"Hello from the func ["<<x<<"] ["<<y<<"]"<< std::endl;
}

int main() {

	unsigned char * stackBase = new unsigned char[_stack_size];
	unsigned char * stackTop = stackBase + _stack_size;

	std::cout <<"Before "<<std::hex <<(void*)(stackBase) << std::endl;

	asm volatile("movq %%rsp, %%r11\n\t"	/* save current stack ptr to r11(or rax etc.) */
				"movq %[in1], %%r12\n\t"
				"movq %%r12, %%rsp\n\t"		/* change rsp to stk, i.e. to our own stack top */ 
				"pushq %%r11\n\t"			/* Push r11 or prev stack ptr to current stack */
				// "pushq %%rbp\n\t"		
				: /*no input */
				:[in1] "m" (stackTop)			/* two inputs */
				:"r11","r12");

	func(1,2);
	
	asm volatile(
				"popq %%rsp\n\t"	/* restore stack: rsp = stack.pop() */
				:/* no outputs */
				:/* no inputs */
				:);

	std::cout <<"Program Ended!" << std::endl;
	std::cout <<"After "<<std::hex<<(void*)(stackBase) << std::endl;
	delete[] stackBase;
}


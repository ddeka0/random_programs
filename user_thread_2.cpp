/* Ubuntu 18.04 x86_64 */
/* Compiler gcc version 7.5.0 */
#include <iostream>
#include <cassert>
#include <memory>

class Thread {
public:
	
	/* stack will point to lowest addr of th User Stack */
	unsigned char * stack = nullptr;
	
	/*stack_ptr will point to the Highest addr of the User Stack */
	unsigned char* stack_ptr = nullptr;

	int stack_size = 0;

	Thread() {
		stack_size = 16*64; // 1024 B
		stack = new unsigned char[stack_size];
		/*stack_ptr points to the highest memory of this space*/
		stack_ptr = stack + stack_size;
	}
	
	~Thread() {
		std::cout <<"User stack deleted"<< std::endl;
		if(stack != nullptr)
			delete[] stack;
	}
	void func(int x,int y) {
		std::cout <<"Hello from the func ["<<x<<"] ["<<y<<"]"<< std::endl;
	}
};
int main() {
	/* createa a User level Thread */
	Thread t;

	/* Some random local variable */
	// int a[100];

	unsigned char* stk = t.stack_ptr;
	
	/* Note: rsp = current stack top */

	uint64_t prev_stack = 0;		/* for assert checking */
	uint64_t x = 7;	/*some random value, just for stack alighment of 16B */

	
	asm volatile("movq %%rsp, %%r11\n\t"		/* save current stack ptr to r11(or rax etc.) */
		"movq %[in1], %%r12\n\t"
		"movq %[in2], %%r13\n\t"
		"movq %%r11, %[out1]\n\t"	/* just for assert check later */
		"movq %%r12, %%rsp\n\t"	/* change rsp to stk, i.e. to our own stack top */ 
		"pushq %%r11\n\t"			/* Push r11 or prev stack ptr to current stack */		
		"pushq	%%r13\n\t"			/* for stack alignment only */
		:[out1] "=m" (prev_stack)	/*just for assert check later */
		:[in1] "m" (stk),[in2] "m" (x)			/* two inputs */
		:"r11","r12","r13","rsp","cc","memory");


	// call the func, this func
	// will will use our own stack

	t.func(1,2);
	
	/*just for checking x */

	uint64_t y;
	uint64_t _prev_stack;
	asm volatile(
		"popq %%r11\n\t"			/* y = stack.pop() */
		"popq %%rsp\n\t"			/* restore stack: rsp = stack.pop() */
		"movq %%rsp, %[out2]\n\t"	/* just for checking */
		"movq %%r11, %[out1]\n\t"
		:[out1] "=m" (y),[out2] "=m" (_prev_stack) /* two outputs */
		:/* no inputs */
		:"rsp","r11","cc","memory");

	assert(prev_stack == _prev_stack && "Main's stack not restored!");
	std::cout <<std::hex<<"Stack restored = "<< prev_stack << std::endl;

	assert(x == y && "x and y are not equal!!");
	std::cout <<"Program Ended!" << std::endl;
}


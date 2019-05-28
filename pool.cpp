#include <ctime>
#include <locale>
#include <sys/time.h>
#include <bits/stdc++.h>
#include "cpprest/uri.h"
#include "cpprest/json.h"
#include "yaml-cpp/yaml.h"
#include "cpprest/filestream.h"
#include <cpprest/http_client.h>
#include "cpprest/http_listener.h"
#include "cpprest/asyncrt_utils.h"
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener; 
using namespace web::http;
using namespace web::http::client;
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


/*Actual code starts from this point ......................... */


#define NUM_ELEMENTS 1
/*This function takes and vector as input and process each element
of the vector independently using the thread pool 
This function returns a vector of all the task handles (or threads ) 
This vector will be used later to merge all the thread (using .wait())
*/
pplx::task<std::vector<pplx::task<void>>> Process(std::vector<int> &v) {
	std::vector<pplx::task<void>> tids;
	for(auto &x:v) {
		auto t = pplx::task<void>([&x]()
		{	
			x = rand()%100;
			int x = 5000000;
			while(x-- > 0) {
				rand();
			}
		});
		tids.push_back(t);
	}
	cout << YELLOW << "return from Process()" << RESET << endl;
	return pplx::task_from_result<std::vector<pplx::task<void>>>(tids);
}

void TaskA() {
	sleep(rand()%5);
	cout << RED <<"taskA executing .." << RESET << endl;
}

void TaskB() {
	sleep(rand()%5);
	cout << GREEN <<"taskB executing .." << RESET << endl;
}

std::map<int,std::function<void(void)>> taskIdMap = {
	{0,		TaskA},
	{1,		TaskB}
};

int main() {
	
	srand(time(NULL));
	cout <<"::::::::::::::::::TEST 1::::::::::::::::::"<<endl;
	std::vector<int> v;
	for(int i = 0;i<NUM_ELEMENTS;i++) {
		v.push_back(i);
	}
	
	Process(v)
	.then([](pplx::task<std::vector<pplx::task<void>>> prevTask)
	{
		std::vector<pplx::task<void>> tids;
		try {
			tids = prevTask.get();
		}
		catch(const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		cout << YELLOW <<"waiting for worker threads to finish"<< RESET << endl;
		for(auto &t : tids) {
			t.wait();
		}
	})
	.wait();
	
	cout << YELLOW << "All worker threads finished task" << RESET << endl;
	for(auto &x:v) {
		std::cout << BOLDCYAN << x <<" ";
	}
	cout <<RESET<<endl;
	cout <<":::::::::::::::::<<ends>>:::::::::::::::::"<<endl<<endl;



	cout <<"::::::::::::::::::TEST 2::::::::::::::::::"<<endl;

	/* Following code models a thread pool execution */
	
	int cnt = 100;	// 10 task to dispatch 
	while(cnt--) {
		// sleep(rand()%2);
		int taskId = rand()%2;	// simulates an receipt of an event or task
		auto func = taskIdMap[taskId];
		pplx::task<int>([func,taskId]()
		{
			func();
			return taskId;

		})
		.then([](pplx::task<int> prevTask)
		{
			auto id = prevTask.get();
			cout << BOLDYELLOW <<(id == 0?"TaskA done !":"TaskB is done !") << RESET <<endl;
		}).wait();
	}
	cout<< BOLDWHITE <<"No more task to dispatch. Main thread id free now"<< RESET <<endl;
	
	getchar();
	cout <<":::::::::::::::::<<ends>>:::::::::::::::::"<<endl<<endl;
	
	cout <<"::::::::::::::::::TEST 3::::::::::::::::::"<<endl;
	
	auto g = [](std::string x) {
		cout<<x;
		return std::string(" World!");
	};

	pplx::task_from_result<std::string>("Hello")
	.then([g](std::string x)
	{	
		auto ret = g(x);
		return ret;
	})
	.then([](pplx::task<std::string> prevTask)
	{
		cout << prevTask.get() <<endl;

	}).wait();
	
	cout <<":::::::::::::::::<<ends>>:::::::::::::::::"<<endl<<endl;
	
	
}

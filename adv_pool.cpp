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
#define MAX_UE	5
using Message = std::pair<int,std::pair<std::function<void(void)>,std::string>>;
using Function = std::function<void(void)>;
using Node = std::pair<pair<int,int>,std::string>;
void GetOp() {
	sleep(rand()%2);
	std::cout << YELLOW <<"Get is done" << RESET << std::endl;
}
void SetOp() {
	sleep(rand()%5);
	std::cout << GREEN <<"Set is done" << RESET << std::endl;
}
std::map<std::string,std::string> depOpMap = {
	{"GetOp",		"SetOp"},
	{"SetOp",		"GetOp"}
};
std::map<int,pair<Function,std::string>> opIdMap = {
	{0,		{GetOp,"GetOp"}},
	{1,		{SetOp,"SetOp"}}
};
Message getNextEvent() {
	int ueId = rand()%MAX_UE;
	auto Op = opIdMap[rand()%2];
	return std::make_pair(ueId,std::make_pair(Op.first,Op.second));
}
std::multimap<Node,std::vector<Node>> Graph;
std::mutex gmutx;
int cnt = 0;
int main() {	
	srand(time(NULL));
	while(true) {
		sleep(1);
		Message msg = getNextEvent();
		cout << BOLDWHITE <<"udId = "<<msg.first <<" Op = "<<msg.second.second << RESET << endl;
		pplx::task<void>([msg]()
		{
			gmutx.lock();
			
			std::string opName = msg.second.second;
			int ueId = msg.first;
			Node node = {{ueId,cnt++},opName};

			Node dnode1 = {ueId,depOpMap[opName]};	// if found append
			Node dnode2 = {ueId,opName};			// if found ignore
			
			
			Graph.insert({ node , std::vector<Node>()});

			bool ok = true;
			for(auto &[_node,adjList]:Graph) {
				if(_node == dnode1) {
					Graph[_node].push_back(node);
					cout << CYAN <<" appended "<< RESET << std::endl;
					ok = false;
				}else if(_node == dnode2) {
					cout << RED <<" rejected "<< RESET << std::endl;
					ok = false;
				}	
			}				
			
			gmutx.unlock();

			if(ok) {
				// dispatch 
				msg.second.first();
			}
		});
	}
}

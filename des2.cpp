/*	Debashish Deka
	g++ -std=c++14 des.cpp
	./a.out
*/
#include <bits/stdc++.h>
using namespace std;

#define GOSSIP_EVENT	0
#define DECIDE_EVENT	1
#define GOSSIP_TIMEOUT 	6

int delays[10][10];

class Event; 			// forward declaration
std::set<Event> Set; 	// this will act as a priority queue
						// and store the events maintaining the
						// relative order of time

class Node;
vector<shared_ptr<Node>> all_nodes;
class Event {
public:
	int refTime;	// start reference time of this type of event
	int eventTime;
	int eventType;
	int eventTimeOutTime;
	shared_ptr<Node> targetNode;
	string msgToDeliver;
	
	Event(int refTime,int eventTime,shared_ptr<Node> target,string msg,int eventType,int timeOut) {
		this->refTime = refTime;
		this->eventTime = eventTime;
		this->targetNode = target;
		this->msgToDeliver = msg;
		this->eventType = eventType;
		this->eventTimeOutTime = timeOut;
	}

	bool operator < (const Event & event) const {
		return eventTime < event.eventTime;
	}
};

class Node {
public:
	int nodeId;
	vector<shared_ptr<Node>> peers;

	bool gossip_found;
	bool decide_event_started;
	
	Node(int Id) {
		this->nodeId = Id;
		this->gossip_found = false;
		this->decide_event_started = false;
	}

	void sendMsg(const Event &event,shared_ptr<Node> dstNode) {
		Event new_event(event.refTime,
						event.eventTime + delays[this->nodeId][dstNode->nodeId],
						dstNode,
						event.msgToDeliver,
						event.eventType,
						event.eventTimeOutTime);

		Set.insert(new_event);
		cout << "Msg sent to (gossip message) " << dstNode->nodeId << endl;
	}	

	void sendGossip(const Event &event) {
		cout <<"Message received at "<<this->nodeId <<"["<<event.msgToDeliver<<"]"<< endl;
		this->gossip_found = true;
		for(auto &peer:peers) {
			if( ((event.eventTime + delays[this->nodeId][peer->nodeId]) - event.refTime) <= event.eventTimeOutTime ) {
 				sendMsg(event,peer);
			}
		}
	}

	void decide() {
		if(this->gossip_found) {
			cout <<"Round 2 has started "<< this->nodeId <<" has decided" << endl;
			this->decide_event_started = true;
		}
	}

};
void round2_start() {
	for(auto & node:all_nodes) {
		node->decide();
	}
}
void executeEvent(const Event &event) {
	auto targetNode = event.targetNode;
	int eventType = event.eventType;
	
	if(eventType == GOSSIP_EVENT) {
		targetNode->sendGossip(event);
	}else if(eventType == DECIDE_EVENT) {
		//targetNode->decide();
		round2_start();		
	}
}
int main() {

	// create the nodes 
	auto A = shared_ptr<Node>(new Node(1));
	auto B = shared_ptr<Node>(new Node(2));
	auto C = shared_ptr<Node>(new Node(3));
	auto D = shared_ptr<Node>(new Node(4));

	all_nodes.push_back(A);
	all_nodes.push_back(B);
	all_nodes.push_back(C);
	all_nodes.push_back(D);

	// set the sending peers 
	A->peers.push_back(B);
	A->peers.push_back(C);
	C->peers.push_back(D);
	D->peers.push_back(B);

	// set the delays in the links
	// these delays will come from the Normal distribution
	delays[A->nodeId][C->nodeId] = 3;
	delays[A->nodeId][B->nodeId] = 8;
	delays[C->nodeId][D->nodeId] = 2;
	delays[D->nodeId][B->nodeId] = 2;

	// create a start event at node A
	// helps us to initiate the system
	Event X(0,0,A,"Welcome",GOSSIP_EVENT,GOSSIP_TIMEOUT);
	Event Y(6,6,A,"",DECIDE_EVENT,0);

	Set.insert(X);
	Set.insert(Y);

	// loop indefinitely
	while(true) {
		if(Set.empty()) {
			break;
		}
		auto event = Set.begin();
		executeEvent(*event);
		Set.erase(event);
	}

	cout <<"Simulation completed" << endl;
	return 0;
}

#include <iostream>
#include <list>

struct node_t
{
	int val;
	int prior;

	node_t(int val_ , int prior_) {val = val_ ; prior = prior_;};
};

class queue_prior
{
private:
	std::list<node_t> queue;
public:
	queue_prior() {};
	~queue_prior() {};

	void push(int val , int prior);
	int pop();
	void print();
};

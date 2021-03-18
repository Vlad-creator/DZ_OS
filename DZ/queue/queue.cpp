#include "queue.h"

void queue_prior::push(int val , int prior)
{
	node_t per(val , prior);
	int count = 0;
	for (auto it = queue.begin() ; it != queue.end() ; ++it)
	{
		if ((it -> prior) < (per.prior))
		{
			queue.insert(it , per);
			count = 1;
			break;
		};
	}
	if (count == 0)
		queue.push_back(per);
}

int queue_prior::pop()
{
	if (queue.size() == 0)
	{
		std::cout << "No elements";
		return 0;
	}

	int res = (queue.begin()) -> val;
	queue.pop_front();
	return res;
}

void queue_prior::print()
{
	std::cout << "queue - " << std::endl;
	if (queue.size() == 0)
	{
		std::cout << "queue is empty" << std::endl;
	}

	for (auto it = queue.begin() ; it != queue.end() ; ++it)
	{
		std::cout << "val - " << it -> val << " prior - " << it -> prior << std::endl;
	}
}

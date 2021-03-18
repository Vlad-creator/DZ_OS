#include "queue.h"

int main()
{
	queue_prior test{};
	for (int i = 0 ; i < 5 ; ++i)
		test.push(i , i * 2);

	std::cout << "queue before push (100 , 7) - " << std::endl;
	test.print();

	test.push(100 , 3);
	std::cout << "queue after push (100 , 7) , before pop - " << std::endl;
	test.print();

	std::cout << test.pop() << std::endl;
	int per = test.pop();
	per = test.pop();
	std::cout << "queue after pop - " << std::endl;
	test.print();

	return 0;
}
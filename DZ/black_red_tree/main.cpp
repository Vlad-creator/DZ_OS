#include "RBtree.h"

int main()
{
	struct Pair arr[6] =
            {{4, 0},
             {3, 0},
             {7, 0},
             {5, 0},
             {10, 0},
             {12, 0}};

    Map* map = createMap(arr, 6);
    assert (addItem(map, arr[1]) == SUCCESS);
    assert (printMap(map) == SUCCESS);
    assert (deleteMap(map) == SUCCESS);	
	return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define NDEBUG

#ifdef TEST
#define CALLOC(x, y) calloc_h(x, y)
#else
#define CALLOC(x, y) calloc(x, y)
#endif

struct Node;
struct Map;

enum error_t {SUCCESS = 0, LACK_OF_MEMORY, INVALID_ARG};

struct Pair{
    int key;
    int value;
};

struct Map* createMap (struct Pair* data, size_t size);

int foreach (struct Map* map, void (*foo)(struct Node* el, void* data), void* data);

struct Node* findItem (struct Map* map, int key);
int getValue (struct Node* node, int* error);
int setValue (struct Node* node, int value);
int deleteItem (struct Map* map, int key);

int addItem (struct Map* map, struct Pair item);
int printMap (struct Map* map);
int deleteMap (struct Map* map);
int clearMap (struct Map* map);
int isEmpty (struct Map* map);

void* calloc_h(size_t nnum, size_t size);

struct Node* findTop (struct Node* node);
struct Node* findParent (struct Node* tree, int key);

int isInTree (struct Map* map, int key);
struct Node* deleteNode (struct Node* node);
void insert (struct Node* parent, struct Node* node);

void foreach_h (struct Node* tree, void (*foo)(struct Node* el, void* data), void* data);
void deleteTree (struct Node* tree);
void printTreeWithIndents (struct Node* tree, int indents);

enum color_t {BLACK, RED};

struct Node {
    struct Node *parent, *left, *right;
    enum color_t color;
    int value;
    int key;
};

struct Map {
    struct Node* treeRoot;
};


static struct Node* findGrandparent(struct Node *node);
static struct Node* findUncle(struct Node *node);
static struct Node* findBrother (struct Node* node);


static void leftRotation(struct Node *node);
static void rightRotation(struct Node *node);


static void replaceWithChild (struct Node* node, struct Node* child);

static void deleteTheOnlyChild(struct Node* node);
static void delete_case1 (struct Node* node);
static void delete_case2 (struct Node* node);
static void delete_case3 (struct Node* node);
static void delete_case4 (struct Node* node);
static void delete_case5 (struct Node* node);
static void delete_case6 (struct Node* node);

static void insert_case1(struct Node* node);
static void insert_case2(struct Node* node);
static void insert_case3(struct Node* node);
static void insert_case4(struct Node* node);
static void insert_case5(struct Node* node);

void* calloc_h(size_t nnum, size_t size) {
    static int a = 0;

    if (a == 0 || a == 2) {
        a++;
        return NULL;
    }

    a++;
    return calloc (nnum,size);
}



//                              Find functions
//==========================================================================================

int isInTree (struct Map* map, int key) {
    return findItem(map, key) != NULL;
}


struct Node* findTop (struct Node* node) {

    if (node == NULL)
        return NULL;

    struct Node* res = node;

    while (res->parent)
        res = res->parent;

    return res;
}


struct Node* findParent (struct Node* tree, int key) {

    if (tree == NULL)
        return NULL;

    if (tree->key > key) {
        if (tree->left)
            return findParent(tree->left, key);
        else
            return tree;
    } else {
        if (tree->right)
            return findParent(tree->right, key);
        else
            return tree;
    }
}

static struct Node* findGrandparent(struct Node *node) {

    return node->parent->parent;
}

static struct Node* findUncle (struct Node* node) {

    struct Node* grandpa = findGrandparent(node);

    if (node->parent == grandpa->left)
        return grandpa->right;
    else
        return grandpa->left;
}

static struct Node* findBrother (struct Node* node) {

    if (node == NULL || node->parent == NULL) //TODO: if i ever call this function with NULL arguments?
        return NULL;

    if (node == node->parent->left)
        return node->parent->right;
    else
        return node->parent->left;
}

struct Node* findMax (struct Node* tree) {

    while (tree->right)
        tree = tree->right;

    return tree;
}

struct Node* findMin (struct Node* tree) {

    while (tree->left)
        tree = tree->left;

    return tree;
}


//                              Rotate functions
//==========================================================================================




static void leftRotation (struct Node* node) {

    struct Node* pivot = node->right;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->right = pivot->left;
    if (pivot->left != NULL)
        pivot->left->parent = node;

    node->parent = pivot;
    pivot->left = node;
}

static void rightRotation(struct Node *node) {

    struct Node* pivot = node->left;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->left = pivot->right;

    if (pivot->right != NULL)
        pivot->right->parent = node;

    node->parent = pivot;
    pivot->right = node;
}



//                              Inserting element
//==========================================================================================



void insert (struct Node* parent, struct Node* node) {

    node->parent = parent;

    if (parent == NULL) {
        insert_case1(node);
        return;
    }

    if (parent->key > node->key)
        parent->left = node;
    else
        parent->right = node;

    insert_case2(node);
}

static void insert_case1(struct Node* node) {

    if (node->parent == NULL)
        node->color = BLACK;
    else
        insert_case2(node);
}

static void insert_case2(struct Node* node) {

    if (node->parent->color == BLACK)
        return; /* Tree is still valid */
    else
        insert_case3(node);
}

static void insert_case3(struct Node* node) {

    struct Node *uncle = findUncle(node), *grandpa;

    if ((uncle != NULL) && (uncle->color == RED)) {

        node->parent->color = BLACK;
        uncle->color = BLACK;

        grandpa = findGrandparent(node);
        grandpa->color = RED;

        insert_case1(grandpa);
    } else {
        insert_case4(node);
    }
}

static void insert_case4(struct Node* node) {

    struct Node* grandpa = findGrandparent(node);

    if ((node == node->parent->right) && (node->parent == grandpa->left)) {

        leftRotation(node->parent);
        node = node->left;
    } else if ((node == node->parent->left) && (node->parent == grandpa->right)) {

        rightRotation(node->parent);
        node = node->right;
    }

    insert_case5(node);
}


static void insert_case5(struct Node* node) {

    struct Node* grandpa = findGrandparent(node);

    node->parent->color = BLACK;
    grandpa->color = RED;

    if ((node == node->parent->left) && (node->parent == grandpa->left)) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}





//                              Deleting element
//==========================================================================================




struct Node* deleteNode (struct Node* node) {

    struct Node* M;
    struct Node* tmp = node;

    if (node->right)
        M = findMin (node->right);
    else if (node->left)
        M = findMax (node->left);
    else {
        tmp = node->parent;
        M = node;
    }

    node->value = M->value;
    node->key = M->key;

    deleteTheOnlyChild(M);

    return findTop(tmp);
}


static void deleteTheOnlyChild(struct Node* node) {

    assert (node->left == NULL || node->right == NULL);

    struct Node *child;

    if (node->left == NULL && node->right == NULL) {

        if (node->color == BLACK)
            delete_case1(node);

        if (node->parent == NULL) {
            free(node);
            return;
        }

        if (node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;

        free(node);
        return;
    }

    child = node->right;

    replaceWithChild (node, child);

    if (node->color == BLACK)//Cause node has only one child, child->color can be only RED
        child->color = BLACK;

    free(node);
}


static void replaceWithChild (struct Node* node, struct Node* child) {

    assert (child && node);

    child->parent = node->parent;

    if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;
}


static void delete_case1 (struct Node* node)
{
    if (node->parent != NULL)
        delete_case2(node);
}

static void delete_case2 (struct Node* node) {

    struct Node* brother = findBrother (node);

    if (brother->color == RED) {
        node->parent->color = RED;
        brother->color = BLACK;

        if (node == node->parent->left)
            leftRotation (node->parent);
        else
            rightRotation (node->parent);
    }

    delete_case3 (node);
}

static void delete_case3 (struct Node* node) {

    struct Node *brother = findBrother (node);

    if ((node->parent->color == BLACK) &&
        (brother->color == BLACK) &&
        (brother->left == NULL || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK)) {

        brother->color = RED;
        delete_case1(node->parent);
    } else
        delete_case4(node);
}

static void delete_case4 (struct Node* node) {

    struct Node *brother = findBrother (node);

    if ((node->parent->color == RED) &&
        (brother->color == BLACK) &&
        (brother->left == NULL  || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK))  {

        brother->color = RED;
        node->parent->color = BLACK;
    } else
        delete_case5(node);
}

static void delete_case5 (struct Node* node) {

    struct Node *brother = findBrother (node);

    if  (brother->color == BLACK) {

        if ((node == node->parent->left) &&
            (brother->right == NULL || brother->right->color == BLACK) &&
            (brother->left && brother->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->left->color = BLACK;
            rightRotation(brother);

        } else if ((node == node->parent->right) &&
                   (brother->left == NULL || brother->left->color == BLACK) &&
                   (brother->right && brother->right->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->right->color = BLACK;
            leftRotation(brother);
        }
    }

    delete_case6(node);
}

static void delete_case6 (struct Node* node) {

    struct Node* brother = findBrother (node);

    brother->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) {
        brother->right->color = BLACK;
        leftRotation (node->parent);
    } else {
        brother->left->color = BLACK;
        rightRotation (node->parent);
    }
}


void deleteTree (struct Node* tree) {

    if (tree == NULL)
        return;

    if (tree->left)
        deleteTree(tree->left);
    if (tree->right)
        deleteTree(tree->right);

    free (tree);
}


//                              Foreach functions
//==========================================================================================


void foreach_h (struct Node* tree, void (*foo)(struct Node* el, void* data), void* data) {

    if (tree == NULL)
        return;

    foreach_h(tree->left, foo, data);
    foreach_h(tree->right, foo, data);
    foo (tree, data);
}



//                              Dump functions
//==========================================================================================


static void writeIndents (int indents) {
    for (int i = 0; i < indents; ++i)
        printf (":::|");
}

static void printNode (struct Node* node, int indents) {

    writeIndents(indents);

    if (node == NULL)
        printf ("NULL(B)\n");
    else {

        printf("[key: %d, value: %d]", node->key, node->value);

        if (node->color == RED)
            printf("(R)\n");
        else
            printf("(B)\n");
    }
}

void printTreeWithIndents (struct Node* tree, int indents) {

    printNode(tree, indents);

    if (tree == NULL)
        return;

    printTreeWithIndents(tree->left, indents + 1);
    printTreeWithIndents(tree->right, indents + 1);
}

//                              Create function
//==========================================================================================

/*
 * function creates Map with elements from data.
 * if LACK_OF_MEMORY occurs, then it will return NULL,
 * otherwise the pointer on the created Map will be returned
 */
struct Map* createMap (struct Pair* data, size_t size) {

    struct Map* map = (struct Map*) CALLOC(1, sizeof(struct Map));

    if (map == NULL)
        return NULL;

    map->treeRoot = NULL;

    if (data == NULL || size == 0)
        return map;

    for (size_t i = 0; i < size; ++i) {

        if (addItem(map, data[i]) != SUCCESS) {
            deleteMap(map);
            return NULL;
        }
    }

    return map;
}


//                              Find functions
//==========================================================================================

/*
 * function tries to find item in map.
 * If it is found, then the pointer on it will be returned,
 * otherwise NULL will be returned
 */

struct Node* findItem (struct Map* map, int key) {

    if (map->treeRoot == NULL)
        return NULL;

    struct Node* tree = map->treeRoot;

    struct Node* tmp = tree;

    while (tmp) {
        if (tmp->key > key)
            tmp = tmp->left;
        else if (tmp->key < key)
            tmp = tmp->right;
        else
            return tmp;
    }

    return NULL;
}


//                              Inserting element
//==========================================================================================



/*
 * function inserts new item in map if it is unique;
 */
int addItem (struct Map* map, struct Pair item) {

    if (map == NULL)
        return INVALID_ARG;

    struct Node* tree = map->treeRoot;

    if (isInTree(map, item.key)) {
        return SUCCESS;
    }

    struct Node* node = (struct Node*) CALLOC(1, sizeof(struct Node));

    if (node == NULL) {
        return LACK_OF_MEMORY;
    }

    node->value = item.value;
    node->key = item.key;
    node->color = RED;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    struct Node* parent = findParent (tree, node->key);

    insert (parent, node);

    map->treeRoot = findTop(node);

    return SUCCESS;
}





//                              Deleting function
//==========================================================================================


/*
 * function deletes item from the map;
 * if the map or tree root is NULL then it returns INVALID_ARG error
 * else SUCCESS is returned
 */

int deleteItem (struct Map* map, int key) {

    if (map == NULL)
        return INVALID_ARG;

    struct Node* tree = map->treeRoot;

    if (tree == NULL)
        return INVALID_ARG;

    struct Node* node = findItem(map, key);

    if (node)
        map->treeRoot = deleteNode(node);

    return SUCCESS;
}

int clearMap (struct Map* map) {

    if (map == NULL)
        return INVALID_ARG;

    deleteTree(map->treeRoot);
    map->treeRoot = NULL;
    return SUCCESS;
}

int deleteMap (struct Map* map) {

    if (map == NULL)
        return INVALID_ARG;

    deleteTree(map->treeRoot);
    free (map);
    return SUCCESS;
}


//                              Foreach functions
//==========================================================================================

int foreach (struct Map* map, void (*foo)(struct Node* el, void* data), void* data) {

    if (map == NULL || foo == NULL)
        return INVALID_ARG;

    foreach_h(map->treeRoot, foo, data);
    return SUCCESS;
}


//                              Dump functions
//==========================================================================================


int printMap (struct Map* map) {

    if (map == NULL)
        return INVALID_ARG;

    printTreeWithIndents(map->treeRoot, 0);

    return SUCCESS;
}


//                              Interface functions
//==========================================================================================



int getValue (struct Node* node, int* error) {

    if (node == NULL) {
        if (error)
            *error = INVALID_ARG;
        return 0;
    }

    return node->value;
}

int setValue (struct Node* node, int value) {

    if (node == NULL)
        return INVALID_ARG;

    node->value = value;
    return SUCCESS;
}

int isEmpty (struct Map* map) {

    if (map == NULL)
        return INVALID_ARG;

    return map->treeRoot == NULL;
}
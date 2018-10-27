#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lxOs.h"
#include "lxMsgQue.h"
#include "lxQue.h"
#include "lxStack.h"
#include "type.h"
#include "lxlog.h"

typedef struct tree_node {
    struct tree_node * lChild;
    struct tree_node * rChild;
    u32int value;
}tree_node;

static lxque_obj * lxQueOBJ = NULL;

void init(LOG_LEVEL_E level)
{
    Log_Set_PrintLevel(level);
}

void release_tree(tree_node * initNode)
{

}

void create_tree(tree_node * initNode)
{
    if(!initNode)
    {
        return;
    }

    initNode->value = 1;
    initNode->lChild = lxOSMalloc(sizeof(tree_node));
    initNode->rChild = lxOSMalloc(sizeof(tree_node));
    initNode->lChild->value = 2;
    initNode->rChild->value = 3;

    initNode->lChild->lChild = lxOSMalloc(sizeof(tree_node));
    initNode->lChild->lChild->value = 4;
    initNode->lChild->lChild->lChild = NULL;
    initNode->lChild->lChild->rChild = NULL;

    initNode->lChild->rChild = lxOSMalloc(sizeof(tree_node));
    initNode->lChild->rChild->value = 5;
    initNode->lChild->rChild->lChild = NULL;
    initNode->lChild->rChild->rChild = NULL;

    initNode->rChild->lChild = lxOSMalloc(sizeof(tree_node));
    initNode->rChild->lChild->value = 6;
    initNode->rChild->lChild->lChild = NULL;
    initNode->rChild->lChild->rChild = NULL;

    initNode->rChild->rChild = lxOSMalloc(sizeof(tree_node));
    initNode->rChild->rChild->value = 7;
    initNode->rChild->rChild->lChild = NULL;
    initNode->rChild->rChild->rChild = NULL;

    return;
}

void showValue(int data)
{
    LOG_ERROR_PRINT("[%d]\n", data);

    return;
}

void Level_traversal(tree_node * node)
{
    tree_node tmpNode = {0};
    size_t len = sizeof(tree_node);
    if(node == NULL || lxQueOBJ == NULL)
    {
        return;
    }

    lxQueOBJ->push_back(lxQueOBJ, node, len);

    while(!lxQueOBJ->isEmpty(lxQueOBJ))
    {
        lxQueOBJ->pop_front(lxQueOBJ, &tmpNode, len);
        showValue(tmpNode.value);
        lxQueOBJ->push_back(lxQueOBJ, tmpNode.lChild, len);
        lxQueOBJ->push_back(lxQueOBJ, tmpNode.rChild, len);
    }
}

int main(int argc, char *argv[])
{

    tree_node firstNode = {0};
    lxQueOBJ = NEW(lxque_obj);
    init(LOG_LEVEL_ERROR);
    create_tree(&firstNode);
    Level_traversal(&firstNode);

    DELETE(lxque_obj, lxQueOBJ);
    return 0;
}

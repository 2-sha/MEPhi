#pragma once

#include <random>
#include <exception>
#include <functional>
#include <queue>
#include <initializer_list>

// Т.к. он конфиктует с curses
namespace graphviz
{
    #include <gvc.h>
    #include <cgraph.h>
}

template<class Type>
class Treap
{
public:

    struct Node
    {
        // x - tree, y - heap
        long x = 0, y = 0;
        Type data;
        Node* left = nullptr, * right = nullptr, * parent = nullptr;

        Node(long x, long y, Type data)
            : x(x), y(y), data(data) {};
        Node() {};
    };

private:

    const int MAX_RANDOM = 10000000;

    int size;

    std::mt19937 mersenne;

    Node* root;
    Node* lastAdded;

    Node* merge(Node* left, Node* right)
    {
        if (left == nullptr)
            return right;
        if (right == nullptr)
            return left;

        if (left->y > right->y)
        {
            left->right = merge(left->right, right);
            return left;
        }
        else
        {
            right->left = merge(left, right->left);
            return right;
        }
    }

    void split(Node* root, long x, Node** left, Node** right)
    {
        Node* newTree = nullptr;
        if (root->x <= x)
        {
            if (root->right == nullptr)
                *right = nullptr;
            else
                split(root->right, x, &newTree, right);
            *left = new Node(root->x, root->y, root->data);
            (*left)->x = root->x;
            (*left)->y = root->y;
            (*left)->left = root->left;
            (*left)->right = newTree;
        }
        else
        {
            if (root->left == nullptr)
                *left = nullptr;
            else
                split(root->left, x, left, &newTree);
            *right = new Node(root->x, root->y, root->data);
            (*right)->x = root->x;
            (*right)->y = root->y;
            (*right)->left = newTree;
            (*right)->right = root->right;
        }
    }

    void initRandom()
    {
        std::random_device rd;
        mersenne.seed(rd());
    }

    void forwardTrav(Node* p, std::function<void(Node* node)> func)
    {
        if (p == nullptr)
            return;
        forwardTrav(p->left, func);
        func(p);
        forwardTrav(p->right, func);
    }

    void nlrTrav(Node* p, std::function<void(Node* node)> func)
    {
        if (p == nullptr)
            return;
        func(p);
        nlrTrav(p->left, func);
        nlrTrav(p->right, func);
    }

public:

    void buildTree(std::vector<std::pair<long, Type>> list)
    {
        if (size != 0)
            throw std::invalid_argument("Tree already built");
        for (auto item : list)
        {
            Node* newItem = new Node(item.first, mersenne() % MAX_RANDOM, item.second);

            while (lastAdded != nullptr && lastAdded->y < newItem->y)
                lastAdded = lastAdded->parent;
            newItem->parent = lastAdded;
            if (lastAdded == nullptr)
            {
                newItem->left = root;
                root = newItem;
            }
            else
            {
                newItem->left = lastAdded->right;
                lastAdded->right = newItem;
            }
            lastAdded = newItem;
        }
        size = list.size();
    }

    void clear()
    {
        if (root == nullptr)
            return;

        std::queue<Node*> nodes;
        nodes.push(root);

        while (!nodes.empty())
        {
            Node* p = nodes.front();
            if (p->left != nullptr)
                nodes.push(p->left);
            if (p->right != nullptr)
                nodes.push(p->right);

            nodes.pop();
            delete p;
        }

        root = lastAdded = nullptr;
        size = 0;
    }

	Treap() 
    {
        initRandom();
    };
    Treap(std::initializer_list<std::pair<long, Type>> list)
    {
        initRandom();
        buildTree(list);
    };
	~Treap() 
    {
        clear();
    };

    void add(long x, Type data)
    {
        if (root == nullptr)
        {
            root = new Node(x, mersenne() % MAX_RANDOM, data);
            return;
        }
        Node* newNode = new Node(x, mersenne() % MAX_RANDOM, data);
        Node* left;
        Node* right;
        split(root, x, &left, &right);
        root = merge(merge(left, newNode), right);
        size++;
    }

    void remove(long x)
    {
        if (size == 0)
            return;
        Node* left, * right, * item;
        split(root, x - 1, &left, &item);
        split(root, x, &item, &right);
        root = merge(left, right);
        size--;
    }

   Node *find(long x)
    {
        Node* p = root;
        if (p == nullptr)
            throw std::invalid_argument("Дерево пустое");

        while (p != nullptr && p->x != x)
        {
            if (x > p->x)
                p = p->right;
            else
                p = p->left;
        }

        return p;
    }

    void map(std::function<void(Node* node)> func)
    {
        Node* p = root;
        forwardTrav(p, func);
    }

    void nlr(std::function<void(Node* node)> func)
    {
        Node* p = root;
        nlrTrav(p, func);
    }

    void generateImage(const char* name, const char* ext)
    {
        graphviz::GVC_t* gvc = graphviz::gvContext();
        graphviz::Agraph_t* g = graphviz::agopen(_strdup("G"), graphviz::Agstrictundirected, NULL);
        graphviz::agattr(g, 1, (char*)"shape", (char*)"box");

        std::queue<std::pair<Node*, graphviz::Agnode_t*>> nodes;

        graphviz::Agnode_t* agn = agnode(g, NULL, TRUE);
        graphviz::agset(agn, (char*)("label"), (char*)(
            "(" + std::to_string(root->x) + "; " +
            std::to_string(root->y) + ")\n" +
            root->data
        ).c_str());
        nodes.push(std::make_pair(root, agn));

        int i = 1;
        while (!nodes.empty())
        {
            Node* parentNode = nodes.front().first;
            graphviz::Agnode_t* parentAgn = nodes.front().second;

            if (parentNode->left != nullptr)
            {
                graphviz::Agnode_t* agn = agnode(g, NULL, TRUE);
                graphviz::agset(agn, (char*)("label"), (char*)(
                    "(" + std::to_string(parentNode->left->x) + "; " +
                    std::to_string(parentNode->left->y) + ")\n" +
                    parentNode->left->data
                ).c_str());
                graphviz::agedge(g, parentAgn, agn, NULL, TRUE);
                nodes.push(std::make_pair(parentNode->left, agn));
            }
            if (parentNode->right != nullptr)
            {
                graphviz::Agnode_t* agn = agnode(g, NULL, TRUE);
                graphviz::agset(agn, (char*)("label"), (char*)(
                    "(" + std::to_string(parentNode->right->x) + "; " +
                    std::to_string(parentNode->right->y) + ")\n" +
                    parentNode->right->data
                ).c_str());
                graphviz::agedge(g, parentAgn, agn, NULL, TRUE);
                nodes.push(std::make_pair(parentNode->right, agn));
            }

            nodes.pop();
        }

        graphviz::gvLayout(gvc, g, "dot");
        graphviz::gvRenderFilename(gvc, g, ext, name);
        graphviz::gvFreeLayout(gvc, g);
        graphviz::agclose(g);
        graphviz::gvFreeContext(gvc);
    }

    int getSize()
    {
        return size;
    }

    Node* getRoot()
    {
        return root;
    }

};

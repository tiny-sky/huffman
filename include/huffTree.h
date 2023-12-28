#ifndef _HUFF_TREE_H
#define _HUFF_TREE_H

#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>

namespace huffman {

static int flag;
std::string file;

struct Node {
    uchar c;
    int freq;
    Node *left;
    Node *right;
    Node(uchar _c, int f, Node *l = nullptr, Node *r = nullptr)
        : c(_c), freq(f), left(l), right(r) {}
    bool operator<(const Node &node) const {//重载，优先队列的底层数据结构std::heap是最大堆
        return freq > node.freq;
    }
};

class huffTree {
public:
    huffTree(const std::map<uchar, int> &afMap) {
        for (auto i : afMap) {
            Node n(i.first, i.second);
            q.push(n);
        }
        makehuffTree();
    }
    ~huffTree() {
        Node node = q.top();
        _deleteTree(node.left);
        _deleteTree(node.right);
    }
    void makehuffTree() {
        while (q.size() != 1) {
            Node *left = new Node(q.top());
            q.pop();
            Node *right = new Node(q.top());
            q.pop();
            Node node('R', left->freq + right->freq, left, right);
            q.push(node);
        }
    }
    void huffmanCode(std::map<uchar, std::string> &codeMap) {
        Node node = q.top();
        std::string prefix;
        if(flag & FLAG_MAP | flag & FLAG_FILE){
            std::ofstream hufmap(file+".hufmap", std::ios::binary);
            _huffmanCode(&node, prefix, codeMap, hufmap);
            hufmap.close();
        } else
            _huffmanCode(&node, prefix, codeMap);
    }
    Node getHuffTree() {
        return q.top();
    }
    void watch() {
        Node node = q.top();
        if (flag & FLAG_TREE && !(flag & FLAG_FILE))
            _treeWatch(&node);
        if(flag & FLAG_FILE && flag & FLAG_TREE){
            std::ofstream huftree(file +".huftree", std::ios::binary);
            _treeWatch(&node, huftree);
            huftree.close();
        }
    }

private:
    struct nodeBacklog {//回溯点
        Node *node;
        int next_sub_idx;
    };
    enum {
        LeftIndex,
        RightIndex
    };
    enum { MaxLevel = 64 };

    static void nbl_push(nodeBacklog *nbl, nodeBacklog **top, nodeBacklog **bottom) {
        if (*top - *bottom < MaxLevel) {
            (*(*top)++) = *nbl;//这里其实已经将nbl的值拷贝到*top指向的位置，不能直接*top = nbl，否则会随nbl变化
        }
    }
    static nodeBacklog *nbl_pop(nodeBacklog **top, nodeBacklog **bottom) {
        return *top > *bottom ? --*top : nullptr;
    }
    static bool _isLeaf(Node *n) {
        return n->left == nullptr && n->right == nullptr;
    }

private:
    void _deleteTree(Node *n) {
        if (!n) return;
        _deleteTree(n->left);
        _deleteTree(n->right);
        delete n;
    }
    void _treeWatch(Node *root);
    void _treeWatch(Node *root, std::ofstream &outFile);

    static void _printNode(Node *n) {
        if (n) {
            printf("%c:%d\n", n->c, n->freq);
        }
    }

    static void _printNodeFile(Node *n, std::ofstream &outFile) {
        if(n) {
            outFile << n->c << ":" << n->freq << std::endl;
        }
    }
    void _huffmanCode(Node *root, std::string &prefix,
                      std::map<uchar, std::string> &codeMap) {
        if (_isLeaf(root)) {
            codeMap[root->c] = prefix;
            return;
        }
        prefix.push_back('0');
        _huffmanCode(root->left, prefix, codeMap);
        prefix.pop_back();
        prefix.push_back('1');
        _huffmanCode(root->right, prefix, codeMap);
        prefix.pop_back();
    }

    void _huffmanCode(Node *root, std::string &prefix,
                      std::map<uchar, std::string> &codeMap, std::ofstream &outFile) {
        if(_isLeaf(root)) {
            codeMap[root->c] = prefix;
            if (flag & FLAG_MAP && flag & FLAG_FILE)
                outFile << std::left << std::setw(8) << root->c << std::setw(4) << "->" << std::setw(10) << prefix << std::endl;
            if (flag & FLAG_MAP && !(flag & FLAG_FILE))
                std::cout << std::left << std::setw(8) << root->c << std::setw(4) << "->" << std::setw(10) << prefix << std::endl;
            return;
        }
        prefix.push_back('0');
        _huffmanCode(root->left, prefix, codeMap,outFile);
        prefix.pop_back();
        prefix.push_back('1');
        _huffmanCode(root->right, prefix, codeMap,outFile);
        prefix.pop_back();
    }

private:
    std::priority_queue<Node> q;
    std::string filename;
};

void huffTree::_treeWatch(Node *root) {
    int level = 0;
    Node *node = root;
    nodeBacklog nbl;
    nodeBacklog *p_nbl = nullptr;
    nodeBacklog *top, *bottom, nbl_stack[MaxLevel];
    top = bottom = nbl_stack;

    for (;;) {
        if (node != nullptr) {
            int sub_index = p_nbl != nullptr ? p_nbl->next_sub_idx : RightIndex;
            p_nbl = nullptr;
            if (_isLeaf(node) || sub_index == LeftIndex) {
                nbl.node = nullptr;
                nbl.next_sub_idx = RightIndex;
            } else {
                nbl.node = node;
                nbl.next_sub_idx = LeftIndex;
            }
            nbl_push(&nbl, &top, &bottom);
            level++;
            if (sub_index == RightIndex) {
                for (int i = 1; i < level; ++i) {
                    if (i == level - 1) {
                        printf("%-8s", "|-------");
                    } else {
                        if (nbl_stack[i - 1].node != nullptr) {
                            printf("%-8s", "|");
                        } else {
                            printf("%-8s", " ");
                        }
                    }
                }
                _printNode(node);
            }
            node = sub_index == LeftIndex ? node->left : node->right;
        } else {
            p_nbl = nbl_pop(&top, &bottom);
            if (p_nbl == nullptr)
                break;
            node = p_nbl->node;
            level--;
        }
    }
}

void huffTree::_treeWatch(Node *root, std::ofstream &outFile) {
    int level = 0;
    Node *node = root;
    nodeBacklog nbl;
    nodeBacklog *p_nbl = nullptr;
    nodeBacklog *top, *bottom, nbl_stack[MaxLevel];
    top = bottom = nbl_stack;

    for (;;) {
        if (node != nullptr) {
            int sub_index = p_nbl != nullptr ? p_nbl->next_sub_idx : RightIndex;
            p_nbl = nullptr;
            if (_isLeaf(node) || sub_index == LeftIndex) {
                nbl.node = nullptr;
                nbl.next_sub_idx = RightIndex;
            } else {
                nbl.node = node;
                nbl.next_sub_idx = LeftIndex;
            }
            nbl_push(&nbl, &top, &bottom);
            level++;
            if (sub_index == RightIndex) {
                for (int i = 1; i < level; ++i) {
                    if (i == level - 1) {
                        //printf("%-8s", "|-------");
                        outFile << std::left << std::setw(8) << "|-------";
                    } else {
                        if (nbl_stack[i - 1].node != nullptr) {
                            outFile << std::left << std::setw(8) << "|";
                        } else {
                            outFile << std::left << std::setw(8) << " ";
                        }
                    }
                }
                _printNodeFile(node,outFile);
            }
            node = sub_index == LeftIndex ? node->left : node->right;
        } else {
            p_nbl = nbl_pop(&top, &bottom);
            if (p_nbl == nullptr)
                break;
            node = p_nbl->node;
            level--;
        }
    }
}
}// namespace huffman

#endif
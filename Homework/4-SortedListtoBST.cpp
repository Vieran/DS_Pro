/* description: https://202.120.40.8:30463/contest/6/problem/1
 * start: 2021-3-28
 * timelimit: 8days
 */

#include <stdio.h>

#include <queue>
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
    void insert(int val) {
        ListNode *tmp = this;
        while (tmp->next) tmp = tmp->next;
        tmp->next = new ListNode(val);
    }
    void delete_(ListNode *t) {
        ListNode *tmp = t;
        while (tmp) {
            t = t->next;
            delete tmp;
            tmp = t;
        }
    }
};

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

void LL(TreeNode *&t) {
    TreeNode *tmp = t->left;
    t->left = tmp->right;
    tmp->right = t;
    t = tmp;
}
void RR(TreeNode *&t) {
    TreeNode *tmp = t->right;
    t->right = tmp->left;
    tmp->left = t;
    t = tmp;
}
void LR(TreeNode *&t) {
    RR(t->left);
    LL(t);
}
void RL(TreeNode *&t) {
    LL(t->right);
    RR(t);
}

// get the height of the tree
int height(TreeNode *&t) {
    if (t == nullptr) return 0;
    int left_h = height(t->left) + 1;
    int right_h = height(t->right) + 1;
    return (left_h > right_h) ? left_h : right_h;
}

// get the balance factor of a tree
int balance_factor(TreeNode *&t) {
    if (t == nullptr)
        return 0;
    int balance = height(t->left) - height(t->right);
    return (balance > 0) ? balance : (-balance);
}

void insert(int val, TreeNode *&t) {
    if (t == nullptr) {
        t = new TreeNode(val, nullptr, nullptr);
    } else if (val < t->val) {  // insert in the left
        insert(val, t->left);
        if (balance_factor(t) > 1)
            if (val < t->left->val)
                LL(t);
            else
                LR(t);
    } else if (val > t->val) {
        insert(val, t->right);
        if (balance_factor(t) > 1)
            if (val < t->right->val)
                RL(t);
            else
                RR(t);
    }
}

void delete_(TreeNode *&t) {
    TreeNode *tmp = t;
    while (tmp) {
        delete tmp;
    }
}

// given a listnode list, return the treenode list
TreeNode *sortedListToBST(ListNode *head) {
    // create a TreeNode tree and insert the listnode into it
    TreeNode *treehead = new TreeNode(head->val, nullptr, nullptr);
    ListNode *tmp = head->next;
    while (tmp) {
        insert(tmp->val, treehead);
        tmp = tmp->next;
    }

    // level traverse of the tree and store it in treenode list
    std::queue<TreeNode *> q;
    q.push(treehead);
    TreeNode *t_tmp = treehead;
    while (!q.empty()) {
        t_tmp = q.front();
        printf("%d ", t_tmp->val);
        q.pop();
        if (t_tmp->left) q.push(t_tmp->left);
        if (t_tmp->right) q.push(t_tmp->right);
    }
    return treehead;
}

int main() {
    ListNode *ln = new ListNode(-10);
    ln->insert(-3);
    ln->insert(1);
    ln->insert(5);
    ln->insert(9);
    ln->insert(11);
    ln->insert(29);
    sortedListToBST(ln);
    ln->delete_(ln);
    return 0;
}
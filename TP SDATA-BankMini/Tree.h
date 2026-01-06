#ifndef TREE_H
#define TREE_H

#include <string>
using namespace std;

// Forward declaration
struct RekeningNode;

// =============================
// BINARY SEARCH TREE (BST)
// =============================

// Menambahkan rekening ke BST
RekeningNode* insertRekening(
    RekeningNode* root,
    string noRek,
    double saldo
);

// Mencari rekening di BST
RekeningNode* cariRekening(
    RekeningNode* root,
    string noRek
);

// Menampilkan BST secara inorder
void tampilInOrder(RekeningNode* root);

#endif

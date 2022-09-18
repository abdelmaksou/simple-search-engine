#ifndef TRIE_H
#define TRIE_H

#include <bits/stdc++.h>
#include <unordered_set>

using namespace std;

#define ALPHABET_SIZE 62

class Trie
{
public:
    bool isEndOfWord;
    vector<string> websites;
    Trie* children[ALPHABET_SIZE];

    // Constructor
    Trie()
    {
        this->isEndOfWord = false;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            this->children[i] = nullptr;
        }
    }

    void insert(string, string, bool);
    bool remove(Trie*&, string);
    vector<string> search(string, bool);
    bool isEmpty(Trie const*);
};

#endif
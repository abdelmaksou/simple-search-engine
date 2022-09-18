#include "Trie.h"

void Trie::insert(string key, string website, bool cased)
{
    if (cased)
    {
        Trie* curr = this;
        for (int i = 0; i < key.length(); i++) {
            int index = key[i] - 'A';
            if (!curr->children[index])
                curr->children[index] = new Trie();

            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
        curr->websites.push_back(website);
    }
    else
    {
        Trie* curr = this;
        for (int i = 0; i < key.length(); i++) {
            int index = tolower(key[i]) - 'a';
            if (!curr->children[index])
                curr->children[index] = new Trie();

            curr = curr->children[index];
        }

        curr->isEndOfWord = true;
        curr->websites.push_back(website);
    }
}

// Recursive function to delete a key in the Trie
bool Trie::remove(Trie*& root, string key)
{
    // return if Trie is empty
    if (root == nullptr) {
        return false;
    }

    // if the end of the key is not reached
    if (key.length())
    {
        // recur for the node corresponding to the next character in the key
        // and if it returns true, delete the current node (if it is non-leaf)

        if (root != nullptr &&
            root->children[key[0]] != nullptr &&
            remove(root->children[key[0]], key.substr(1)) &&
            root->isEndOfWord == false)
        {
            if (!isEmpty(root))
            {
                delete root;
                root = nullptr;
                return true;
            }
            else {
                return false;
            }
        }
    }

    // if the end of the key is reached
    if (key.length() == 0 && root->isEndOfWord)
    {
        // if the current node is a leaf node and doesn't have any children
        if (!isEmpty(root))
        {
            // delete the current node
            delete root;
            root = nullptr;

            // delete the non-leaf parent nodes
            return true;
        }

        // if the current node is a leaf node and has children
        else {
            // mark the current node as a non-leaf node (DON'T DELETE IT)
            root->isEndOfWord = false;

            // don't delete its parent nodes
            return false;
        }
    }

    return false;
}

vector<string> Trie::search(string key, bool cased)
{
    if (cased)
    {
        Trie* root = this;

        for (int i = 0; i < key.length(); i++) {
            int index = key[i] - 'A';
            if (root->children[index] == nullptr)
                break;

            root = root->children[index];
        }

        if (root != NULL && root->isEndOfWord)
        {
            unordered_set<string> s(root->websites.begin(), root->websites.end());
            vector <string> res;
            res.assign(s.begin(), s.end());
            return res;
        }
    }
    else
    {
        Trie* root = this;

        for (int i = 0; i < key.length(); i++) {
            int index = tolower(key[i]) - 'a';
            if (root->children[index] == nullptr)
                break;

            root = root->children[index];
        }

        if (root != NULL && root->isEndOfWord)
        {
            unordered_set<string> s(root->websites.begin(), root->websites.end());
            vector <string> res;
            res.assign(s.begin(), s.end());
            return res;
        }
    }

    vector<string> none;
    return none;
}

bool Trie::isEmpty(Trie const* curr)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (curr->children[i]) {
            return true;    // child found
        }
    }

    return false;
}

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int Max = 0;
vector<int> longestSuffixIndices;

class SuffixTree;

class Node{
public:
    friend SuffixTree;
    Node(Node* link, int start, int* end) : Node(link, start, end, -1,0,0){}
    Node(Node* link, int start, int* end, int ind, int word_1, int word_2) : suffix_link(link), start(start), end(end), suffix_index(ind), word_1(word_1), word_2(word_2){}

private:
    map<char, Node*> children; 
    Node* suffix_link;
    int start;
    int* end;
    int suffix_index;
    int word_1 = -1;
    int word_2 = -1;
};

class SuffixTree{
public:
    SuffixTree(string &text, int sep);

    void Depth();
private:
    Node* root = new Node(nullptr, -1, new int(-1));

    Node* last_internal_node = nullptr; 
    Node* activeNode = nullptr;  
    int active_phase = -1; 
    int matched_length = 0;
    int count_left_suffix = 0;
    int processed_end = -1;

    string text;
    int sep;
    void BuildSuffixTree(int pos);

    void findLongestSuffixes(Node* node, int d);
};

SuffixTree::SuffixTree(string &str, int separator) : text(str), sep(separator) {
    text += '$';
    activeNode = root;
    for (size_t i = 0; i < text.length(); i++) {
        BuildSuffixTree(i);
    }
}

void SuffixTree::BuildSuffixTree(int phase){
    last_internal_node = nullptr; 
    processed_end++;
    count_left_suffix++; 
    while (count_left_suffix > 0){
        if (matched_length == 0) {
            active_phase = phase;
        }
        auto find = activeNode->children.find(text[active_phase]);  

        if (find == activeNode->children.end()){
            if(phase - count_left_suffix + 1 > sep){
                activeNode->children.insert(make_pair(text[active_phase], new Node(root, phase, &processed_end, phase - count_left_suffix + 1, 0, 1)));
            }else{
                activeNode->children.insert(make_pair(text[active_phase], new Node(root, phase, &processed_end, phase - count_left_suffix + 1, 1, 0)));
            }
            if (last_internal_node != nullptr){
                last_internal_node->suffix_link = activeNode;
                last_internal_node = nullptr;
            }

        } else{
            Node* next = find->second;
            int edge_length = *(next->end) - (next->start) + 1;
            if (matched_length >= edge_length){
                active_phase += edge_length;
                matched_length -= edge_length;
                activeNode = next;
                continue;
            }
            if (text[next->start + matched_length] == text[phase]){
                matched_length++;
                if (last_internal_node != nullptr && activeNode != root) {
                    last_internal_node->suffix_link = activeNode;
                }
                break;
            }
            Node* split = new Node(root, next->start, new int(next->start + matched_length - 1));
            activeNode->children[text[active_phase]] = split;
            next->start += matched_length;
            split->children.insert(make_pair(text[next->start], next));
            if(phase - count_left_suffix + 1 > sep){
                split->children.insert(make_pair(text[phase], new Node(root, phase, &processed_end, phase - count_left_suffix + 1,0,1)));
            }else{
                split->children.insert(make_pair(text[phase], new Node(root, phase, &processed_end, phase - count_left_suffix + 1,1,0)));
            }
             if (last_internal_node != nullptr) {
                last_internal_node->suffix_link = split;
            }                                   

            last_internal_node = split;
        }
        if (activeNode != root) {
            activeNode = activeNode->suffix_link;
        } else if (matched_length > 0) {
            matched_length --;
            active_phase++;
        }
        count_left_suffix--;
    }
}


void SuffixTree::findLongestSuffixes(Node* node, int depth) {
    if (node->children.empty()) {
        return;
    }

    for (const auto& [_, child] : node->children) {
        int edgeLength = *child->end - child->start + 1;

        findLongestSuffixes(child, depth + edgeLength);
        node->word_1 |= child->word_1;
        node->word_2 |= child->word_2;
    }
    if (node->word_1 == 1 && node->word_2 == 1) {
        if (depth > Max) {
            Max = depth;
            longestSuffixIndices.clear();
            longestSuffixIndices.push_back(*node->end);
        } else if (depth == Max) {
            longestSuffixIndices.push_back(*node->end);
        }
    }
}

void SuffixTree::Depth(){
    findLongestSuffixes(root, 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    string word1, word2;
    cin >> word1 >> word2;
    string text = word1 + "#" + word2;
    int separator = static_cast<int>(word1.size());
    SuffixTree suffixTree(text, separator);
   suffixTree.Depth();
    cout << Max << '\n';
    for(auto i: longestSuffixIndices){
        for(int j = i - Max + 1; j < i + 1; j++){
            cout << text[j];
        }
        cout << '\n';
    }
    return 0;
}
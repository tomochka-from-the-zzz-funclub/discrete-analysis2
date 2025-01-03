#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

const short MAX_SIZE = 257;

struct word {
    char key[MAX_SIZE] = "";
    unsigned long long value = 0;
    friend bool operator>(const word& left, const word& right);
	friend bool operator<(const word& left, const word& right);
	friend bool operator==(const word& left, const word& right);
	friend bool operator>=(const word& left, const word& right);
	friend bool operator<=(const word& left, const word& right);
	friend ostream& operator<<(ostream& os, const word& w);
	void lower_case() {
		for (int i = 0; i < strlen(key); ++i) {
			if (key[i] >= 'A' && key[i] <= 'Z')
				key[i] += 32;
		}
	}
};
ostream& operator<<(ostream& os, const word& w) {
    os << w.key << " " << w.value;
    return os;
}

bool operator>(const word& left, const word& right) {
	if (strcmp(left.key, right.key) > 0) {
		return true;
	} 
	return  false;
}

bool operator<(const word& left, const word& right) {
	if (strcmp(left.key, right.key) < 0) {
		return true;
	} 
	return  false;
}

bool operator==(const word& left, const word& right) {
	if (strcmp(left.key, right.key) == 0) {
		return true;
	} 
	return  false;
}

template<int T> 
struct my_btree_node {
    int quantity;
    word* words;
    my_btree_node<T>** children; 
    bool leaf;

    my_btree_node();
    my_btree_node<T>* my_btree_node_search(word); 
    void my_btree_split_child(int, my_btree_node<T>*);
    void regular_insert(word); 
    int find_word(word find_word);
    void my_btree_node_delete_word(char*); 
    void my_btree_node_delete_from_leaf(int);
    void my_btree_node_delete_from_noleaf(int);
    void my_btree_node_filling(int);
    void my_btree_node_steal_right(int); 
    void my_btree_node_steal_left(int); 
    void my_btree_node_merge_brother(int);
    void my_btree_node_destroy();
    void my_btree_node_save(ofstream&);
    void my_btree_node_print(ostream& out, int c); 
};

template<int T> 
my_btree_node<T>::my_btree_node() : quantity(0), leaf(true) {
    words = new word[2 * T - 1];
    children = new my_btree_node<T>*[2 * T];
    for (int i = 0; i < 2 * T; ++i) {
        children[i] = nullptr;
    }
}

template<int T>
int my_btree_node<T> :: find_word(word find_word) {
    unsigned left = -1;
    unsigned right = quantity;
    while (left + 1 < right) {
        unsigned mid = (left + right) / 2;
        if (words[mid] < find_word) {
                left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

template<int T> 
my_btree_node<T>* my_btree_node<T>::my_btree_node_search(word search_word) {
    unsigned i = find_word(search_word);

    if (i < quantity && strcmp(search_word.key, words[i].key) == 0) {
        return this;
    }

    if (leaf == true) {
        return nullptr;
    }
    return children[i]->my_btree_node_search(search_word);
}  

template<int T> 
void my_btree_node<T>::my_btree_split_child(int index, my_btree_node<T>* child) {
    // index это индекс, в который попадет ключ из дочернего узла
    // full_node это дочерний узел, который мы разделяем
    
    // создаем новый узел
    my_btree_node<T>* part = new my_btree_node<T>();
    part->leaf = child->leaf;
    part->quantity = T - 1;
    // копируем вторую половину старого узла в новый
    for (int i = 0; i < T - 1; ++i) {
        part->words[i] = child->words[i + T];
    }
    // если есть дети, то их тоже перепривязываем
    if (child->leaf == false) {
        for (int i = 0; i < T; ++i) {
            part->children[i] = child->children[i+T];
        }
    }
    
    child->quantity = T - 1;

    for (int i = quantity; i >= index + 1; --i) {
        children[i + 1] = children[i];
    }
    // теперь new_node стал правым ребенком, а full_node стал левым 
    children[index + 1] = part;
    // сдвигаю ключи вправо, чтобы поставить новый ключ
    for (int i = quantity - 1; i >= index; --i) {
        words[i + 1] = words[i];
    }
    // ставим в родительский узел середину дочернего узла
    words[index] = child->words[T - 1];
    ++quantity;
}

template<int T> 
void my_btree_node<T>::regular_insert(word insert_word) {
    int i = quantity - 1;

    if (leaf) {
        while (i >= 0 && (insert_word < words[i])) {
            words[i+1] = words[i];
            --i;
        }
        words[i + 1] = insert_word;
        ++quantity;
    } else {
        while (i >= 0 && (insert_word < words[i])) {
            --i;
        }
        // если ребенок заполнен
        if (children[i+1]->quantity == 2*T-1) {
            this->my_btree_split_child(i + 1, children[i+1]);

            if (strcmp(insert_word.key, words[i+1].key) > 0) {
                ++i;
            }
        }
        children[i+1]->regular_insert(insert_word);
    }
}

template<int T> 
void my_btree_node<T>::my_btree_node_delete_word(char* key) {
    int idx = 0;
    while (idx < quantity && strcmp(key, words[idx].key) > 0) {
        ++idx;
    }

    if (idx < quantity && strcmp(key, words[idx].key) == 0) {
        if (leaf) {
            my_btree_node_delete_from_leaf(idx);
        } else {
            my_btree_node_delete_from_noleaf(idx); 
        }
    } else {
        if (children[idx]->quantity < T) {
            my_btree_node_filling(idx);
        }

        if (idx > quantity) {
            children[idx-1]->my_btree_node_delete_word(key);
        } else {
            children[idx]->my_btree_node_delete_word(key);
        }
    }
}

template<int T> 
void my_btree_node<T>::my_btree_node_delete_from_leaf(int index) {
    for (int i = index + 1; i < quantity; i++) {
        words[i - 1] = words[i];
    }
    --quantity;
}

template<int T> 
void my_btree_node<T>::my_btree_node_delete_from_noleaf(int index) {
    char key[MAX_SIZE]; 
    strcpy(key, words[index].key);
    word new_parent;

    if (children[index]->quantity >= T) {
        my_btree_node<T>* left = children[index];
        while (!left->leaf) {
            left = left->children[left->quantity];
        }
        new_parent = left->words[left->quantity - 1];
        words[index] = new_parent;
        children[index]->my_btree_node_delete_word(new_parent.key);
    } else if (children[index+1]->quantity >= T) {
        my_btree_node<T>* right = children[index+1];
        while(!right->leaf) {
            right = right->children[0];
        }
        new_parent = right->words[0];
        words[index] = new_parent;
        children[index + 1]->my_btree_node_delete_word(new_parent.key);
    } else {
        my_btree_node_merge_brother(index);
        children[index]->my_btree_node_delete_word(key);
    }
}

template<int T>
void my_btree_node<T>::my_btree_node_filling(int index) {
    if (index != 0 && children[index - 1]->quantity >= T) {
        my_btree_node_steal_left(index);
    } else if (index != quantity && children[index + 1]->quantity >= T) {
        my_btree_node_steal_right(index);
    } else {
        if (index != quantity) {
            my_btree_node_merge_brother(index);
        } else {
            my_btree_node_merge_brother(index - 1);
        }
    }
}

template<int T>
void my_btree_node<T>::my_btree_node_steal_right(int index) {
    my_btree_node<T>* child = children[index];
    my_btree_node<T>* sibling = children[index + 1];

    child->words[child->quantity] = words[index];
    if (!child->leaf) {
        child->children[child->quantity + 1] = sibling->children[0];
    }
    words[index] = sibling->words[0];
    for (int i = 0; i < sibling->quantity - 1; ++i) {
        sibling->words[i] = sibling->words[i + 1];
    }
    if (!sibling->leaf) {
        for (int i = 0; i < sibling->quantity; ++i) {
            sibling->children[i] = sibling->children[i + 1]; 
        }
    }
    child->quantity += 1;
    sibling->quantity -= 1;
}

template<int T>
void my_btree_node<T>::my_btree_node_steal_left(int index) {
    my_btree_node<T>* child = children[index];
    my_btree_node<T>* sibling = children[index - 1];

    for (int i = child->quantity; i >= 1; --i) {
        child->words[i] = child->words[i - 1];
    }
    child->words[0] = words[index - 1];
    if (!child->leaf) {
        for (int i = child->quantity + 1; i >= 1; --i) {
            child->children[i] = child->children[i - 1];
        }
        child->children[0] = sibling->children[sibling->quantity];
    }
    child->words[0] = words[index - 1];
    words[index - 1] = sibling->words[sibling->quantity - 1];
    
    child->quantity += 1;
    sibling->quantity -= 1;
}

template<int T>
void my_btree_node<T>::my_btree_node_merge_brother(int index) {
    my_btree_node<T>* left_child = children[index];
    my_btree_node<T>* right_child = children[index + 1];

    left_child->words[T-1] = words[index];
    for (int i = 0; i < T - 1; ++i) {
        left_child->words[i + T] = right_child->words[i]; 
    }   
    if (!left_child->leaf) {
        for (int i = 0; i < T; ++i) {
            left_child->children[i + T] = right_child->children[i];
        }
    }
    for (int i = index + 1; i < quantity; ++i) {
        words[i - 1] = words[i];
    }

    for (int i = index + 2; i <= quantity; ++i) {
        children[i - 1] = children[i];
    }

    left_child->quantity = 2*T - 1;
    --quantity;
    delete[] right_child->words;
    delete[] right_child->children;
    delete right_child;
}

template<int T>
void my_btree_node<T>::my_btree_node_destroy() {
    delete[] words;
    if (children[0] == nullptr) {
        delete[] children;
        return;
    }
    for (int i = 0; i <= quantity; ++i) {
        if (children[i] != nullptr) {
            children[i]->my_btree_node_destroy();
        }
        delete children[i];
    }
    delete[] children;
} 

template<int T> 
void my_btree_node<T>:: my_btree_node_print(ostream& out, int c) {
    for (int i = 0; i < c; ++i)
        cout << '\t';
    for (int i = 0; i < this->quantity; ++i) {
        out << this->words[i] << ' ';
    }
    out << endl;
    for (int i = 0; i <= this->quantity; ++i) {
        if (this->children[i]) {
            this->children[i]->my_btree_node_print(out, c + 1);
        }
    }
}

template<int T>
class my_btree {
    public:
    my_btree_node<T>* root;
    my_btree();
    void my_btree_on_load(word);
    void my_btree_save(char*);
    void my_btree_load(char*);
    ~my_btree();
};

template<int T> 
my_btree<T>::my_btree() {
    root = nullptr;
}

template<int T> 
my_btree<T>::~my_btree() {
    if (root != nullptr) {
        root->my_btree_node_destroy();
    }
    delete root;
}

template<int T> 
void my_btree_node<T>::my_btree_node_save(ofstream &out) {
    if (children[0] == nullptr) {
        for (int i = 0; i < quantity; ++i) {
            out.write(words[i].key, sizeof(char) * (strlen(words[i].key) + 1));  
            out.write((char*)&words[i].value, sizeof(long long int));
        }
    } else {
        for (int i = 0; i < quantity; ++i) {
            children[i]->my_btree_node_save(out);
            out.write(words[i].key, sizeof(char) * (strlen(words[i].key) + 1));  
            out.write((char*)&words[i].value, sizeof(long long int));
        }
        children[quantity]->my_btree_node_save(out);
    }
}

template<int T> 
void my_btree<T>::my_btree_save(char* path) {
    ofstream out(path, ios::binary);
    short is_tree = 1;
    if (root == nullptr) {
        is_tree = 0;
        out.write((char*)&is_tree, sizeof(short));
        return;
    }

    out.write((char*)&is_tree, sizeof(short));
    root->my_btree_node_save(out);
    char end_token = '$';
    out.write((char*)&end_token, sizeof(char));
    out.close();
}

template<int T> 
void my_btree<T>::my_btree_load(char* path) {
    if (root != nullptr) {
        root->my_btree_node_destroy();
        delete root;
        root = nullptr;
    }

    ifstream in(path, ios::binary);
    short is_tree;
    in.read((char*)&is_tree, sizeof(short));
    char c;
    if (is_tree) {
        while (true) {
            in.read((char*)&c, sizeof(char));
            if (c == '$') {
                break;
            }
            word inserted_elem;
            inserted_elem.key[0] = c;
            for (int i = 1; c != '\0'; ++i) {
                in.read((char*)&c, sizeof(char));
                inserted_elem.key[i] = c;
            }
            in.read((char*)&inserted_elem.value, sizeof(unsigned long long));
            this->my_btree_on_load(inserted_elem);
            
        }
    }
    in.close();
}

template<int T> 
void my_btree<T>::my_btree_on_load(word elem) {
    if (root == nullptr) {
        root = new my_btree_node<T>;
        root->leaf = true;
        root->words[0] = elem;
        root->quantity = 1;
        return;
    }
    if (root->quantity == 2*T - 1) {
        my_btree_node<T>* new_root = new my_btree_node<T>;
        new_root->leaf = false;
        new_root->children[0] = root;
        new_root->my_btree_split_child(0, root);
        int i = 0;
        if (strcmp(elem.key, new_root->words[0].key) > 0) {
            ++i;
        }
        new_root->regular_insert(elem);
        root = new_root;
    } else { 
        root->regular_insert(elem);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 
    cout.tie(nullptr);
    const int T = 3;
    my_btree<T> btree;
    word word_v;
    char path[MAX_SIZE];
    while(cin >> word_v.key) {
        switch (word_v.key[0]) {
            case '+':
                cin >> word_v.key >> word_v.value;
                word_v.lower_case();
                if (btree.root == nullptr) {
                    btree.root = new my_btree_node<T>;
                    btree.root->leaf = true;
                    btree.root->words[0] = word_v;
                    btree.root->quantity = 1;
                    cout << "OK\n";
                    break;
                }
                if (btree.root->my_btree_node_search(word_v) != nullptr) {
                    cout << "Exist\n";
                    break;
                }
                if (btree.root->quantity == 2*T-1) {
                    my_btree_node<T>* new_root = new my_btree_node<T>;
                    new_root->leaf = false;
                    new_root->children[0] = btree.root;
                    new_root->my_btree_split_child(0, btree.root);
                    int i = 0;
                    if (strcmp(word_v.key, new_root->words[0].key) > 0) {
                        ++i;
                    }
                    new_root->regular_insert(word_v);
                    btree.root = new_root;
                } else { 
                    btree.root->regular_insert(word_v);
                }
                cout << "OK\n";
                break;
            case '-':
                cin >> word_v.key;
                for (int i = 0; i < strlen(word_v.key); ++i) {
			        if (word_v.key[i] >= 'A' && word_v.key[i] <= 'Z')
                        word_v.key[i] += 32;
                }
                if (btree.root == nullptr) {
                    cout << "NoSuchWord\n";
                    break;
                }
                
                if (btree.root->my_btree_node_search(word_v) == nullptr) {
                    cout << "NoSuchWord\n";
                    break;
                }

                btree.root->my_btree_node_delete_word(word_v.key);
                if (btree.root->quantity == 0) {
                    my_btree_node<T>* old_root = btree.root;
                    if (btree.root->leaf) {
                        btree.root = nullptr;
                    } else {
                        btree.root = btree.root->children[0];
                    }
                    delete[] old_root->words;
                    delete[] old_root->children;
                    delete old_root;
                }
                cout << "OK\n";
                break;
            case '!':
                cin >> word_v.key >> path;
                if (strcmp(word_v.key, "Save") == 0) {
                    btree.my_btree_save(path);
                } else {
                    btree.my_btree_load(path);
                }
                cout << "OK\n";
                break;
            default:
                for (int i = 0; i < strlen(word_v.key); ++i) {
			        if (word_v.key[i] >= 'A' && word_v.key[i] <= 'Z')
                        word_v.key[i] += 32;
                }
                if (btree.root == nullptr) {
                    cout << "NoSuchWord\n";
                    break;
                }

                my_btree_node<T>* result = btree.root->my_btree_node_search(word_v);
                if (result == nullptr) {
                    cout << "NoSuchWord\n";
                } else {
                    int i = 0;
                    while (i < result->quantity && strcmp(word_v.key, result->words[i].key) > 0) {
                        i++;
                    }
                    cout << "OK: " << result->words[i].value << "\n";
                }
                break;
        }
    }
    return 0;
}
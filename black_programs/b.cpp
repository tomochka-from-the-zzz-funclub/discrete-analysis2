#include<iostream>
#include<fstream>
#include<cctype>
#include<string.h>
using namespace std;

const int MAX_SIZE = 257;

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
    ~my_btree_node();
    word* my_btree_node_search(word search_word);
    void my_btree_split_child(my_btree_node<T>* child, int i); // хор
    void regular_insert(word insert_word); //хор
    int find_word(word find_word) ;//
    void my_btree_node_delete_word(word delete_word); // хор, но с изменением
    void my_btree_node_delete_from_leaf(int index); // ХОР
    void my_btree_node_delete_from_noleaf(int index);//хор
    word my_btree_node_get_left (int index);// хор
    word my_btree_node_get_right(int index); // хор, с изменениями
    void my_btree_node_filling(int index) ;// хор
    void my_btree_node_steal_left(int index); // хор
    void my_btree_node_steal_right(int index);    // хор
    void my_btree_node_merge_brother(int index); //  хор, но есть вопрос
    void my_btree_node_destroy();
    void my_btree_node_load(fstream& in);
    void my_btree_node_save(fstream& out);
    void  my_btree_node_print(ostream& out, int c);
};

template<int T> 
my_btree_node<T> :: my_btree_node() : quantity(0), leaf(true) { 
    words = new word[2 * T - 1];
    children = new my_btree_node<T> * [2 * T];
    for (int i = 0; i < 2 * T; ++i)
        children[i] = nullptr;
}

template<int T>
my_btree_node<T> ::~my_btree_node() {
    if (words) {
        delete[] words;
        words = nullptr;
    }
    if (!children) {
        delete[] children;
        children = nullptr;
    }
    quantity = 0;
}

template<int T>
word* my_btree_node<T> :: my_btree_node_search(word search_word) { 
    if (this) {
        unsigned index = find_word(search_word);
        if ((index < quantity) && (search_word == words[index])) {
            return &words[index];
        }
        if (leaf) {
            return nullptr;
        } else {
            return children[index]->my_btree_node_search(search_word);
        }
    }
    return nullptr;
}

template<int T>
void my_btree_node<T> :: my_btree_split_child(my_btree_node<T>* child, int index) { 
    my_btree_node<T>* part = new my_btree_node<T>();
    part->leaf = child->leaf;
    part->quantity = T - 1;
    for (int j = 0; j < T - 1; ++j) {
        part->words[j] = child->words[j + T];
    }
    if (!(child->leaf)) {
        for (int j = 0; j < T; ++j) {
            part->children[j] = child->children[j + T];
        }
    }
    child->quantity = T - 1;
    for (int j = T; j >= index + 1; --j) {
        children[j+1] = children[j];
    }
    children[index + 1] = part;
    for (int j = quantity - 1; j >= index; --j) {
        words[j + 1] = words[j];
    }
    words[index] = child->words[T - 1];
    ++quantity;
}

template<int T>
void my_btree_node<T> :: regular_insert(word insert_word) {
    int i = quantity - 1;
    if (leaf) {
        while ((i >= 0) && (insert_word < words[i])) {
            words[i + 1] = words[i];
            --i;
        }
        words[i + 1] = insert_word;
        ++quantity;
    } else {
        while ((i >= 0) && (insert_word < words[i])) {
            --i;
        }
        ++i;
        if (children[i]->quantity == (2 * T - 1)) {
            my_btree_split_child(children[i], i);
            if (insert_word > words[i]) {
                ++i;
            }
        }
        children[i]->regular_insert(insert_word);
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
void my_btree_node<T> :: my_btree_node_delete_from_leaf(int index) {
    for (int i = index + 1; i < quantity; ++i) {
        words[i - 1] = words[i];
    }
    --quantity;
}

template<int T>
void my_btree_node<T> :: my_btree_node_delete_from_noleaf(int index) {
    word delete_word = words[index];

    if (children[index]->quantity >= T) {
        word left = my_btree_node_get_left(index);
        words[index] = left;
        children[index]->my_btree_node_delete_word(left);
    } else if (children[index + 1]->quantity >= T) {
        word right = my_btree_node_get_right(index);
        words[index] = right;
        children[index + 1]->my_btree_node_delete_word(right);
    } else {
        my_btree_node_merge_brother(index);
        children[index]->my_btree_node_delete_word(delete_word);
    }
    
}

template<int T>	
word my_btree_node<T> :: my_btree_node_get_left(int index) {
    my_btree_node<T>* current = children[index];
    while (!current->leaf) {
        current = current->children[current->quantity];
    }
    return current->words[current->quantity - 1];
}

template<int T>
word my_btree_node<T> :: my_btree_node_get_right(int index) {
    my_btree_node<T>* current = children[index + 1];
    while (!current->leaf) {
        current = current->children[0];
    }
    return current->words[0];
}

template<int T>	
void my_btree_node<T> :: my_btree_node_filling(int index) { 
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
void my_btree_node<T> :: my_btree_node_steal_left(int index) {
    my_btree_node<T>* child = children[index];
    my_btree_node<T>* sibling = children[index - 1];

    for (int i = child->quantity ; i >= 1; --i) {
        child->words[i] = child->words[i - 1];
    }
    if (!child->leaf) {
        for (int i = child->quantity; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
        child->children[0] = sibling->children[sibling->quantity];
    }
    child->words[0] = words[index - 1];
    words[index - 1] = sibling->words[sibling->quantity - 1];
    child->quantity += 1;
    sibling->quantity-= 1;
}

template<int T>
void my_btree_node<T> :: my_btree_node_steal_right(int index) {
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
        for (int i = 0; i <= sibling->quantity ; ++i) {
            sibling->children[i] = sibling->children[i + 1];
        }
    }
    child->quantity += 1;
    sibling->quantity-= 1;
}

template<int T>
void my_btree_node<T> :: my_btree_node_merge_brother(int index) {  
    my_btree_node<T>* left_child = children[index];
    my_btree_node<T>* right_child = children[index + 1];
    left_child->words[T - 1] = words[index];

    for (int i = 0; i < T; ++i) {
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
    delete right_child; // ?нужно ли удалять отдельно 
}

template<int T>
void my_btree_node<T> :: my_btree_node_delete_word(word delete_word) {
    int index = find_word(delete_word);//?
    if (index < quantity && words[index] == delete_word) {
        if (leaf) {
            my_btree_node_delete_from_leaf(index);
        } else {
            my_btree_node_delete_from_noleaf(index);
        }
    } else {
        // if (leaf) {
        //     cout << "not found" << endl;
        //     return;
        // }
        // bool flag;
        // if (index == quantity) {
        //     flag = true;
        // } else {
        //     flag = false;
        // }
        if (children[index]->quantity < T) {
            my_btree_node_filling(index);
        }
        if (index > quantity) {//if (flag && index > quantity) {
            children[index - 1]->my_btree_node_delete_word(delete_word);
        } else {
            children[index]->my_btree_node_delete_word(delete_word);
        }
    }
}

template<int T>
void my_btree_node<T> :: my_btree_node_destroy() {
    for (int i = 0; i <= quantity; ++i) {
        if (children[i]) {
            children[i]->my_btree_node_destroy();
            delete children[i];
            children[i] = nullptr;
        }
    }
}

template<int T>
void my_btree_node<T> :: my_btree_node_load(fstream& in) {
    in.read((char*)&(this->quantity), sizeof(int));
    in.read((char*)&(this->leaf), sizeof(bool));
    for (int i = 0; i < quantity; ++i) {
        in.read((char*)&(this->words[i]), sizeof(word));
    }
    if (!leaf) {
        for (int i = 0; i <= quantity; ++i) {
            children[i] = new my_btree_node<T>();
            children[i]->my_btree_node_load(in);
        }
    }
}

template<int T>
void my_btree_node<T> :: my_btree_node_save(fstream& out) {
    out.write((char*)&(this->quantity), sizeof(int));
    out.write((char*)&(this->leaf), sizeof(bool));
    for (int i = 0; i < quantity; ++i) {
        out.write((char*)&(this->words[i]), sizeof(word));
    }
    if (!leaf) {
        for (int i = 0; i <= quantity; ++i) {
            children[i]->my_btree_node_save(out);
        }
    }
}

template<int T> 
struct my_btree {
    my_btree_node<T>* root;
    my_btree();
    ~my_btree() ;
    void my_btree_node_print(ostream& out, int c) ;
    word* my_btree_search(word searh_word) ;
    void my_btree_insert(word insert_word);
    void my_btree_node_delete_word(word delete_word) ;
    //void my_btree_node_print(ostream& out);
    void my_btree_save(fstream& out);
    void my_btree_node_load(fstream& in);
};

template<int T> 
my_btree<T>:: my_btree() {
    root = new my_btree_node<T>();
}

template<int T> 
my_btree<T>:: ~my_btree() {
    if (root) {
        root->my_btree_node_destroy();
        delete root;
        root = nullptr;
    }
}

template<int T> 
word* my_btree<T> :: my_btree_search(word searh_word) {
    if (root) {
        return root->my_btree_node_search(searh_word);
    }
    return nullptr;
}

template<int T> 
void my_btree<T> :: my_btree_insert(word insert_word) {
    if (!root)
        root = new my_btree_node<T>();
    if (root->quantity == (2 * T - 1)) {
        my_btree_node<T>* s = new my_btree_node<T>();
        s->leaf = false;
        s->children[0] = root;
        s->my_btree_split_child(root, 0);
        root = s;
        root->regular_insert(insert_word);
    } else {
        root->regular_insert(insert_word);
    }
}

template<int T> 
void my_btree<T> :: my_btree_node_delete_word(word delete_word) {
    if (!root) {
        cout << "empty\n";
        return;
    }
    root->my_btree_node_delete_word(delete_word);
    if (root->quantity == 0) {
        my_btree_node<T>* tmp = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        } if (tmp)
            delete tmp;
    }
}

template<int T> 
void my_btree<T> :: my_btree_save(fstream& out) {
    if (root) {
        root->my_btree_node_save(out);
    }
}

template<int T> 
void my_btree<T> :: my_btree_node_load(fstream& in) {
    if (in.peek() == EOF)
        return;
    if (root) {
        root->my_btree_node_destroy();
        delete root;
    }
    root = new my_btree_node<T>();
    root->my_btree_node_load(in);
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



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    const int T = 3;
    cout.tie(nullptr);
	my_btree<T> btree;
	word buffer;
	while (cin >> buffer.key) {
		if (buffer.key[0] == '+') {
			cin >> buffer.key >> buffer.value;
			buffer.lower_case();
            if (btree.root == nullptr) {
                btree.root = new my_btree_node<T>;
                btree.root->words[0] = buffer;
                btree.root->quantity = 1;
                cout << "OK\n";
                return;
            }
			if (btree.my_btree_search(buffer)) {
				cout << "Exist\n";
				continue;
			}
			btree.my_btree_insert(buffer);
		    //cout << "Результат для " << i <<  
            cout << "OK\n";
            //btree.root->my_btree_node_print(cout, 0);
		}
		else if (buffer.key[0] == '-') {
			cin >> buffer.key;
			buffer.lower_case();
			if (!btree.my_btree_search(buffer)) {
                //btree.root->my_btree_node_print(cout, 0);
				cout << "NoSuchWord\n";
				continue;
			}
			btree.my_btree_node_delete_word(buffer);
			cout << "OK\n";
		}
		else if (buffer.key[0] == '!') {
			cin >> buffer.key;
			if (strcmp(buffer.key, "Load") == 0) {
				cin >> buffer.key;
				fstream file(buffer.key, ios::binary|ios::in);
				btree.my_btree_node_load(file);
				file.close();
				cout << "OK\n";
			}
			else if (strcmp(buffer.key, "Save") == 0) {
				cin >> buffer.key;
				fstream file(buffer.key, ios::binary | ios::out);
				file.sync_with_stdio(false);
				btree.my_btree_save(file);
				file.close();
				cout << "OK\n";
			}
		}
		else {
			buffer.lower_case();
			word* result = btree.my_btree_search(buffer);
			if (result) {
				cout << "OK: " << result->value << endl;
			}
			else {
				cout << "NoSuchWord\n";
			}
		}
	}
	return 0;
}
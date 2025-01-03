#include <sstream>
#include <cstring>
#include <iostream>
const int M = 20;
using namespace std;
struct  data_{
	char str[2048];
	friend ostream& operator<< (ostream &out, const data_ &point) {
		out << point.str;
		return out;
	}
};

struct key {
    char key_value[M];
	//short len;
    int id;
    key() {}
	key(char k[M], int i) {
		for (int i = 0; i < M; i++) {
			key_value[i] = k[i];
		}
		id = i;
		//len = l;
	}
};
struct val{
	char small_key[M+1];
	val() {}
	val(char* v) {
		//cout << v << endl;
		for (int i = 0; i < strlen(v); i++) {
			small_key[i] = v[i];
			//cout << i << " " << small_key[i] << endl;
		}
		small_key[strlen(v)] = '\0';
		//cout << strlen(v) << endl;
	}
};

template<class T> class TVector {
private:
	T* record;
	size_t size;
	size_t capacity;
public:
	TVector() : record(nullptr), size(0), capacity(0) {};
	TVector(size_t s, const T val = T()) {
		record = new T[s];
		size = s;
		capacity = s;
		for (int i = 0; i < (int)s; i++)
			record[i] = val;
	}
	~TVector() {
		size = 0;
		capacity = 0;
		delete[] record;
		record = nullptr;
	}
	T* begin() const {
		return record;
	}
	T* end() const {
		return record ? record + size : nullptr;
	}
	size_t get_size() const {
		return size;
	}

	size_t get_capacity() const {
		return capacity;
	}

	T& get_elem(int index) const {
		if (index < 0 || index >= (int)size) {
			throw out_of_range("overflow!");
		}
		return record[index];
	}

	T& operator[](int index) const {
		return get_elem(index);
	}

	void push_back(T&& value) {
		if (size == capacity) {
			T* help = record;
			size == 0 ? capacity = 1 : capacity *= 2;
			record = new T[capacity];
			for (size_t i = 0; i < size; i++)
				record[i] = help[i];
			delete[] help;
		}
		record[size++] = value;
		return;
	}
	TVector<T>& operator= (const TVector<T>& other) {
		if (size != other.get_size()) {
			delete[] record;
			record = new T[other.get_capacity()];
			capacity = other.get_capacity();
			size = other.size;
		}
		for (int i = 0; i < (int)size; i++)
			record[i] = other.record[i];
		return *this;
	}
};

int c[10];
void radix_sort(TVector<key>& arr) {
	TVector<key> b = TVector<key>(arr.get_size());
	for (int j = M-1; j >= 0; j--) {
        for (int i = 0; i <  10; i++) {
			c[i] = 0;
		}
        for (short i = 0; i < arr.get_size(); ++i) {
            ++c[arr[i].key_value[j] - '0'];
        }
        for (short i = 1; i < 10; ++i) {
            c[i] += c[i - 1];
        }
        for (short i = arr.get_size() - 1; i >= 0; --i) {
                b[c[arr[i].key_value[j] - '0'] - 1] = arr[i];
				c[arr[i].key_value[j] - '0']--;
        }
		//arr = b;
	}
	arr = b;	
}

int main() {
    ios_base::sync_with_stdio(false);
    TVector<key> base_key;
    TVector<data_> base_data;
	TVector<val> val_key;
	
    char* small_key = new char [M];
	char* big_key;
	data_ test_data;
	//char big_key[M];

    int i = 0; 
    while (scanf("%s", small_key) != EOF) { 
		//cout << small_key << endl;
		val_key.push_back(val(small_key));
		//cout << val_key[i].small_key << endl;
        if (strlen(small_key) < M) {
            big_key = new char [M];
			for (short j = 0; j <M; ++j) {
            	if (j < M - strlen(small_key)) {
                	big_key[j] = '0';
            	} else {
					big_key[j] = small_key[j - M + strlen(small_key)];
				}
			}
			base_key.push_back(key(big_key, i));
        } else {
            base_key.push_back(key(small_key, i));
        }

		scanf("%s", test_data.str);

		base_data.push_back(data_(test_data));
        ++i;
		delete[] small_key;
		//delete[] big_key;
		char* small_key = new char [M];
		
    }
	// for(int i = 0; i < val_key.get_size(); i++) {
	// 	cout << val_key[i].small_key <<  endl;
	// }
    radix_sort(base_key);
	// for(int i = 0; i < val_key.get_size(); i++) {
	// 	cout << val_key[i] <<  endl;
	// }
    for (unsigned int i = 0; i < (unsigned int)base_key.get_size(); ++i) {
	 	cout << val_key[base_key[i].id].small_key<< '\t' << base_data[base_key[i].id] << '\n';
		//cout << base_key[i].key_value << endl;
	}
    return 0;
}
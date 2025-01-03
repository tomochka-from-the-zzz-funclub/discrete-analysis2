//вариант H.3-2

#include <string>
#include <iostream>

using namespace std;

short MAX = 21;
struct key {
    unsigned long long key_value;
    int id;
    key() {}
    key(unsigned long long key_, int i){
        key_value = key_;
        id = i;
    }
};

struct data_str {
   	string str_value;
	data_str() {}
	data_str(istream& in) {
		in >> str_value;
	}
	data_str(string s){
		str_value = s;
	}
};


template<class T> 
class TVector {
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
		if (index < 0 || index >= static_cast<int>(size)) {
			throw std::out_of_range("overflow!");
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

int get_digit(unsigned long long number, int digit_position) {
    return (number >> digit_position * 8) & 255;
}

 void radix_sort(TVector<key>& arr) {
        for (int byte = 0; byte < 8; ++byte) {
        int count[256] {0};
        for (int i = 0; i < arr.get_size(); ++i) {
            count[(arr[i].key_value >> byte * 8) & 255]++;
        }
        for (int i = 1; i < 256; ++i) {
            count[i] += count[i-1];
        }

        TVector<key> sorted = TVector<key>(arr.get_size());
        for (int i = 0; i < arr.get_size(); ++i) {
            sorted[i].key_value = arr[i].key_value;
			sorted[i].id = arr[i].id;
        }

        for (int i = arr.get_size() - 1; i >= 0; --i) {
			arr[count[get_digit(sorted[i].key_value, byte)] - 1].key_value = sorted[i].key_value;
			arr[count[get_digit(sorted[i].key_value, byte)] - 1].id = sorted[i].id;
            count[get_digit(sorted[i].key_value, byte)]  --;
        
        }
    }
 }

int main() {
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);
    TVector<key> base_key;
    TVector<data_str> base_string;
    unsigned long long test_key;
    int i = 0;

    while (cin >> test_key) {
        base_key.push_back(key(test_key, i));

        base_string.push_back(data_str(cin));
		i++;
	}

    radix_sort(base_key);

    for (int i = 0; i < base_key.get_size(); ++i) {
        cout << base_key[i].key_value << '\t' << base_string[ base_key[i].id].str_value << '\n';
    }

    return 0;
}
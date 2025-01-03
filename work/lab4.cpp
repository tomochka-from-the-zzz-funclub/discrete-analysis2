//вариант 0

#include <iostream>
#include <string>
#include <cstring>
using namespace std;
template<typename T>
class TVector {
private:
    T* data;
    int capacity;
    int cur_size;

    void increase_capacity() {
        int new_capacity = capacity ? capacity * 2 : 8;
        T* new_data = new T[new_capacity];
        memcpy(new_data, data, cur_size * sizeof(T));
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    TVector() : capacity(0), cur_size(0), data(nullptr) {}

    TVector(int size) : capacity(size), cur_size(size) {
        data = new T[capacity];
    }

    TVector(const TVector& other) : capacity(other.cur_size), cur_size(other.cur_size) {
        data = new T[capacity];
        memcpy(data, other.data, cur_size * sizeof(T));
    }

    TVector(TVector&& other) noexcept : data(other.data), capacity(other.capacity), cur_size(other.cur_size) {
        other.data = nullptr;
        other.capacity = 0;
        other.cur_size = 0;
    }

    TVector& operator=(const TVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.cur_size;
            cur_size = other.cur_size;
            data = new T[capacity];
            memcpy(data, other.data, cur_size * sizeof(T));
        }
        return *this;
    }

    TVector& operator=(TVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            cur_size = other.cur_size;
            other.data = nullptr;
            other.capacity = 0;
            other.cur_size = 0;
        }
        return *this;
    }

    ~TVector() {
        delete[] data;
    }

    void push_back(T elem) {
        if (cur_size == capacity) {
            increase_capacity();
        }
        data[cur_size++] = elem;
    }

    int get_size() const {
        return cur_size;
    }

    void resize(int new_size) {
        T* new_data = new T[new_size];
        memcpy(new_data, data, min(cur_size, new_size) * sizeof(T));
        delete[] data;
        data = new_data;
        capacity = new_size;
        cur_size = new_size;
    }

    T& operator[](int id) {
        return data[id];
    }

    void insert(size_t index, const T& value) {
        if (index > cur_size) {
            increase_capacity();

        }

        if (cur_size == capacity) {
            increase_capacity();
        }

        for (size_t i = cur_size; i > index; i--) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        cur_size++;
    }
};

TVector<int> z_func(const string& str) {
    TVector<int> result(str.length());
    int left = 0, right = 0;
    result[0] = str.length();
    for (int i = 1; i < str.length(); ++i) {
        if (i <= right){
            result[i] = min(result[i - left], right - i + 1);
        } else {
            result[i] = 0;
        }
        while (i + result[i] < str.length() && str[result[i]] == str[i + result[i]]) {
            ++result[i];
        }
        if (i + result[i] - 1 > right) {
            left = i;
            right = i + result[i] - 1;
        }
    }
    return result;
}

TVector<int> find(const string& pattern, const string& text) {
    string str = pattern + '$' + text;
    TVector<int> result;
    TVector<int> zFunc = z_func(str);
    for (int i = pattern.length(); i < zFunc.get_size(); ++i) {
        if (zFunc[i+1] == pattern.length()) {
            result.push_back(i - pattern.length() );
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string text, pattern;
    getline(cin, text);
    getline(cin, pattern);
    TVector<int> result = find(pattern, text);
    for (int i = 0; i < result.get_size(); i++) {
        cout << result[i] << endl;
    }
    return 0;
}

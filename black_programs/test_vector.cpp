#include <iostream>
#include <string.h>
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
            std::cerr << "Index out of range" << std::endl;
            return;
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

int main() {
    TVector<int> my;
    my.push_back(5);
    my.push_back(4);
    my.push_back(3);
    my.push_back(2);
    my.push_back(1);
    my.insert(0, 0);
    for (int i = 0; i < my.get_size(); i++) {
        cout << my[i] << " ";
    }
    cout << endl;
}

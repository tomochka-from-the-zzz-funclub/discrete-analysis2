#include <iostream>
#include <string>
#include <cstring>
//#include <vector>
#include <array>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int LENGTH_OF_VALUE = 2048;
int NUMBER_OF_DISCHARGE = 20;

typedef struct TVector {
    data_pair* Data;
    int Capacity;
    int CurSize;
} TVector;

void Resize(TVector* vector) {
    vector->Data = realloc(vector->Data, 2 * vector->Capacity * sizeof(data_pair));
    vector->Capacity *= 2;
}

void TVector_init(TVector* vector) {
    vector->Data = calloc(10, sizeof(data_pair));
    vector->Capacity = 10;
    vector->CurSize = 0;
}

void TVector_init_size(TVector* vector, int size) {
    vector->Data = calloc(size, sizeof(data_pair));
    vector->Capacity = size;
    vector->CurSize = size;
}

void TVector_copy(TVector* dest, const TVector* src) {
    dest->Capacity = src->Capacity;
    dest->CurSize = src->CurSize;
    dest->Data = malloc(dest->CurSize * sizeof(data_pair));
    for (int i = 0; i < dest->CurSize; i++) {
        dest->Data[i] = src->Data[i];
    }
}

void TVector_move(TVector* dest, TVector* src) {
    dest->Capacity = src->Capacity;
    dest->CurSize = src->CurSize;
    dest->Data = src->Data;
    src->Capacity = 0;
    src->Data = NULL;
    src->CurSize = 0;
}

void TVector_free(TVector* vector) {
    free(vector->Data);
}

void TVector_push_back(TVector* vector, data_pair elem) {
    if (vector->CurSize == vector->Capacity) {
        Resize(vector);
    }
    vector->Data[vector->CurSize++] = elem;
   // printf ("%s",  vector->Data[vector->CurSize++].key);
}

int TVector_size(TVector* vector) {
    return vector->CurSize;
}

data_pair* TVector_get(TVector* vector, int id) {
    return &(vector->Data[id]);
}

data_pair* TVector_begin(TVector* vector) const {
    return vector->Data;
}

struct data_pair {
    char* key;
    char* value;
    short size_key;
};

struct counting_sort_class {
    static size_t get_id(char elem1, char elem2) {
        return elem1*10 + elem2;
    }
    template <typename Iter>
    static void Counting_sort(Iter begin, Iter end, size_t discharge, std::vector<data_pair>& sorted_vector) {
        std::array<int, 100> array_count;
        for (auto& i: array_count) {
            i = 0;
        }
        for (auto it = begin; it != end; it = std::next(it)) {
            ++array_count[counting_sort_class::get_id(it->key[discharge], it->key[discharge + 1])];
        }
        if (begin != end) {
            for (auto it = std::next(array_count.begin()); it != array_count.end(); it = std::next(it)) {
                *it += *std::prev(it);
            }
            // std::vector<Record> sorted_vector(std::distance(begin, end), {"", ""});
            for (auto it = std::prev(end); ; it = std::prev(it)) {
                size_t idx = --array_count[counting_sort_class::get_id(it->key[discharge], it->key[discharge + 1])];
                sorted_vector[idx] = *it;
                if (it == begin) {
                    break;
                }
            }
            for (auto it_unsorted = begin, it_sorted = sorted_vector.begin(); it_unsorted != end;
                it_unsorted = std::next(it_unsorted), it_sorted = std::next(it_sorted)) {
                *it_unsorted = *it_sorted;
            }
        }
    }
};

struct radix_sort_class {
    template <typename Iter>
    static void Radix_sort(Iter container_begin, Iter container_end , int* max_len) {
        char* new_key = malloc(*max_len * sizeof(char));
        for (short i = 0; i < TVector_size(arr); ++i) {
            for (short j = 0; j < *max_len; ++j) {
                if (j < *max_len - arr->Data[i].size_key) {
                    new_key[j] = '0';
                } else {
                    new_key[j] = arr->Data[i].key[j - *max_len + arr->Data[i].size_key];
                }
            }
            for (short j = 0; j < *max_len; ++j) {
            arr->Data[i].key[j] = new_key[j];
            }
        }
        free (new_key);
        std::vector<data_pair> sorted_vector(std::distance(container_begin, container_end));
        while (NUMBER_OF_DISCHARGE - 2 >= 0) {
            NUMBER_OF_DISCHARGE -= 2;
            counting_sort_class::Counting_sort(container_begin, container_end, NUMBER_OF_DISCHARGE, sorted_vector);
        }

        for (short j = 0; j < TVector_size(arr); ++j) {
        if (*max_len > arr->Data[j].size_key) {
            for (short i = *max_len - arr->Data[j].size_key; i < *max_len; ++i) {
                arr->Data[j].key[i - *max_len + arr->Data[j].size_key] =arr->Data[j].key[i];
                arr->Data[j].key[i] = '0';
            }
            arr->Data[j].key[arr->Data[j].size_key] = '\0';
        }
    }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    TVector data;
    data_pair record;
    int max_len = 0;
    record.key = new char[NUMBER_OF_DISCHARGE + 1];
    while (scanf("%s", record.key) != EOF) {
        record.value = new char[LENGTH_OF_VALUE + 1];
        scanf("%s", record.value);
        record.value = (char *)realloc(record.value, sizeof(char) * (strlen(record.value) + 1));         
        TVector_push_back(data, record);
        max_len++;
        record.key = new char[NUMBER_OF_DISCHARGE + 1];
    }
    radix_sort_class::Radix_sort(data.begin(), data.end(), &max_len);
    for (auto& i: data) {
        std::cout << i.key << "\t" << i.value << "\n";
    }
}
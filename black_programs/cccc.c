#include <stdlib.h>
#include <stdio.h>

typedef struct data_pair {
    char* key;
    char* value;
    short size_key;
} data_pair;

typedef struct TVector {
    data_pair* Data;
    int Capacity;
    int CurSize;
} TVector;

void Resize(TVector* vector) {
    vector->Data = realloc(vector->Data, 1.5 * vector->Capacity * sizeof(data_pair));
    vector->Capacity *= 1.5;
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
}

int TVector_size(TVector* vector) {
    return vector->CurSize;
}

data_pair* TVector_get(TVector* vector, int id) {
    return &(vector->Data[id]);
}

void count_sort(TVector* arr, TVector* sorted, short int* pos) {
    short count[10] = { 0 };
    for (short i = 0; i < TVector_size(arr); ++i) {
        ++count[arr->Data[i].key[*pos] - '0'];
    }
    for (short i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }
    short key;
    for (short i = TVector_size(arr) - 1; i >= 0; --i) {
        key = arr->Data[i].key[*pos] - '0';
        if (key >= 0 && key < 10) {
            (*sorted).Data[count[key] - 1] = arr->Data[i];
            --count[key];
        }
    }
    *arr = *sorted;
}

void radix_sort(TVector* arr, const short* max_len) {
    char* new_key = malloc(*max_len * sizeof(char));
    for (short i = 0; i < TVector_size(arr); ++i) {
        for (short j = 0; j < *max_len; ++j) {
            if (j < *max_len - arr->Data[i].size_key) {
                new_key[j] = '0';
            }
            else {
                new_key[j] = arr->Data[i].key[j - *max_len + arr->Data[i].size_key];
            }
        }
        for (short j = 0; j < *max_len; ++j) {
            arr->Data[i].key[j] = new_key[j];
        }
    }
    free(new_key);
    TVector sorted;
    TVector_init_size(&sorted, TVector_size(arr));
    for (short pos = *max_len - 1; pos >= 0; pos--) {
        count_sort(arr, &sorted, &pos);
    }

    for (short j = 0; j < TVector_size(arr); ++j) {
        if (*max_len > arr->Data[j].size_key) {
            for (short i = *max_len - arr->Data[j].size_key; i < *max_len; ++i) {
                arr->Data[j].key[i - *max_len + arr->Data[j].size_key] = arr->Data[j].key[i];
                arr->Data[j].key[i] = '0';
            }
            arr->Data[j].key[arr->Data[j].size_key] = '\0';
        }
    }
}

int main() {
    TVector arr;
    TVector_init(&arr);

    // Добавляем элементы в массив
    data_pair elem;
    elem.key = "9876543210";
    elem.value = "value1";
    elem.size_key = 10;
    TVector_push_back(&arr, elem);

    elem.key = "0123456789";
    elem.value = "value2";
    elem.size_key = 10;
    TVector_push_back(&arr, elem);

    elem.key = "5432109876";
    elem.value = "value3";
    elem.size_key = 10;
    TVector_push_back(&arr, elem);

    // Выводим элементы массива
    for (int i = 0; i < TVector_size(&arr); i++) {
        printf("Key: %s\n", TVector_get(&arr, i)->key);
        printf("Value: %s\n", TVector_get(&arr, i)->value);
        printf("Size of Key: %d\n", TVector_get(&arr, i)->size_key);
        printf("\n");
    }

    // Сортируем массив
    short max_len = 10;
    radix_sort(&arr, &max_len);

    // Выводим отсортированные элементы массива
    for (int i = 0; i < TVector_size(&arr); i++) {
        printf("Key: %s\n", TVector_get(&arr, i)->key);
        printf("Value: %s\n", TVector_get(&arr, i)->value);
        printf("Size of Key: %d\n", TVector_get(&arr, i)->size_key);
        printf("\n");
    }

    TVector_free(&arr);

    return 0;
}
   
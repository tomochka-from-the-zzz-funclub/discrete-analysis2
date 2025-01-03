#include <stdlib.h>
#include <stdio.h>
#include <string.h>
template<typename T> 
class TVector {
    private:
    T* Data;
    int Capacity;
    int CurSize;
    public:
    TVector(int size);
    void TVector_increase(TVector<T>* vector);
    void TVector_resize(TVector<T>* vector);
    void TVector_init(TVector<T>* vector);
    void TVector_init_size(TVector<T>* vector, int size);
    void TVector_copy(TVector<T>* dest, const TVector<T>* src);
    void TVector_move(TVector<T>* dest, TVector<T>* src);
};
template<typename T>
void TVector_increase(TVector<T>* vector) {
    vector->Data = realloc(vector->Data, 2 * vector->Capacity * sizeof(T));
    vector->Capacity *= 2;
}
template<typename T>
TVector<T>::TVector(int size) {
    this->Data = new word[size];
    this->CurSize = size;
    this->Capacity = template<int T>size;
}
template<typename T>
void TVector_resize(TVector<T>* vector){

}
template<typename T>
void TVector_init(TVector<T>* vector) {
    vector->Data = calloc(10, sizeof(T));
    vector->Capacity = 10;
    vector->CurSize = 0;
}
template<typename T>
void TVector_init_size(TVector<T>* vector, int size) {
    vector->Data = calloc(size, sizeof(T));
    vector->Capacity = size;
    vector->CurSize = size;
}
template<typename T>
void TVector_copy(TVector<T>* dest, const TVector<T>* src) {
    dest->Capacity = src->Capacity;
    dest->CurSize = src->CurSize;
    dest->Data = malloc(dest->CurSize * sizeof(T));
    for (int i = 0; i < dest->CurSize; i++) {
        dest->Data[i] = src->Data[i];
    }
}
template<typename T>
void TVector_move(TVector<T>* dest, TVector<T>* src) {
    dest->Capacity = src->Capacity;
    dest->CurSize = src->CurSize;
    dest->Data = src->Data;
    src->Capacity = 0;
    src->Data = NULL;
    src->CurSize = 0;
}
template<typename T>
void TVector_free(TVector<T>* vector) {
    free(vector->Data);
}
template<typename T>
void TVector_push_back(TVector<T>* vector, T elem) {
    if (vector->CurSize == vector->Capacity) {
        Resize(vector);
    }
    vector->Data[vector->CurSize++] = elem;
}
template<typename T>
int TVector_size(TVector<T>* vector) {
    return vector->CurSize;
}
template<typename T>
T* TVector_get(TVector<T>* vector, int id) {
    return &(vector->Data[id]);
}

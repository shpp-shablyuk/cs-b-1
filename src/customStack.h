#ifndef CUSTOMSTACK_H
#define CUSTOMSTACK_H

#include <iostream>

template<typename ValueType>
class CustomStack {

public:
    CustomStack();

    ~CustomStack();


    int size() const;

    bool empty() const;

    ValueType top() const;

    void push(ValueType value);

    void pop();

    CustomStack(const CustomStack<ValueType> & src);

    CustomStack<ValueType> & operator=(const CustomStack<ValueType> & src);

private:

    static const int INITIAL_CAPACITY = 10;

    ValueType *array;
    int capacity;
    int count;

    void expandCapacity();
    void deepCopy(const CustomStack<ValueType> & src);

};

template<typename ValueType>
CustomStack<ValueType>::CustomStack() {
    capacity = INITIAL_CAPACITY;
    array = new ValueType[capacity];
    count = 0;
}

template<typename ValueType>
CustomStack<ValueType>::CustomStack(const CustomStack<ValueType> & src) {
    deepCopy(src);
}

template<typename ValueType>
CustomStack<ValueType> & CustomStack<ValueType>::operator=(const CustomStack<ValueType> & other) {
    if (this != &other) {
        delete [] array;
        deepCopy(other);
    }
    return *this;
}

template<typename ValueType>
CustomStack<ValueType>::~CustomStack() {
    delete [] array;
}

template<typename ValueType>
int CustomStack<ValueType>::size() const {
    return count;
}

template<typename ValueType>
bool CustomStack<ValueType>::empty() const {
    return count == 0;
}

template<typename ValueType>
void CustomStack<ValueType>::push(ValueType value) {
    if (count == capacity) {
        expandCapacity();
    }
    array[count++] = value;
}

template<typename ValueType>
ValueType CustomStack<ValueType>::top() const {
    if (count == 0) {
        throw "top: stack empty";
    }
    return array[count - 1];
}


template<typename ValueType>
void CustomStack<ValueType>::pop()  {
    if (count == 0) {
        throw "pop: stack empty";
    }
    count--;
}

template<typename ValueType>
void CustomStack<ValueType>::expandCapacity() {
    ValueType * tempArray = array;
    capacity *= 2;
    array = new ValueType[capacity];
    for (int i = 0; i < count; i++) {
        array[i] = tempArray[i];
    }
    delete[] tempArray;
}

template<typename ValueType>
void CustomStack<ValueType>::deepCopy(const CustomStack<ValueType> & src) {
   capacity = src.capacity;
   array = new ValueType[capacity];
   for (int i = 0; i < src.count; i++) {
       array[i] = src.array[i];
   }
   count = src.count;
}



#endif // CUSTOMSTACK_H

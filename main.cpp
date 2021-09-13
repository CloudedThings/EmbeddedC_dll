#include <iostream>
#include <optional>
#include "stdexcept"

using namespace std;

template<typename T>
class Node {
public:
    Node() : data_(T()), next_(nullptr), prev_(nullptr) {}

    explicit Node(T data) : data_(data), next_(nullptr), prev_(nullptr) {};


    Node<T> *getNext() { return next_; }

    void setNext(Node<T> *next) { next_ = next; }

    Node<T> *getPrev() { return prev_; }

    void setPrev(Node<T> *prev) { prev_ = prev; }

    T getData() { return data_; }

    void setData(T data) { data_ = data; }

private:
    T data_;
    Node<T> *next_;
    Node<T> *prev_;
};

template<typename T>
class LinkedList {
public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    LinkedList(const initializer_list<T> &initList) : head_(nullptr), tail_(nullptr), size_(0) {
        for (auto value: initList) {
            pushTail(value);
        }
    }

    LinkedList(const LinkedList<T> &) = delete;

    LinkedList<T> &operator=(const LinkedList<T>) = delete;     // explicit copy of LL not allowed

    ~LinkedList() {
        clearList();
    };

    void pushHead(T data) {
        if (!head_) {
            head_ = new Node<T>(data);
            tail_ = head_;
            size_++;
            return;
        }
        head_->setPrev(new Node<T>(data));
        head_->getPrev()->setNext(head_);
        head_ = head_->getPrev();
        size_++;
    }

    optional<T> popHead() {
        if (!head_) {
            return nullopt;
        }
        if (head_ == tail_) {
            T data = head_->getData();
            delete head_;
            head_ = tail_ = nullptr;
            size_--;
            return data;
        }
        T data = head_->getData();
        head_ = head_->getNext();
        delete head_->getPrev();
        head_->setPrev(nullptr);
        size_--;
        return data;
    }

    void pushTail(T data) {
        if (!tail_) {
            tail_ = new Node<T>(data);
            head_ = tail_;
            size_++;
            return;
        }
        tail_->setNext(new Node<T>(data));
        tail_->getNext()->getPrev();
        tail_ = tail_->getNext();
        size_++;
    }

    optional<T> popTail() {
        if (!tail_) {
            return nullopt;
        }
        if (head_ == tail_) {
            T data = tail_->getData();
            delete tail_;
            tail_ = head_ = nullptr;
            size_--;
            return data;
        }
        T data = tail_->getData();
        tail_ = tail_->getPrev();
        delete tail_->getNext();
        tail_->setNext(nullptr);
        size_--;
        return data;
    }

    void pushAtPos(int index, T data) {
        if (index > size_) {
            throw out_of_range("Index out of range");
        }
        if (index == 0) {
            pushHead(data);
            return;
        }
        if (index == size_) {
            pushTail(data);
            return;
        }
        Node<T> *current = &getAt_(index - 1);
//        for (int i = 1; i < index; i++) {
//            current = current->getNext();
//        }
        Node<T> *newNode = new Node<T>(data);
        newNode->setNext(current->getNext());
        newNode->setPrev(current);
        current->setNext(newNode);
        newNode->getNext()->setPrev(newNode);
        size_++;
    }

    optional<T> popAtPos(int index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        if (index == 0) {
            return popHead();
        }
        if (index == size_ - 1) {
            return popTail();
        }
        Node<T> *current = &getAt_(index);
//        for (int i = 0; i < index ; i++) {
//            current = current->getNext();
//        }
        T data = current->getData();
        current->getPrev()->setNext(current->getNext());
        current->getNext()->setPrev(current->getPrev());
        delete current;
        size_--;
        return data;
    }

    void clearList() {
        Node<T> *current = head_;
        while (head_) {
            current = head_->getNext();
            delete head_;
            head_ = current;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    int size() {
        return size_;
    }

    void print() {
        Node<T> *temp = head_;
        if (head_ == nullptr) {
            cout << "List is empty" << endl;
        } else {
            cout << "Linked List: ";
            while (temp != nullptr) {
                cout << temp->getData() << "->";
                temp = temp->getNext();
            }
            cout << "NULL" << endl;
            cout << "Size: " << size_ << endl;
        }
    }

    T operator[](int index) {
        return getAt_(index).getData();
    }

    Node<T> &getAt_(int index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        if (index == 0) {
            return *head_;
        }
        if (index == size_ - 1) {
            return *tail_;
        }
        if (index <= size_ / 2) {
            Node<T> *current = head_;
            for (int i = 1; i < index; i++) {
                current = current->getNext();
            }
            return *current;
        } else {
            Node<T> *current = tail_;
            for (int i = size_ - 1; i > size_ / 2 + 1; i--) {
                current = current->getPrev();
            }
            return *current;
        }
    }

private:
    Node<T> *head_;
    Node<T> *tail_;
    int size_;
};

int main() {
    LinkedList<int> intList{0, 100, 300};
    intList.pushHead(1);
    intList.pushHead(2);
    intList.pushHead(3);
    intList.pushHead(4);
    intList.pushHead(5);
    intList.pushTail(10);
    intList.pushTail(11);
    intList.pushTail(12);

    intList.print();

    intList.pushAtPos(0, 99);

    intList.print();

    intList.popAtPos(2);

    intList.print();

    cout << intList.getAt_(4).getData() << endl;

    // TODO
    /*
     * iterator
     * intList[9] = 11;
     * intList.changeAt(4, 89); -> example: popAt(4) -> pushAt(4,89)
     *
     */


    return 0;
}

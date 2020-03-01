#ifndef SimpleList_h
#define SimpleList_h

#include <type_traits>
#include <cstddef>
#include <functional>

template<class T>
struct SimpleListNode {
  T                  data;
  SimpleListNode<T>* next = NULL;
};

template<typename T>
class SimpleList {
  public:
    SimpleList();
    SimpleList(int(*compare)(T & a, T & b));
    virtual ~SimpleList();

    virtual int size();
    virtual bool isEmpty();

    virtual void add(int index, T obj);
    virtual void add(T obj);

    virtual void remove(int index);
    virtual void removeFirst();
    virtual void removeLast();

    virtual int count(T obj);

    virtual T pop();
    virtual T get(int index);
    virtual T getFirst();
    virtual T getLast();

    virtual void clear();

  protected:
    int (* compare)(T& a, T& b) = NULL;

    int listSize                 = 0;
    SimpleListNode<T>* listBegin = NULL;
    SimpleListNode<T>* listEnd   = NULL;

    // Helps get() method by saving last position
    SimpleListNode<T>* lastNodeGot = NULL;
    int lastIndexGot               = -1;
    bool isCached                  = false;

    virtual SimpleListNode<T>* getNode(int index);
};

template<typename T>
SimpleList<T>::SimpleList() {}

// Clear Nodes and free Memory
template<typename T>
SimpleList<T>::~SimpleList() { clear(); }

template<typename T>
SimpleListNode<T>* SimpleList<T>::getNode(int index) {
  if ((index < 0) || (index >= listSize)) { return NULL; };

  SimpleListNode<T>* hNode = listBegin;
  int c                    = 0;

  if (isCached && (index >= lastIndexGot)) {
    c     = lastIndexGot;
    hNode = lastNodeGot;
  }

  while (hNode && c < index) {
    hNode = hNode->next;
    c++;
  }

  if (hNode) {
    isCached     = true;
    lastIndexGot = c;
    lastNodeGot  = hNode;
  }

  return hNode;
}

template<typename T>
int SimpleList<T>::size() {
    return listSize;
}

template<typename T>
bool SimpleList<T>::isEmpty() {
    return listSize == 0;
}

template<typename T>
void SimpleList<T>::add(T obj) {
  SimpleListNode<T>* newNode = new SimpleListNode<T>();

  newNode->data = obj;

  if (!listBegin) { listBegin = newNode; }

  if (listEnd) {
    listEnd->next = newNode;
    listEnd       = newNode;
  } else {
    listEnd = newNode;
  }

  listSize++;
}

template<typename T>
void SimpleList<T>::add(int index, T obj) {
    if ((index < 0) || (index >= listSize)) {
        return;
    }

    SimpleListNode<T>* newNode = new SimpleListNode<T>();
    newNode->data = obj;

    if (index == 0) {
        listBegin = newNode;
    } else {
        SimpleListNode<T>* nodePrev = getNode(index - 1);
        newNode->next  = nodePrev->next;
        nodePrev->next = newNode;
    }

    listSize++;
}

template<typename T>
void SimpleList<T>::remove(int index) {
    if ((index < 0) || (index >= listSize)) return;

    SimpleListNode<T>* nodePrev     = getNode(index - 1);
    SimpleListNode<T>* nodeToDelete = getNode(index);

    if (index == 0) {
        listBegin = nodeToDelete->next;
    } else {
        nodePrev->next = nodeToDelete->next;

        if (!nodePrev->next) listEnd = nodePrev;
    }

    delete nodeToDelete;

    isCached = false;

    listSize--;
}

template<typename T>
void SimpleList<T>::removeFirst() { remove(0); }

template<typename T>
void SimpleList<T>::removeLast() { remove(listSize - 1); }

template<typename T>
int SimpleList<T>::count(T obj) {
  if (compare == NULL) return -1;

  int c = 0;

  for (int i = 0; i < listSize; i++) {
    if (compare(obj, getNode(i)->data) == 0) c++;
  }

  return c;
}

template<typename T>
T SimpleList<T>::get(int index) {
  SimpleListNode<T>* h = getNode(index);

  if (h) return h->data;

  return T();
}

template<typename T>
T SimpleList<T>::getFirst() { return get(0); }

template<typename T>
T SimpleList<T>::getLast() { return get(listSize - 1); }

template<typename T>
T SimpleList<T>::pop() {
  T data = getLast();

  removeLast();

  return data;
}

template<typename T>
void SimpleList<T>::clear() {
  while (listSize > 0) removeFirst();

  listSize  = 0;
  listBegin = NULL;
  listEnd   = NULL;

  lastNodeGot  = NULL;
  lastIndexGot = -1;
  isCached     = false;
}

#endif

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

template <typename T> class DataStructure {
public:
  virtual void insert(T value) = 0;
  virtual void remove(T value) = 0;
  virtual bool contains(T k) const = 0;
  virtual bool isEmpty() = 0;
  virtual void show() = 0;
  virtual void clear() = 0;
  virtual ~DataStructure() {};
};

#endif

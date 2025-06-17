#ifndef SET_HPP
#define SET_HPP

template <typename T, typename AbstractSet> class Set {
public:
  virtual AbstractSet operator+(const AbstractSet &otherSet) const = 0;
  virtual AbstractSet operator-(const AbstractSet &otherSet) const = 0;
  virtual AbstractSet operator&(const AbstractSet &otherSet) const = 0;
  virtual bool operator==(const AbstractSet &otherSet) const = 0;
  virtual AbstractSet
  symmetricDifference(const AbstractSet &otherSet) const = 0;
  virtual bool isSubset(const AbstractSet &otherSet) const = 0;
  virtual ~Set() = default;
};

#endif

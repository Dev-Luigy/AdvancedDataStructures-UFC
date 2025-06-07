#ifndef ISET_HPP
#define ISET_HPP

template <typename T, typename Set> class ISet {
public:
  virtual Set operator+(const Set &otherSet) const = 0;
  virtual Set operator-(const Set &otherSet) const = 0;
  virtual Set operator&(const Set &otherSet) const = 0;
  virtual bool operator==(const Set &otherSet) const = 0;
  virtual Set symmetricDifference(const Set &otherSet) const = 0;
  virtual bool isSubset(const Set &otherSet) const = 0;
  virtual ~ISet() = default;
};

#endif

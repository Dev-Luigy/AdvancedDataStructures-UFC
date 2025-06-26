#ifndef HASH_TABLE_HPP

#include "../core/DataStructure.hpp"

template <typename T, typename ColisionType,
          typename HashFunction = std::hash<T>>
class Hash : public DataStructure<T> {
  ~Hash() = default;

public:
  Hash(const Hash &) = delete;
  Hash(Hash &&) = delete;
  Hash &operator=(const Hash &) = delete;
  Hash &operator=(Hash &&) = delete;
};

#endif // !HASH_TABLE_HPP

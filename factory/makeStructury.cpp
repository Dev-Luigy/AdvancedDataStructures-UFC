#include "makeStructury.hpp"

std::unique_ptr<IFreqDS> createStructure(const std::string &type) {
  if (type == "avl")
    return std::make_unique<AVLWrapper>();
  if (type == "rb")
    return std::make_unique<RBWrapper>();
  if (type == "open")
    return std::make_unique<OpenHashWrapper>();
  if (type == "extern")
    return std::make_unique<ExternHashWrapper>();
  return nullptr;
}

#ifndef MEEPO_GRAPH_PARAMETER_H
#define MEEPO_GRAPH_PARAMETER_H

#include <any>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Meepo {

class Parameter {
 public:
  Parameter() = default;

  Parameter(const Parameter& other) = default;

  Parameter(Parameter&& other) noexcept : params_(std::move(other.params_)) {}

  auto operator=(const Parameter& other) -> Parameter& {
    if (this != &other) {
      params_ = other.params_;
    }
    return *this;
  }

  auto operator=(Parameter&& other) noexcept -> Parameter& {
    if (this != &other) {
      params_ = std::move(other.params_);
    }
    return *this;
  }

  using iterator = std::unordered_map<std::string, std::any>::iterator;
  using const_iterator =
      std::unordered_map<std::string, std::any>::const_iterator;

  auto begin() -> iterator { return params_.begin(); }
  auto begin() const -> const_iterator { return params_.begin(); }
  auto end() -> iterator { return params_.end(); }
  auto end() const -> const_iterator { return params_.end(); }

  void set(const std::string& key, const std::any& value) {
    params_[key] = value;
  }

  template <typename T>
  auto get(const std::string& key) const -> T {
    if (params_.find(key) == params_.end()) {
      throw std::runtime_error("Parameter key not found");
    }

    return std::any_cast<T>(params_.at(key));
  }

  auto empty() const -> bool { return params_.empty(); }

 private:
  std::unordered_map<std::string, std::any> params_;
};

}  // namespace Meepo

#endif  // MEEPO_GRAPH_PARAMETER_H
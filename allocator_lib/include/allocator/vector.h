#ifndef VECTOR_H
#define VECTOR_H

#include <memory>

template< typename T,
          typename Allocator = std::allocator< T > >
class vector
{
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using iterator = T*;
  using const_iterator = T const *;

  explicit vector(allocator_type const & alloc = allocator_type());
 ~vector();

  iterator begin() { return start_; }
  iterator end() { return start_ + count_; }
  const_iterator begin() const { return start_; }
  const_iterator end() const { return start_ + count_; }

  inline void push_back(T const & value);

 private:
  inline void relocate();

  allocator_type alloc_;
  T * start_;
  size_t count_;
  size_t total_;
};

template< typename T,
          typename Allocator >
vector< T, Allocator >::vector(allocator_type const & alloc)
    : alloc_(alloc)
    , start_(nullptr)
    , count_(0)
    , total_(0)
{}

template< typename T,
          typename Allocator >
vector< T, Allocator >::~vector()
{
  for (size_t i = 0; i < count_; ++i) {
    alloc_.destroy(&start_[i]);
  }
  alloc_.deallocate(start_, total_);
}

template< typename T,
          typename Allocator >
void vector< T, Allocator >::push_back(T const & value)
{
  if (!total_) {
    start_ = alloc_.allocate(1, nullptr);
    ++total_;
  }

  if (total_ == count_) {
    relocate();
  }

  alloc_.construct(start_ + (count_++), value);
}

template< typename T,
          typename Allocator >
void vector< T, Allocator >::relocate()
{
  auto new_total = total_ + 1; // if *2. then throw bad_alloc, since custom allocator the page size is 10
  auto new_start = alloc_.allocate(new_total, nullptr);
  for (size_t i = 0; i < count_; ++i) {
    alloc_.construct(&new_start[i], start_[i]);
    alloc_.destroy(&start_[i]);
  }
  alloc_.deallocate(start_, total_);
  total_ = new_total;
  start_ = new_start;
}

#endif /* VECTOR_H */

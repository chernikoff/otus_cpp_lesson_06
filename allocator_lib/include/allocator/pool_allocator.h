#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <array>
#include <list>
#include <algorithm>
#include <cassert>

template< typename T, size_t PageSize = 1024 >
class pool_allocator
{
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = T const *;
  using reference = T &;
  using const_reference = T const &;

  template< typename U >
  struct rebind
  {
    using other = pool_allocator< U, PageSize >;
  };

  inline pool_allocator() noexcept {}
  inline pool_allocator(pool_allocator const & other) noexcept
      : pages_(other.pages_)
  {}
  inline pool_allocator(pool_allocator && other) noexcept
  {
    std::swap(pages_, other.pages_);
  }

  inline ~pool_allocator() noexcept{};

  inline pool_allocator & operator=(pool_allocator const & other) noexcept
  {
    if (this == &other) return *this;
    pages_ = other.pages_;
    return *this;
  }
  inline pool_allocator & operator=(pool_allocator && other) noexcept
  {
    std::swap(pages_, other.pages_);
    return *this;
  }

  inline T* allocate(size_t n, void const * hint = nullptr);
  inline void deallocate(T* p, size_t n) noexcept;

  template< typename U, typename ... Args >
  inline void construct(U * p, Args && ... args);

  template< typename U >
  inline void destroy(U * p);

 private:
  struct page
  {
    uint8_t mem[PageSize * sizeof(T)];
    size_t allocated = 0;
    size_t deallocated = 0;
  };

  std::list< page > pages_;
};

template< typename T, size_t PageSize >
T* pool_allocator< T, PageSize >::allocate(size_t n, void const *)
{
  if (n > PageSize) throw std::bad_alloc();
  auto it = std::find_if(std::begin(pages_), std::end(pages_),
                         [&n](auto const & page) {
                           if ((PageSize - page.allocated) >= n) return true;
                           return false;
                         });

  page * alloc_page = nullptr;
  if (it == std::end(pages_)) {
    pages_.emplace_back();
    alloc_page = &pages_.back();
  }
  else {
    alloc_page = &*it;
  }

  auto ptr = alloc_page->mem + alloc_page->allocated * sizeof(T);
  alloc_page->allocated += n;
  return reinterpret_cast< T* >(ptr);
}

template< typename T, size_t PageSize >
void pool_allocator< T, PageSize >::deallocate(T *p, size_t n) noexcept
{
  auto ptr = reinterpret_cast< uintptr_t >(p);
  for(auto it = std::begin(pages_), end = std::end(pages_);
      it != end; ++it) {
    auto start_addr = reinterpret_cast< uintptr_t >(it->mem);
    auto end_addr = reinterpret_cast< uintptr_t >(it->mem + PageSize * sizeof(T));
    if (start_addr <= ptr && ptr <= end_addr) {
      it->deallocated += n;
      if (it->deallocated == PageSize) {
        pages_.erase(it);
      }
      return;
    }
  }
  assert(false);
}

template< typename T, size_t PageSize >
template< typename U, typename ... Args >
void pool_allocator< T, PageSize >::construct(U *p, Args && ... args)
{
  new(p) U(std::forward< Args >(args) ...);
}

template< typename T, size_t PageSize >
template< typename U >
void pool_allocator< T, PageSize >::destroy(U *p)
{
  p->~U();
}

#endif /* POOL_ALLOCATOR_H */

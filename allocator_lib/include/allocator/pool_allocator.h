#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

template< typename T, size_t PageSize = 1024 >
class pool_allocator
{
 public:
  using value_type = T;

  template< typename U >
  struct rebind
  {
    using other = pool_allocator< U, PageSize >;
  };

  inline pool_allocator() noexcept;
  inline pool_allocator(pool_allocator const & other) noexcept;
  inline pool_allocator(pool_allocator && other) noexcept;

  inline ~pool_allocator() noexcept;

  inline T* allocate(size_t n, void const * hint = nullptr);
  inline void deallocate(T* p, size_t n) noexcept;

  template< typename U, typename ... Args >
  inline void construct(U * p, Args && ... args);

  template< typename U >
  inline void destroy(U * p);
};

#endif /* POOL_ALLOCATOR_H */

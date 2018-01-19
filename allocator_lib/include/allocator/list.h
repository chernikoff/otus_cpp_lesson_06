#ifndef LIST_H
#define LIST_H

#include <memory>
#include <iterator>

template< typename T, typename Allocator = std::allocator< T > >
class list
{
  struct node
  {
    T value;
    node * next;
  };

 public:
  using allocator_type = Allocator;
  using value_type = T;

  class iterator
      : public std::iterator< std::forward_iterator_tag, value_type >
  {
   public:
    explicit iterator(node * n)
        : current(n)
    {}

    iterator(iterator const & other)
        : current(other.current)
    {}

    iterator & operator=(iterator const & other)
    {
      current = other.current;
      return *this;
    }

    iterator & operator++()
    {
      current = current->next;
      return *this;
    }

    iterator operator++(int)
    {
      iterator copy(*this);
      ++(*this);
      return copy;
    }

    value_type & operator*()
    {
      return current->value;
    }

    value_type * operator->()
    {
      return &current.value;
    }

    bool operator==(iterator const & other) const
    {
      return current == other.current;
    }

    bool operator!=(iterator const & other) const
    {
      return current != other.current;
    }

   private:
    node * current;
  };

  list()
      : head(nullptr)
  {}

  void push_back(value_type const & value)
  {
    auto mem = alloc.allocate(1, nullptr);
    alloc.construct(mem);
    mem->value = value;
    mem->next = nullptr;

    auto item = last();
    if (item) {
      item->next = mem;
      return;
    }

    head = mem;
  }

  auto begin() { return iterator(first()); }
  auto end() { return iterator(nullptr); }

 private:
  auto first() { return head; }
  auto last()
  {
    auto it = head;

    while (head && it->next) {
      it = it->next;
    }

    return it;
  }

  node * head;
  typename allocator_type::template rebind< node >::other alloc;
};


#endif /* LIST_H */

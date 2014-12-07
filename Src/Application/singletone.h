#include <memory>

template<class T>
std::shared_ptr<T> create_singleton()
{
   static std::weak_ptr<T> ptr ;
   if (! ptr.expired())
      return ptr.lock() ;

   std::shared_ptr<T> t ( new T ) ;
   ptr = t ;
   return t ;
}

template<typename T, typename A0>
std::shared_ptr<T> create_singleton( A0 const &a0 )
{
  static std::weak_ptr<T> ptr ;
  if (! ptr.expired())
    return ptr.lock() ;

  std::shared_ptr<T> t ( new T(a0) ) ;
  ptr = t ;
  return t ;
}

template<typename T, typename A0, typename A1>
std::shared_ptr<T> create_singleton( A0 const &a0, A1 const &a1 )
{
  static std::weak_ptr<T> ptr ;
  if (! ptr.expired())
    return ptr.lock() ;

  std::shared_ptr<T> t ( new T(a0, a1) ) ;
  ptr = t ;
  return t ;
}
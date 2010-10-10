// **************************************************************************
// *                                                                        *
// * Custom allocators example, Copyright (C) 2010 Onwards Chris Pilkington *
// * All rights reserved.  Web: http://chris.iluo.net                       *
// *                                                                        *
// * This software is free software; you can redistribute it and/or         *
// * modify it under the terms of the GNU General Public License as         *
// * published by the Free Software Foundation; either version 2.1 of the   *
// * License, or (at your option) any later version. The text of the GNU    *
// * General Public License is included with this library in the            *
// * file license.txt.                                                      *
// *                                                                        *
// * This library is distributed in the hope that it will be useful,        *
// * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
// * See the file GPL.txt for more details.                                 *
// *                                                                        *
// **************************************************************************

#include <cassert>
#include <cmath>
#include <cstring>

#include <limits>
#include <string>
#include <iostream>
#include <sstream>

#include <algorithm>
#include <vector>
#include <list>


#ifndef nullptr
#ifdef BUILD_DEBUG
// ** Nullptr
// Const class
const class nullptr_t
{
public:
  template <class T>
  operator T*() const { return NULL; }

  template <class C, class T>
  operator T C::*() const { return NULL; }

private:
  void operator&() const; // Forbidden
} nullptr = {};
#else
#define nullptr NULL
#endif // !BUILD_DEBUG
#endif // !nullptr

typedef std::wstring string_t;
typedef std::wostringstream ostringstream_t;



// http://en.wikipedia.org/wiki/Allocator_%28C%2B%2B%29

namespace allocator
{
  // cDefaultAllocator
  // All this allocator does is call new and delete, this is the same behaviour as the default STL allocator.

  template <class T>
  class cDefaultAllocator
  {
  public:
    // Type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    // Rebind allocator to type U
    template <class U>
    struct rebind
    {
      typedef cDefaultAllocator<U> other;
    };

    // Return address of values
    pointer address(reference value) const
    {
      return &value;
    }
    const_pointer address(const_reference value) const
    {
      return &value;
    }

    // Constructors and destructor
    cDefaultAllocator() throw()
    {
    }
    cDefaultAllocator(const cDefaultAllocator&) throw()
    {
    }
    template <class U>
    cDefaultAllocator(const cDefaultAllocator<U>&) throw()
    {
    }
    ~cDefaultAllocator()throw()
    {
    }

    // Return maximum number of elements that can be allocated
    size_type max_size() const throw()
    {
      return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // Allocate but don't initialize num elements of type T
    pointer allocate(size_type num, const void* = nullptr)
    {
      // Print message and allocate memory with global new
      std::cerr<<"allocate "<<num<<" element(s)"<<" of size "<<sizeof(T);
      pointer ret = (pointer)(::operator new(num * sizeof(T)));
      std::cerr<<" at: "<<(void*)ret<<std::endl;
      return ret;
    }

    // Deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num)
    {
      // Print message and deallocate memory with global delete
      std::cerr<<"deallocate "<<num<<" element(s)"<<" of size "<<sizeof(T)<<" at: "<<(void*)p<<std::endl;
      ::operator delete((void*)p);
    }

    // Initialize elements of allocated storage p with value value
    void construct(pointer p, const T& value)
    {
      // Initialize memory with placement new
      new((void*)p)T(value);
    }

    // Destroy elements of initialized storage p
    void destroy(pointer p)
    {
      // Call the destructor of this object
      p->~T();
    }
  };

  // Return that all specializations of this allocator are interchangeable
  template <class T1, class T2>
  bool operator==(const cDefaultAllocator<T1>&, const cDefaultAllocator<T2>&) throw()
  {
    return true;
  }

  template <class T1, class T2>
  bool operator!=(const cDefaultAllocator<T1>&, const cDefaultAllocator<T2>&) throw()
  {
    return false;
  }



  // cCountedAllocator
  // This is the same as the default allocator except that it counts the allocations/deallocations and constructions/destructions

  template <class T>
  class cCountedAllocator
  {
  public:
    // Type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    // Rebind allocator to type U
    template <class U>
    struct rebind
    {
      typedef cCountedAllocator<U> other;
    };

    // Return address of values
    pointer address(reference value) const
    {
      return &value;
    }
    const_pointer address(const_reference value) const
    {
      return &value;
    }

    // Constructors and destructor
    cCountedAllocator() throw() :
      nAllocated(0),
      nConstructed(0)
    {
    }
    cCountedAllocator(const cCountedAllocator& rhs) throw() :
      nAllocated(rhs.nAllocated),
      nConstructed(rhs.nConstructed)
    {
    }
    template <class U>
    cCountedAllocator(const cCountedAllocator<U>& rhs) throw() :
      nAllocated(rhs.nAllocated),
      nConstructed(rhs.nConstructed)
    {
    }
    ~cCountedAllocator()throw()
    {
      assert(nAllocated == 0);
      assert(nConstructed == 0);
    }

    // Return maximum number of elements that can be allocated
    size_type max_size() const throw()
    {
      return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // Allocate but don't initialize num elements of type T
    pointer allocate(size_type num, const void* = nullptr)
    {
      // Print message and allocate memory with global new
      std::cerr<<"allocate "<<num<<" element(s)"<<" of size "<<sizeof(T);
      pointer ret = (pointer)(::operator new(num * sizeof(T)));
      std::cerr<<" at: "<<(void*)ret<<std::endl;
      nAllocated += num;
      return ret;
    }

    // Deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num)
    {
      // Print message and deallocate memory with global delete
      std::cerr<<"deallocate "<<num<<" element(s)"<<" of size "<<sizeof(T)<<" at: "<<(void*)p<<std::endl;
      ::operator delete((void*)p);
      nAllocated -= num;
    }

    // Initialize elements of allocated storage p with value value
    void construct(pointer p, const T& value)
    {
      // Initialize memory with placement new
      new((void*)p)T(value);
      nConstructed++;
    }

    // Destroy elements of initialized storage p
    void destroy(pointer p)
    {
      // Call the destructor of this object
      p->~T();
      assert(nConstructed != 0);
      nConstructed--;
    }
    
  private:
    size_t nAllocated;
    size_t nConstructed;
  };

  // Return that all specializations of this allocator are interchangeable
  template <class T1, class T2>
  bool operator==(const cCountedAllocator<T1>&, const cCountedAllocator<T2>&) throw()
  {
    return true;
  }

  template <class T1, class T2>
  bool operator!=(const cCountedAllocator<T1>&, const cCountedAllocator<T2>&) throw()
  {
    return false;
  }
}

class cPerson
{
public:
  cPerson();
  ~cPerson();

  char szFirstName[1024];
  char szLastName[1024];
  int iAge;
};

cPerson::cPerson() :
  iAge(0)
{
  std::cout<<"cPerson Created"<<std::endl;
  szFirstName[0] = 0;
  szLastName[0] = 0;
}

cPerson::~cPerson()
{
  std::cout<<"cPerson Destroyed"<<std::endl;
}

template <typename T>
void TestContainer(T& t)
{
  t.push_back(cPerson());
  t.push_back(cPerson());
  t.push_back(cPerson());
}

bool RunAllocatorTest()
{
  // Normal memory allocation
  std::cout<<"Normal list memory allocation"<<std::endl;
  {
    std::list<cPerson> people;
    TestContainer(people);
  }
  std::cout<<std::endl;
  std::cout<<"Normal vector memory allocation"<<std::endl;
  {
    std::vector<cPerson> people;
    TestContainer(people);
  }
  std::cout<<std::endl;

  // Default allocator memory allocation
  std::cout<<"Default allocator list memory allocation"<<std::endl;
  {
    std::list<cPerson, allocator::cDefaultAllocator<cPerson> > people;
    TestContainer(people);
  }
  std::cout<<std::endl;
  std::cout<<"Default allocator vector memory allocation"<<std::endl;
  {
    std::vector<cPerson, allocator::cDefaultAllocator<cPerson> > people;
    TestContainer(people);
  }
  std::cout<<std::endl;

  // Counted allocator memory allocation
  std::cout<<"Counted allocator list memory allocation"<<std::endl;
  {
    std::list<cPerson, allocator::cCountedAllocator<cPerson> > people;
    TestContainer(people);
  }
  std::cout<<std::endl;
  std::cout<<"Counted allocator vector memory allocation"<<std::endl;
  {
    std::vector<cPerson, allocator::cCountedAllocator<cPerson> > people;
    TestContainer(people);
  }
  std::cout<<std::endl;

  return true;
}

int main(int argc, char** argv)
{
  return RunAllocatorTest() ? EXIT_FAILURE : EXIT_SUCCESS;
}


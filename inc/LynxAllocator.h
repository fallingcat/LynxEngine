#ifndef __LYNXALLOCATOR_H__
#define __LYNXALLOCATOR_H__

namespace LynxEngine 
{
	#ifdef __USE_STL__
		template <class T>
		class TAllocator
		{
		public:
			typedef size_t    size_type;
			typedef ptrdiff_t difference_type;
			typedef T*        pointer;
			typedef const T*  const_pointer;
			typedef T&        reference;
			typedef const T&  const_reference;
			typedef T         value_type;

			TAllocator() {}
			TAllocator(const TAllocator&) {}

			pointer allocate(size_type n, const void * = 0) 
			{
				T* t = (T*) malloc(n * sizeof(T));		
				return t;
			}
  
			void  deallocate(void* p, size_type) 
			{
				if (p) 
				{
					free(p);			
				} 
			}

			pointer           address(reference x) const { return &x; }
			const_pointer     address(const_reference x) const { return &x; }
			TAllocator<T>&  operator=(const TAllocator&) { return *this; }
			void              construct(pointer p, const T& val) 
								{ new ((T*) p) T(val); }
			void              destroy(pointer p) { p->~T(); }

			size_type         max_size() const { return size_t(-1); }

			template <class U>
			struct rebind { typedef TAllocator<U> other; };

			template <class U>
			TAllocator(const TAllocator<U>&) {}

			template <class U>
			TAllocator& operator=(const TAllocator<U>&) { return *this; }
		};
	#endif
}

#endif
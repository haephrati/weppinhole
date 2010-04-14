#pragma once

template
<
	class K,
	class C = std::less<K>,
	class A = std::allocator< K >
>
class VectorSet 
	: private std::vector< K, A >
	, public C
{
	typedef std::vector<K, A> Base;
	typedef C MyCompare;

public:
	typedef K key_type;
	typedef C key_compare;
	typedef A allocator_type;
	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename A::pointer pointer;
	typedef typename A::const_pointer const_pointer;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

	class value_compare
		: public std::binary_function<value_type, value_type, bool>
		, private key_compare
	{
		friend class VectorSet;

	protected:
		value_compare(key_compare pred) : key_compare(pred)
		{}

	public:
		bool operator()(const value_type& lhs, const value_type& rhs) const
		{ return key_compare::operator()(lhs.first, rhs.first); }
	};

	explicit VectorSet(const key_compare& comp = key_compare(), const A& alloc = A())
		: Base(alloc)
		, MyCompare(comp)
	{}

	template <class InputIterator>
	VectorSet(InputIterator first, InputIterator last, 
		const key_compare& comp = key_compare(), 
		const A& alloc = A())
		: Base(first, last, alloc), MyCompare(comp)
	{
		MyCompare& me = *this;
		std::sort(begin(), end(), me);
	}

	VectorSet& operator=(const VectorSet& rhs)
	{ 
		VectorSet(rhs).swap(*this); 
		return *this;
	}

	// iterators:
	// The following are here because MWCW gets 'using' wrong
	iterator begin() { return Base::begin(); }
	const_iterator begin() const { return Base::begin(); }
	iterator end() { return Base::end(); }
	const_iterator end() const { return Base::end(); }
	reverse_iterator rbegin() { return Base::rbegin(); }
	const_reverse_iterator rbegin() const { return Base::rbegin(); }
	reverse_iterator rend() { return Base::rend(); }
	const_reverse_iterator rend() const { return Base::rend(); }

	// capacity:
	bool empty() const { return Base::empty(); }
	size_type size() const { return Base::size(); }
	size_type max_size() { return Base::max_size(); }

	// modifiers:
	std::pair<iterator, bool> insert(const value_type& val)
	{
		bool found(true);
		iterator i(lower_bound(val));

		if (i == end() || this->operator()( val, *i ))
		{
			i = Base::insert(i, val);
			found = false;
		}
		return std::make_pair(i, !found);
	}

	//Section [23.1.2], Table 69
	//http://developer.apple.com/documentation/DeveloperTools/gcc-3.3/libstdc++/23_containers/howto.html#4
	iterator insert(iterator pos, const value_type& val)
	{
		if( (pos == begin() || this->operator()(*(pos-1),val)) && 
			(pos == end()    || this->operator()(val, *pos)) )
		{
			return Base::insert(pos, val);
		}
		return insert(val);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{ for (; first != last; ++first) insert(*first); }

	void erase(iterator pos)
	{ Base::erase(pos); }

	size_type erase(const key_type& k)
	{
		iterator i(find(k));
		if (i == end()) return 0;
		erase(i);
		return 1;
	}

	void erase(iterator first, iterator last)
	{ Base::erase(first, last); }

	void swap(VectorSet& other)
	{
		Base::swap(other);
		MyCompare& me = *this;
		MyCompare& rhs = other;
		std::swap(me, rhs);
	}

	void clear()
	{ Base::clear(); }

	// observers:
	key_compare key_comp() const
	{ return *this; }

	value_compare value_comp() const
	{
		const key_compare& comp = *this;
		return value_compare(comp);
	}

	// 23.3.1.3 map operations:
	iterator find(const key_type& k)
	{
		iterator i(lower_bound(k));
		if (i != end() && this->operator()(k, i))
		{
			i = end();
		}
		return i;
	}

	const_iterator find(const key_type& k) const
	{       
		const_iterator i(lower_bound(k));
		if (i != end() && this->operator()(k, i))
		{
			i = end();
		}
		return i;
	}

	size_type count(const key_type& k) const
	{ return find(k) != end(); }

	iterator lower_bound(const key_type& k)
	{
		MyCompare& me = *this;
		return std::lower_bound(begin(), end(), k, me);
	}

	const_iterator lower_bound(const key_type& k) const
	{
		const MyCompare& me = *this;
		return std::lower_bound(begin(), end(), k, me);
	}

	iterator upper_bound(const key_type& k)
	{
		MyCompare& me = *this;
		return std::upper_bound(begin(), end(), k, me);
	}

	const_iterator upper_bound(const key_type& k) const
	{
		const MyCompare& me = *this;
		return std::upper_bound(begin(), end(), k, me);
	}

	std::pair<iterator, iterator> equal_range(const key_type& k)
	{
		MyCompare& me = *this;
		return std::equal_range(begin(), end(), k, me);
	}

	std::pair<const_iterator, const_iterator> equal_range(
		const key_type& k) const
	{
		const MyCompare& me = *this;
		return std::equal_range(begin(), end(), k, me);
	}

	template <class K1, class C1, class A1>
	friend bool operator==(const VectorSet<K1, C1, A1>& lhs,
		const VectorSet<K1, C1, A1>& rhs);

	bool operator<(const VectorSet& rhs) const
	{
		const Base& me = *this;
		const Base& yo = rhs;
		return me < yo;
	}

	template <class K1, class C1, class A1>
	friend bool operator!=(const VectorSet<K1, C1, A1>& lhs,
		const VectorSet<K1, C1, A1>& rhs);

	template <class K1, class C1, class A1>
	friend bool operator>(const VectorSet<K1, C1, A1>& lhs,
		const VectorSet<K1, C1, A1>& rhs);

	template <class K1, class C1, class A1>
	friend bool operator>=(const VectorSet<K1, C1, A1>& lhs,
		const VectorSet<K1, C1, A1>& rhs);

	template <class K1, class C1, class A1>
	friend bool operator<=(const VectorSet<K1, C1, A1>& lhs,
		const VectorSet<K1, C1, A1>& rhs);
};

template <class K, class C, class A>
inline bool operator==(const VectorSet<K, C, A>& lhs,
					   const VectorSet<K, C, A>& rhs)
{
	const std::vector< K, A>& me = lhs;
	return me == rhs;
}

template <class K, class C, class A>
inline bool operator!=(const VectorSet<K, C, A>& lhs,
					   const VectorSet<K, C, A>& rhs)
{ return !(lhs == rhs); }

template <class K, class C, class A>
inline bool operator>(const VectorSet<K, C, A>& lhs,
					  const VectorSet<K, C, A>& rhs)
{ return rhs < lhs; }

template <class K, class C, class A>
inline bool operator>=(const VectorSet<K, C, A>& lhs,
					   const VectorSet<K, C, A>& rhs)
{ return !(lhs < rhs); }

template <class K, class C, class A>
inline bool operator<=(const VectorSet<K, C, A>& lhs,
					   const VectorSet<K, C, A>& rhs)
{ return !(rhs < lhs); }


// specialized algorithms:
template <class K, class C, class A>
void swap(VectorSet<K, C, A>& lhs, VectorSet<K, C, A>& rhs)
{ lhs.swap(rhs); }


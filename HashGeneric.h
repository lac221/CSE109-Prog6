/*
CSE 109: Fall 2018
Lydia Cornwell
lac221
Makes hash map
Program #6
*/
#ifndef HASH_GENERIC_H
#define HASH_GENERIC_H

#include<vector>
#include<cstring>
#include<string>
#include<iostream>
#include<sstream>
#include<utility>

using namespace std;

template<typename T, std::pair<const void*, size_t>(*D)(const T&), int (*H)(const void*, size_t)>
class Hash_t
{

	template<typename T1, std::pair<const void*, size_t>(*D1)(const T1&), int (*H1)(const void*, size_t)>
	friend ostream& operator<<(ostream&, const Hash_t<T1,D1,H1>&);

public:
	Hash_t();
	Hash_t(size_t buckets);
	Hash_t(size_t buckets, int(*hashfunc)(const void*, size_t));
	Hash_t(Hash_t &src);
	Hash_t& operator=(const Hash_t<T,D,H>&);
	bool insert(T);
	Hash_t& operator+=(T);
	bool find(T);
	bool remove(T);
	Hash_t& operator-=(T);
	string to_string() const;
	size_t size() const;
	bool resize(size_t);

	/*
	class iterator {
		friend class Hash_t;

	public:
		iterator();
		iterator(std::vector<std::vector<T> >*, size_t, size_t);
		iterator(const iterator&);
		iterator& operator=(const iterator&);
		bool operator!=(const iterator&);
		bool operator==(const iterator&);
		iterator operator+(size_t amt);
		iterator& operator+=(size_t amt);
		iterator operator-(size_t amt);
		iterator& operator-=(size_t amt);
		iterator& operator++();
		iterator& operator--();
		//postfix
		iterator operator++(int);
		iterator operator--(int);

		const T& operator*() const;

	private:
		std::vector<std::vector<T> >* bucket_ptr;
		size_t bucket_idx;
		size_t data_idx;
	};

	iterator begin();
	iterator end();
	iterator remove(iterator& it);
	*/

private:
	vector<vector<T> > buckets;
	size_t _size;

	unsigned int getIndex(const T&) const;

};


#include "HashGeneric.cpp"

#endif

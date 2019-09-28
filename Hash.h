/*
CSE 109: Fall 2018
Lydia Cornwell
lac221
Makes hash map
Program #6
*/
#ifndef HASH_H
#define HASH_H

#include<vector>
#include<cstring>
#include<string>
#include<iostream>
#include<sstream>

using namespace std;

class Hash_t
{
public:
	Hash_t();
	Hash_t(size_t buckets);
	Hash_t(size_t buckets, int(*hashfunc)(const void*, size_t));
	Hash_t(Hash_t &src);
	Hash_t& operator=(const Hash_t&);
	bool insert(unsigned int);
	Hash_t& operator+=(unsigned int);
	bool find(unsigned int);
	bool remove(unsigned int);
	Hash_t& operator-=(unsigned int);
	string to_string() const;
	size_t size() const;
	bool resize(size_t);
	friend ostream& operator<<(ostream&, const Hash_t&);

	class iterator 
	{
		friend class Hash_t;

	public:
		iterator();
		iterator(std::vector<std::vector<unsigned int> >*, size_t, size_t);
		iterator(const iterator&);
		iterator& operator=(const iterator&);
		bool operator!=(const iterator&) const;
		bool operator==(const iterator&) const;
		iterator operator+(size_t amt) const;
		iterator& operator+=(size_t amt);
		iterator operator-(size_t amt) const;
		iterator& operator-=(size_t amt);
		iterator& operator++();
		iterator& operator--();
		//postfix
		iterator operator++(int);
		iterator operator--(int);

		const unsigned int& operator*() const;

	private:
		std::vector<std::vector<unsigned int> >* bucket_ptr;
		size_t bucket_idx;
		size_t data_idx;
	};

	iterator begin();
	iterator end();
	iterator remove(iterator& it);

private:
	vector<vector<unsigned int>> buckets;
	int(*function)(const void*, size_t);
	size_t _size;

	unsigned int getIndex(const unsigned int&) const;

};

#endif


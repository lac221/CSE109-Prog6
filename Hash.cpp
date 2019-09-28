

/*
CSE 109: Fall 2018
Lydia Cornwell
lac221
Makes hash map
Program #6
*/
#ifndef HASH_CPP
#define HASH_CPP

// #pragma once

#include"Hash.h"
#include"sha1.h"

Hash_t::Hash_t()
	:buckets(vector<vector<unsigned int>>(10)), function(sha1_hash), _size(0)
{
}

Hash_t::Hash_t(size_t buckets)
	:buckets(vector<vector<unsigned int>>(buckets)), function(sha1_hash), _size(0)
{
}

Hash_t::Hash_t(size_t buckets, int(*hashfunc)(const void*, size_t))
	:buckets(vector<vector<unsigned int>>(buckets)), function(hashfunc), _size(0)
{
}

Hash_t::Hash_t(Hash_t &src)
{
	this->buckets = vector<vector<unsigned int>>(src.buckets);
	this->function = src.function;
	this->_size = src._size;
}

Hash_t& Hash_t::operator=(const Hash_t& rhs)
{
	if(this == &rhs)
	{
		return *this;
	}

	buckets.clear();
	this->buckets = rhs.buckets;
	this->function = rhs.function;
	this->_size = rhs._size;

	return *this;
}

bool Hash_t::insert(unsigned int toInsert)
{
	unsigned int index = getIndex(toInsert);
	if(find(toInsert) == true)
	{
		return false;
	}

	vector<unsigned int>::iterator it = buckets[index].begin();
	for(size_t i = 0; i < buckets[index].size(); i++)
	{
		//if(toInsert == buckets[index][i])
		//{
		//	return false;
		//}
		if(toInsert < buckets[index][i])
		{
			buckets[index].insert(it + i, toInsert);
			_size++;
			return true;
		}
	}
	buckets[index].push_back(toInsert);
	_size++;
	return true;
}

Hash_t& Hash_t::operator+=(unsigned int toInsert)
{
	insert(toInsert);
	return *this;
}

bool Hash_t::find(unsigned int element)
{
	unsigned int index = getIndex(element);

	for(size_t i = 0; i < buckets[index].size(); i++)
	{
		if(element == buckets[index][i])
		{
			return true;
		}
	}
	return false;
}

bool Hash_t::remove(unsigned int element)
{
	unsigned int index = getIndex(element);
	vector<unsigned int>::iterator it = buckets[index].begin();

	for(size_t i = 0; i < buckets[index].size(); i++)
	{
		if(element == buckets[index][i])
		{
			buckets[index].erase(it + i);
			_size--;
			return true;
		}
	}
	return false;
}

Hash_t& Hash_t::operator-=(unsigned int element)
{
	remove(element);
	return *this;
}

string Hash_t::to_string() const
{
	stringstream retval;
	int width = 0;

	int temp = buckets.size();
	while(temp > 0) {
		width++;
		temp /= 10;
	}

	for(size_t i = 0; i < buckets.size(); i++)
	{
		retval.width(width);
		retval << i;
		retval << ":";

		for(size_t j= 0; j < buckets[i].size(); j++)
		{
			retval << " " << buckets[i][j];
		}
		if(buckets[i].size() == 0)
		{
			retval << " (Empty)";
		}
		retval << endl;
	}

	return retval.str();
}

size_t Hash_t::size() const
{
	return _size;
}

bool Hash_t::resize(size_t newBuckets)
{
	if(newBuckets == 0)
	{
		return false;
	}

	vector<unsigned int> elements;
	for(vector<unsigned int>& bucket : buckets) {
		elements.insert(elements.end(), bucket.begin(), bucket.end());
	}

	buckets.clear();
	buckets.resize(newBuckets);

	for(unsigned int element : elements)
	{
		insert(element);
	}

	return true;
}

ostream& operator<<(ostream& out, const Hash_t& output)
{
	out << output.to_string();
	return out;
}

unsigned int Hash_t::getIndex(const unsigned int& element) const
{
	const void* data = &element;
	size_t size = sizeof(unsigned int);
	size_t index = function(data, size);

	index %= buckets.size();
	return index;
}

Hash_t::iterator Hash_t::begin()
{
	if(_size == 0) {
		return Hash_t::iterator(&buckets, (size_t)0, (size_t)0);
	}

	int bucket_idx = -1;
	for(size_t i = 0; i < buckets.size(); i++) 
	{
		if(!buckets[i].empty()) 
		{
			bucket_idx = i;
			break;
		}
	}
	if(bucket_idx == -1) {
		return Hash_t::iterator(&buckets, (size_t)0, (size_t)0);
	}

	int data_idx = 0;

	return Hash_t::iterator(&buckets, (size_t)bucket_idx, (size_t)data_idx);
}

Hash_t::iterator Hash_t::end()
{
	if(_size == 0) 
	{
		return iterator(&buckets, 0, 0);
	}

	int bucket_idx = -1;
	for(int i = buckets.size() - 1; i >= 0; i--) 
	{
		if(!buckets[i].empty()) 
		{
			bucket_idx = i;
			break;
		}
	}
	if(bucket_idx == -1) {
		return iterator(&buckets, 0, 0);
	}

	int data_idx = buckets[bucket_idx].size();

	return iterator(&buckets, bucket_idx, data_idx);
}

Hash_t::iterator Hash_t::remove(Hash_t::iterator& it)
{
	unsigned int toRemove = *it;
	fprintf(stderr, "removing %u\n", toRemove);
	if(it.data_idx == buckets[it.bucket_idx].size() - 1)
	{
		it++;
	}

	remove(toRemove);
	fprintf(stderr, "iterator is now %u\n", *it);
	return it;
}

Hash_t::iterator::iterator() : bucket_ptr(NULL), bucket_idx(0), data_idx(0) { }

Hash_t::iterator::iterator(vector<std::vector<unsigned int>>* bucket_, size_t bucket_i, size_t data_i) : bucket_ptr(bucket_), bucket_idx(bucket_i), data_idx(data_i) { }

Hash_t::iterator::iterator(const iterator& src) : bucket_ptr(src.bucket_ptr), bucket_idx(src.bucket_idx), data_idx(src.data_idx) { }


Hash_t::iterator& Hash_t::iterator::operator=(const Hash_t::iterator& src) 
{
	this->bucket_ptr = src.bucket_ptr;
	this->bucket_idx = src.bucket_idx;
	this->data_idx = src.bucket_idx;

	return *this;
}

bool Hash_t::iterator::operator!=(const Hash_t::iterator& to_compare) const
{
	if(this->bucket_ptr != to_compare.bucket_ptr) {
		return true;
	}
	if(this->bucket_idx != to_compare.bucket_idx) {
		return true;
	}
	if(this->data_idx != to_compare.data_idx) {
		return true;
	}
	return false;

	// return !((*this) == to_compare);
}

bool Hash_t::iterator::operator==(const Hash_t::iterator& to_compare) const
{
	if(this->bucket_ptr != to_compare.bucket_ptr) {
		return false;
	}
	if(this->bucket_idx != to_compare.bucket_idx) {
		return false;
	}
	if(this->data_idx != to_compare.data_idx) {
		return false;
	}
	return true;
}

Hash_t::iterator Hash_t::iterator::operator+(size_t amt) const
{
	Hash_t::iterator copy(*this);
	for(size_t i = 0; i < amt; i++)
	{
		copy++;	
	}
	return copy;
}

Hash_t::iterator& Hash_t::iterator::operator+=(size_t amt) 
{
	for(size_t i = 0; i < amt; i++)
	{
		this->operator++();
	}
	return *this;
}

Hash_t::iterator Hash_t::iterator::operator-(size_t amt) const
{
	Hash_t::iterator copy(*this);
	for(size_t i = 0; i < amt; i++)
	{
		copy--;	
	}
	return copy;

}

Hash_t::iterator& Hash_t::iterator::operator-=(size_t amt) 
{
	for(size_t i = 0; i < amt; i++)
	{
		this->operator--();	
	}
	return *this;
}

Hash_t::iterator& Hash_t::iterator::operator++() // ++iter
{
	if(this->data_idx < (*(this->bucket_ptr))[this->bucket_idx].size() - 1) {
		this->data_idx++;
		return *this;
	}

	if(this->bucket_idx == (*(this->bucket_ptr)).size() - 1) {
		this->data_idx++;
		return *this;
	}

	size_t new_bucket_idx = this->bucket_idx + 1;
	while( ( new_bucket_idx < (*(this->bucket_ptr)).size() ) && (*(this->bucket_ptr))[new_bucket_idx].empty() ) {
		new_bucket_idx++;
	}

	if(new_bucket_idx == (*(this->bucket_ptr)).size()) {
		this->data_idx++;
		return *this;
	}
	else {
		this->bucket_idx = new_bucket_idx;
		this->data_idx = 0;
		return *this;
	}
}

Hash_t::iterator& Hash_t::iterator::operator--() 
{
	if(this->data_idx > 0) {
		this->data_idx--;
		return *this;
	}

	if(this->bucket_idx == 0) {
		// check what to do in "undefined behavior" case
		return *this;
	}

	ssize_t new_bucket_idx = this->bucket_idx - 1;
	while( ( new_bucket_idx >= 0 ) && (*(this->bucket_ptr))[new_bucket_idx].empty() ) {
		new_bucket_idx--;
	}

	if(new_bucket_idx == -1) {
		// check what to do in "undefined behavior" case
		// this->data_idx--;
		return *this;
	}
	else {
		this->bucket_idx = new_bucket_idx;
		this->data_idx = (*(this->bucket_ptr))[new_bucket_idx].size() - 1;
		return *this;
	}
}

//postfix
Hash_t::iterator Hash_t::iterator::operator++(int)
{
	Hash_t::iterator temp(*this);
	this->operator++();
	return temp;
}

//postfix
Hash_t::iterator Hash_t::iterator::operator--(int)
{
	Hash_t::iterator temp(*this);
	this->operator--();
	return temp;
}

const unsigned int& Hash_t::iterator::operator*() const 
{
	return (*bucket_ptr)[bucket_idx][data_idx];
}

#endif

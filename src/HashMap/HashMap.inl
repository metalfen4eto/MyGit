#pragma once

#include "HashMap.hpp"

template<typename K, typename T, typename HashFunction>
HashMap<K, T, HashFunction>::HashMap() : table(32), count(0), hashFunction(HashFunction()) {}

template<typename K, typename T, typename HashFunction>
void HashMap<K, T, HashFunction>::clear()
{
    table = HashTable(32);
    count = 0;
}

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction> HashMap<K, T, HashFunction>::begin()
{
    return Iterator(table);
}

template<typename K, typename T, typename HashFunction>
HashMapIterator<K, T, HashFunction> HashMap<K, T, HashFunction>::end()
{
    return Iterator(table, table.end(), table.back().end());
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::ConstIterator HashMap<K, T, HashFunction>::cbegin() const
{
    return ConstIterator (table);
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::ConstIterator HashMap<K, T, HashFunction>::cend() const
{
    return ConstIterator(table, table.cend(), table.back().cend());
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::Iterator HashMap<K, T, HashFunction>::find(const K& key)
{
    BucketIterator bucket = getBucket(key);
    for (ElementIterator elIter = bucket->begin(); elIter != bucket->end(); ++elIter)
    {
        if (elIter->first == key)
        {
            return Iterator(table, bucket, elIter);
        }
    }
    return end();
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::ConstIterator HashMap<K, T, HashFunction>::find(const K& key) const
{
    BucketConstIterator bucket = getBucket(key);
    for (ElementConstIterator elIter = bucket->cbegin(); elIter != bucket->cend(); ++elIter)
    {
        if (elIter->first == key)
        {
            return ConstIterator(table, bucket, elIter);
        }
    }
    return cend();
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::Iterator HashMap<K, T, HashFunction>::insert(const K& key, const T& value)
{
    if (shouldResize())
    {
        resize();
    }

    BucketIterator bucket = getBucket(key);
    for (ElementIterator elIter = bucket->begin(); elIter != bucket->end(); ++elIter)
    {
        if (elIter->first == key)
        {
            elIter->second = value;
            return Iterator(table, bucket, elIter);
        }
    }

    ++count;
    ElementIterator element = bucket->insert_after(bucket->beforeBegin(), std::make_pair(key, value));
    return Iterator(table, bucket, element);
}

template<typename K, typename T, typename HashFunction>
T& HashMap<K, T, HashFunction>::operator[](const K& key)
{
    Iterator it = find(key);
    if (it == end())
    {
        return insert(key, T())->second;
    }
    else
    {
        return it->second;
    }
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::Iterator HashMap<K, T, HashFunction>::erase(HashMap::Iterator it)
{
    if (it == end())
    {
        return it;
    }

    --count;
    it.element = it.bucket->remove(it.element);
    it.findNextValid();
    return it;
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::Iterator HashMap<K, T, HashFunction>::erase(const K& key)
{
    return erase(find(key));
}

template<typename K, typename T, typename HashFunction>
int HashMap<K, T, HashFunction>::size() const
{
    return count;
}

template<typename K, typename T, typename HashFunction>
bool HashMap<K, T, HashFunction>::empty() const
{
    return count == 0;
}

template<typename K, typename T, typename HashFunction>
int HashMap<K, T, HashFunction>::index(const K& key) const
{
    return hashFunction(key) % table.size();
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::BucketIterator HashMap<K, T, HashFunction>::getBucket(const K& key)
{
    return table.begin() + index(key);
}

template<typename K, typename T, typename HashFunction>
bool HashMap<K, T, HashFunction>::shouldResize()
{
    const float load = float(count) / table.size();
    return load > 0.7;
}

template<typename K, typename T, typename HashFunction>
void HashMap<K, T, HashFunction>::resize()
{
    HashTable newTable(table.size() * 2 + 1);
    table.swap(newTable);

    for (Bucket & bucket : newTable) {
        for (Pair & el : bucket) {
            getBucket(el.first)->push_front(std::make_pair(el.first, el.second));
        }
        bucket.clear();
    }
}

template<typename K, typename T, typename HashFunction>
HashMap<K, T, HashFunction>::HashMap(const HashMap<K, T, HashFunction>& other)
{
    copy(other);
}

template<typename K, typename T, typename HashFunction>
HashMap<K, T, HashFunction>::HashMap(HashMap<K, T, HashFunction>&& other) : table(std::move(other.table)), count(other.count), hashFunction(other.hashFunction) {}

template<typename K, typename T, typename HashFunction>
HashMap<K, T, HashFunction>& HashMap<K, T, HashFunction>::operator=(const HashMap<K, T, HashFunction>& other)
{
    if (this != &other)
    {
        copy(other);
    }
    return *this;
}

template<typename K, typename T, typename HashFunction>
typename HashMap<K, T, HashFunction>::BucketConstIterator HashMap<K, T, HashFunction>::getBucket(const K& key) const
{
    return table.cbegin() + index(key);
}

template<typename K, typename T, typename HashFunction>
void HashMap<K, T, HashFunction>::copy(const HashMap<K, T, HashFunction>& other)
{
    hashFunction = other.hashFunction;
    table = HashTable(other.table.capacity());
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
        insert(it->first, it->second);
    }
}

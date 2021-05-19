#ifndef DL_HASHTABLE_HPP
#define DL_HASHTABLE_HPP

using namespace std;
using namespace cop4530;


// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}


//HashTable(size_t size = 101): constructor. Create a hash table, where the size of the vector is set to prime_below(size) (where size is default  to 101), 
//where prime_below() is a private member function of the HashTable and provided to you.
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size){
  hashSize = 0;
  if(size > 0 && size < max_prime + 1){
    max = size;
  }
  else {
    max = default_capacity;
  }
  
  hashtable.resize(prime_below(max));
}


//~HashTable(): destructor. Delete all elements in hash table.
template <typename K,typename V>
HashTable<K,V>::~HashTable(){
  makeEmpty();
}

//bool contains(const K & k) const: check if key k is in the hash table.
template <typename K,typename V>
bool HashTable<K,V>::contains(const K & k) const{
  auto x = myhash(k);
    if (hashtable[x].empty()){
      return false;
    }
    for (auto itr = hashtable[x].begin(); itr != hashtable[x].end(); itr++){
      if (k == itr->first){
        return true;
      }
    }
    return false;
}

//bool match(const std::pair<K, V> &kv) const: check if key-value pair is in the hash table.
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) const{
  auto x = myhash(kv.first);
  if(hashtable[x].empty()){
    return false;
  }
  auto itr1 = hashtable[x].begin();
  while (itr1 != hashtable[x].end()){
    if (kv == *itr1){
      return true;
    }
    ++itr1;
  }
  return false;
}

//bool insert(const std::pair<K, V> & kv): add  the key-value pair kv into the hash table. Don't add if kv is already in the hash table. If the key is the hash table but 
//with a different value, the value should be updated to the new one with kv. Return true if kv is inserted or the value is updated; return false otherwise 
//(i.e., if kv is in the hash table).
template <typename K,typename V>
bool HashTable<K,V>::insert(const std::pair<K, V> & kv){
  if(match(kv)){
    return false;
  }
  if (hashSize == hashtable.size()){
    rehash();
  }
  auto x = myhash(kv.first);
  if(hashtable[x].empty() == false){
    auto itr = hashtable[x].begin();
    while(itr != hashtable[x].end()){
      if(kv.first ==itr->first){
        itr->second = kv.second;
        hashSize++;
        return true;
      }
      itr++;
      }
    hashtable[x].push_back(kv);
    hashSize++;
    return true;
  }
  else{
    hashtable[x].push_back(kv);
    hashSize++;
    return true;
  }
}

//bool insert (std::pair<K,  V> && kv): move version of insert.
template <typename K,typename V>
bool HashTable<K,V>::insert (std::pair<K,  V> && kv){
  if(match(kv)){
    return false;
  }
  if (hashSize == hashtable.size()){
    rehash();
  }
  auto x = myhash(kv.first);
  if(hashtable[x].empty() == false){
    auto itr = hashtable[x].begin();
    while(itr != hashtable[x].end()){
      if(kv.first ==itr->first){
        itr->second = kv.second;
        hashSize++;
        return true;
      }
      itr++;
      }
    hashtable[x].push_back(kv);
    hashSize++;
    return true;
  }
  else{
    hashtable[x].push_back(kv);
    hashSize++;
    return true;
  }
}

//bool remove(const K & k): delete the key k and the corresponding value if it is in the hash table. Return true if k is deleted, return false otherwise 
//(i.e., if key k is not in the hash table).
template <typename K,typename V>
bool HashTable<K,V>::remove(const K & k){
  if(contains(k)){
    auto x = myhash(k);
    auto itr = hashtable[x].begin();
    while (itr != hashtable[x].end()){
      if (itr->first == k){
        hashtable[x].erase(itr);
        --hashSize;
        return true;
      }
      itr++;
    }
    return true;
  }
  else{
    return false;
  }
}

//void clear(): delete all elements in the hash table
template <typename K,typename V>
void HashTable<K,V>::clear(){
  hashSize = 0;
  makeEmpty();
}

//bool load(const char *filename): load the content of the file with name filename into the hash table. In the file, each line contains a single pair of key and value, 
//separated by a white space.
template <typename K,typename V>
bool HashTable<K,V>::load(const char *filename){
  ifstream infile;
  infile.open(filename);
  if (infile.is_open()){
    pair <K, V> p;
    string buffer;
    K k;
    V v;
    while (infile >> buffer){
      istringstream sk(buffer);
      infile >> buffer;
      istringstream sv(buffer);
      sk >> k;
      sv >> v;
      p.first = k;
      p.second = v;
      if(contains(p.first)){
        continue;
      }
      else{
        insert(p);
      }
    }
  }
  else{
    return false;
  }

  infile.close();
  return true;

}

//void dump() const: display all entries in the hash table. If an entry contains multiple key-value pairs, separate them by a semicolon character (:) 
//(see the provided executable for the exact output format).
template <typename K,typename V>
void HashTable<K,V>::dump() const{
  for (int i = 0; i < hashtable.size(); i++){
    if (hashtable[i].empty()){
    cout << "v[" << i << "]: " << endl;
    }
    else{
      cout << "v[" << i << "]: ";
      for (auto itr1 = hashtable[i].begin(); itr1 != hashtable[i].end(); itr1++){
        if (itr1 != hashtable[i].begin()){
          cout << " : ";
        }
        cout << itr1->first << ' ' << itr1->second;
        }
        cout << endl;
      }
    }
}

//size_t size() const: return the number of elements in the hash table.
template <typename K,typename V>
size_t HashTable<K,V>::size() const{
  return hashSize;
}

//bool write_to_file(const char *filename) const: write all elements in the hash table into a file with name filename. Similar to the file format in the load function, 
//each line contains a pair of key-value pair, separated by a white space.
template <typename K,typename V>
bool HashTable<K,V>::write_to_file(const char *filename) const{
  ofstream ofile;
  ofile.open(filename);
    if(ofile.is_open()){
        for (int i = 0; i < hashtable.size(); i++){
          if (hashtable[i].empty()){
            continue;
          }
          auto itr = hashtable[i].begin();
          while (itr != hashtable[i].end()){
            ofile << itr->first << ' ' << itr->second << '\n';
            itr++;
          }
        }
        return true;
    }
    else{
        return false;
    }
}










//void makeEmpty(): delete all elements in the hash table. The public interface clear() will call this function
template <typename K, typename V>
void HashTable<K, V>::makeEmpty(){
  for (int i = 0; i < hashtable.size(); i++){
      if (hashtable[i].empty()){
        continue;
      }
      else{
        hashtable[i].clear();
      }
    }
}

//void rehash(): Rehash function. Called when the number of elements in the hash table is greater than the size of the vector.
template <typename K, typename V>
void HashTable<K, V>::rehash(){
  hashtable.resize(prime_below(max * 2));
}

//size_t myhash(const K &k) const: return the index of the vector entry where k should be stored.
template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const{
  static hash<K> hf;
	return hf(k) % hashtable.size();

}







#endif
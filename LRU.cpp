/* Resizable LRU cache implementation in C++ */

struct LRUCacheEntry {
	string key;
	string value;
	LRUCacheEntry* prev;
	LRUCacheEntry* next;
};

class LRUCache {
	private : 
		map<string, LRUCacheEntry *>  _mapping;
		int sz;
		int cnt;
		LRUCacheEntry *               head;
		LRUCacheEntry *               tail;
		map<string, LRUCacheEntry *>::iterator it;
	
	public :
		LRUCache() {
			cnt = 0;
			sz = 0;
			head = new LRUCacheEntry;
			tail = new LRUCacheEntry;
			head->prev = NULL;
			head->next = tail;
			tail->next = NULL;
			tail->prev = head;			
		}

		inline void resize(int siz) {
			if(sz <= siz) { sz = siz; return; }

			int k = cnt - siz;
			while(k > 0) {
				detach_from_tail();
				cnt--;
				k--;
			}
			sz = siz;
		}

		~LRUCache() {
			delete head;
			delete tail;
		}

		inline void get(string key, string &value) {
			struct LRUCacheEntry* node = _mapping[key];
			if(node) {
				detach(node);
				attach(node);
				value = node->value;		
			}
			else {
				value = "NULL";
			}
		}
		
		inline void peek(string key, string &value) {
			struct LRUCacheEntry* node = _mapping[key];
			if(node) {
				value = node->value;		
			}
			else {
				value = "NULL";
			}
		}

		inline void put(string key, string value) {
			struct LRUCacheEntry* node = _mapping[key];
			if(node) {
				// already present..refresh the linked list
				detach(node);
				node->value = value;
				attach(node);
			}
			else {
				if(cnt == sz) {
					// the buffer is full..remove one node and push one other node.
					node = tail->prev;
					detach(node);
					_mapping.erase(node->key);
					node->value = value;
					node->key = key;
					_mapping[key] = node;
					attach(node);
				}
				else {
					node = new LRUCacheEntry;
					node->key = key;
					node->value = value;
					_mapping[key] = node;
					attach(node);
					cnt++;
				}				
			}
		}

		inline void dump() {
			int i=0;
			for(it = _mapping.begin(); it != _mapping.end(); it++) {
				struct LRUCacheEntry* node = it->second;
				if(node) {
					printf("%s %s\n", node->key.c_str(), node->value.c_str());
				}
			}
		}



	private:
		inline void detach(LRUCacheEntry* node) {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}

		inline void attach(LRUCacheEntry* node) {
			node->next = head->next;
			node->prev = head;
			head->next = node;
			node->next->prev = node;
		}

		inline void detach_from_tail() {
			LRUCacheEntry* node;
			node = tail->prev;
			detach(node);
			_mapping.erase(node->key);
		}		

};


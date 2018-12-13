#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <stack>
namespace aisdi {

template <typename KeyType, typename ValueType>
class HashMap {
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Pair = std::pair<const key_type, mapped_type>;
    using Vector = std::vector<Pair>;
    using hash_value = int (KeyType);

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;


private:
    std::hash <key_type> makeHash;

    Vector *tab;

    int MAX_SIZE;
    int SIZE;

public:
    HashMap() {
        SIZE = 0;
        MAX_SIZE = 999999;
        tab = new Vector[MAX_SIZE];
    }

    ~HashMap() {
        delete[] tab;
        SIZE = 0;
        MAX_SIZE = 0;
    }

    HashMap(std::initializer_list<value_type> list):HashMap() {
        for(auto it = list.begin(); it != list.end(); it++) {
            this->operator[](it->first) = it->second;
        }
    }

    HashMap(const HashMap& other):HashMap() {
        for(auto it = other.begin(); it != other.end(); it++) {
            this->operator[](it->first) = it->second;
        }
    }

    HashMap(HashMap&& other):HashMap() {
        *this = std::move(other);
    }

    HashMap& operator=(const HashMap& other) {
        if(other == *this) {
            return *this;
        }

        SIZE = 0;
        delete[] tab;
        tab = new Vector[MAX_SIZE];

        for(auto it = other.begin(); it != other.end(); it++) {
            this->operator[](it->first) = it->second;
        }

        return *this;
    }

    HashMap& operator=(HashMap&& other) {
        SIZE = 0;
        delete[] tab;
        tab = new Vector[MAX_SIZE];

        std::swap(SIZE, other.SIZE);
        std::swap(tab, other.tab);
        std::swap(MAX_SIZE, other.MAX_SIZE);

        return *this;
    }

    bool isEmpty() const {
        if(SIZE == 0) {
            return true;
        } else {
            return false;
        }
    }

    mapped_type& operator[](const key_type& key) {
        int HASH;

        HASH = makeHash(key)%MAX_SIZE;

        int i = 0;
        for(auto it = tab[HASH].begin(); it != tab[HASH].end(); it++) {
            if(tab[HASH][i].first == key) {
                mapped_type test;
                if(mapped_type{} != test) {
                    tab[HASH][i].second = mapped_type{};
                }

                return tab[HASH][i].second;
            }
            i++;
        }
        tab[HASH].push_back(value_type(key, mapped_type{}));
        SIZE++;

        return tab[HASH].back().second;
    }

    mapped_type& getValue(const key_type& key) const {
        int HASH = makeHash(key)%MAX_SIZE;

        if(!tab[HASH].size()) {
            throw std::out_of_range("");
        }

        int i = 0;
        for(auto it = tab[HASH].begin(); it != tab[HASH].end(); it++) {
            if(tab[HASH][i].first == key) {
                return tab[HASH][i].second;
            }

            i++;
        }

        throw std::out_of_range("");
    }

    const mapped_type& valueOf(const key_type& key) const {
        return getValue(key);
    }

    mapped_type& valueOf(const key_type& key) {
        return getValue(key);
    }

    const_iterator find(const key_type& key) const {
        int HASH = makeHash(key)%MAX_SIZE;

        if(SIZE == 0 || tab[HASH].size() == 0) return end();

        int i = 0;
        for(auto it = tab[HASH].begin(); it != tab[HASH].end(); it++) {

            if(tab[HASH][i].first == key) {
                break;
            }
            i++;
        }

        ConstIterator it;
        it.tab= tab;
        it.HASH = HASH;
        it.pos = i;

        return it;
    }

    iterator find(const key_type& key) {
        int HASH = makeHash(key)%MAX_SIZE;

        if(SIZE == 0 || tab[HASH].size() == 0) return end();

        int i = 0;
        for(auto it = tab[HASH].begin(); it != tab[HASH].end(); it++) {

            if(tab[HASH][i].first == key) {
                break;
            }
            i++;
        }

        Iterator it;
        it.tab= tab;
        it.HASH = HASH;
        it.pos = i;

        return it;
    }

    void Erase(int i, int h) {
        std::stack<Pair> s;
        int x = tab[h].size()-1;

        while(i < x) {
            s.push(tab[h].back());
            tab[h].pop_back();
            x--;
        }
        tab[h].pop_back();
        while(s.size()) {
            tab[h].push_back(s.top());
            s.pop();
        }
    }

    void remove(const key_type& key) {
        int HASH = makeHash(key)%MAX_SIZE;

        if(tab[HASH].size() == 0 || SIZE == 0) {
            throw std::out_of_range("");
        }


        int i = 0;
        for(auto it = tab[HASH].begin(); it != tab[HASH].end(); it++) {
            if(tab[HASH][i].first == key) {
                SIZE--;
                Erase(i, HASH);
                break;
            }
            i++;
        }
    }

    void remove(const const_iterator& it) {
        if(tab[it.HASH].size() == 0 || it.pos >= (int)tab[it.HASH].size() || SIZE == 0) {
            throw std::out_of_range("");
        }
        SIZE--;
        Erase(it.pos, it.HASH);
    }

    size_type getSize() const {
        return SIZE;
    }

    bool operator==(const HashMap& other) const {
        if(SIZE != other.SIZE) {
            return false;
        }

        auto a = begin();
        auto b = other.begin();

        while(a != end()) {
            if(a.HASH != b.HASH || a.pos != b.pos || a.tab[a.HASH][a.pos].first != b.tab[b.HASH][b.pos].first || a.tab[a.HASH][a.pos].second != b.tab[b.HASH][b.pos].second) return false;
            a++;
            b++;
        }

        return true;
    }

    bool operator!=(const HashMap& other) const {
        return !(*this == other);
    }

    int findFirstHASH() const {
        if(SIZE == 0) return 0;
        int i = 0;
        while(i < MAX_SIZE && tab[i].size() == 0 ) {
            i++;
        }
        return i;
    }

    int findLastHASH() const {
        if(SIZE == 0) return 0;
        int i = MAX_SIZE - 1;
        while(i >= 0 && tab[i].size() == 0) {
            i--;
        }

        i++;
        return i;
    }
    iterator begin() {
        Iterator it;
        it.tab = tab;
        it.HASH = findFirstHASH();
        it.pos = 0;

        return it;
    }

    iterator end() {
        Iterator it;
        it.tab = tab;
        it.HASH = findLastHASH();
        it.pos = 0;

        return it;
    }

    const_iterator cbegin() const {
        ConstIterator it;
        it.tab = tab;
        it.HASH = findFirstHASH();
        it.pos = 0;

        return it;
    }

    const_iterator cend() const {
        ConstIterator it;
        it.tab = tab;
        it.HASH = findLastHASH();
        it.pos = 0;

        return it;
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename HashMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using pointer = const typename HashMap::value_type*;
    using Pair = std::pair<const key_type, mapped_type>;
    using Vector = std::vector<Pair>;
    int MAX_SIZE = 999999;
    Vector *tab;
    int HASH;
    int pos;


    explicit ConstIterator() {
        tab = nullptr;
        HASH = 0;
        pos = 0;
    }

    ConstIterator(const ConstIterator& other) {
        tab = other.tab;
        HASH = other.HASH;
        pos = other.pos;
    }

    ConstIterator& operator++() {
        int HASH_b = HASH;

        if(tab[HASH].size() == 0) {
            throw std::out_of_range("");
        }

        if((int)tab[HASH].size() - 1 > pos) {
            pos++;
            return *this;
        } else {
            HASH++;
            pos = 0;
            while(HASH < MAX_SIZE -1 && tab[HASH].size() == 0) {
                HASH++;
            }

            if(HASH == MAX_SIZE -1) HASH = HASH_b + 1;

            return *this;
        }
    }

    ConstIterator operator++(int) {
        int HASH_b = HASH;

        if(tab[HASH].size() == 0) {
            throw std::out_of_range("");
        }

        ConstIterator tmp;
        tmp.tab = tab;
        tmp.HASH = HASH;
        tmp.pos = pos;

        if((int)tab[HASH].size() - 1 > pos) {
            pos++;
        } else {
            HASH++;
            pos = 0;
            while(HASH < MAX_SIZE -1 && tab[HASH].size() == 0) {
                HASH++;
            }

            if(HASH == MAX_SIZE -1) HASH = HASH_b + 1;
        }

        return tmp;
    }

    ConstIterator& operator--() {
        if(tab[HASH].size() == 0 && HASH == 0) {
            throw std::out_of_range("");
        }

        if(pos > 0) {
            pos--;
            return *this;
        } else {
            HASH--;

            while(HASH >= 0 && tab[HASH].size() == 0) {
                HASH--;
            }

            if(HASH == -1) {
                throw std::out_of_range("");
            }

            pos = tab[HASH].size()-1;

            return *this;
        }
    }

    ConstIterator operator--(int) {
        if(tab[HASH].size() == 0 && HASH == 0) {
            throw std::out_of_range("");
        }

        ConstIterator tmp;
        tmp.tab = tab;
        tmp.HASH = HASH;
        tmp.pos = pos;

        if(pos > 0) {
            pos--;
        } else {
            HASH--;

            while(HASH >= 0 && tab[HASH].size() == 0) {
                HASH--;
            }

            if(HASH == -1) {
                throw std::out_of_range("");
            }

            pos = tab[HASH].size()-1;

        }

        return tmp;
    }

    reference operator*() const {
        if(tab[HASH].size() == 0) {
            throw std::out_of_range("");
        }

        return tab[HASH][pos];
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const {
        if(HASH == other.HASH && pos == other.pos && tab == other.tab) return true;
        else return false;
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::value_type*;

    explicit Iterator() {
    }

    Iterator(const ConstIterator& other)
        : ConstIterator(other) {
    }

    Iterator& operator++() {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int) {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--() {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int) {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    reference operator*() const {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */

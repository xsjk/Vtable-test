#include <iostream>
#include <list>

typedef void(*F)(void);

class vtable {
    public:
        #define m_table (*(F**)m_class)

        vtable() = delete;
        vtable(const void *c) = delete;
        vtable(void *c) : vtable(c,0) { while (m_table[m_len]) ++m_len;}
        vtable(void *c, size_t len) :m_class(c), m_len(len) {}
        ~vtable() { for (auto &o : m_overwritten) delete[] o; }
        F operator[](size_t i) const { return m_table[i]; }
        size_t length() const { return m_len; }

        // overwrite the vtable of the given object with this vtable
        void overwrite(void* other){
            F* newtable = new F[m_len];
            memcpy(newtable, m_table, sizeof(F) * m_len);
            m_overwritten.push_back(newtable);
            *(F**)other = newtable;
        }
        void overwrite(vtable other){
            overwrite(other.m_class);
        }
         
    private:
        void *m_class;
        size_t m_len;
        std::list<F*> m_overwritten;


    friend std::ostream& operator<<(std::ostream& os, const vtable& vt) {
        os << '[';
        for (size_t i = 0; i < vt.m_len; ++i) {
            if (i==vt.m_len-1) os << (void*)vt[i];
            else os << (void*)vt[i] << ' ';
        }
        return os << ']';
    }
};

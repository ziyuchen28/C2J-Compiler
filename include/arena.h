#include <cstddef>
#include <cstdint>
#include <stdlib.h>


struct ArenaPage
{
    size_t cap;
    size_t offset;
    ArenaPage *next;
    alignas(std::max_align_t) unsigned char data[1];
};


class Arena
{
public:
    explicit Arena(size_t pagesize = 16 * 1024, size_t oversize_threshold = 8 * 1024)
        : head_(nullptr), current_(nullptr), 
          total_capacity_(0),
          pagesize_(pagesize), 
          oversize_threshold_(oversize_threshold) {}

    ~Arena() { release_all(); }

    void *allocate(size_t n, size_t align = alignof(std::max_align_t)) {
        // need a new page
        if (!current_ || !fits(n, align, current_)) {
            // overflow
            // const bool over_threshold = n + align > oversize_threshold_; 
            const bool over = n > (oversize_threshold_ > align ? oversize_threshold_ - align : 0);
            size_t size = over ? align_up(n, alignof(std::max_align_t)) : pagesize_;
            ArenaPage *new_page = allocate_page(size);
            if (!current_) head_ = current_ = new_page;
            else { current_->next = new_page; current_ = new_page; }
        }
        size_t off_aligned = align_up(current_->offset, align);
        void *available = current_->data + off_aligned;
        current_->offset = off_aligned + n;
        return available;
    }
    // template<class T, class...A>
    // T* make(A&&...a) {
    //
    // }
    void release_all() {
        ArenaPage *page = head_;
        while (page) {
            ArenaPage *next = page->next;           
            ::operator delete(page);
            page = next;
        }
        head_ = current_ = nullptr;
        total_capacity_ = 0;
    }
    struct Stats 
    {
        size_t pages, total_capacity, used_bytes;
    };
    Stats stats() const {
        ArenaPage *page = head_;
        size_t pages = 0, total_capacity = 0, used_bytes = 0;
        while (page) { 
            pages++; 
            total_capacity += page->cap; 
            used_bytes += page->offset; 
            page = page->next; 
        }
        return {pages, total_capacity, used_bytes};
    }
private:
    ArenaPage *head_;
    ArenaPage *current_;
    size_t pagesize_;
    size_t oversize_threshold_;
    size_t total_capacity_;
    static size_t align_up(size_t size, size_t align) {
        return (size + align - 1) & ~(align - 1);         
    }
    static bool fits(size_t n, size_t align, ArenaPage *current) {
        size_t off_aligned = align_up(current->offset, align);
        size_t total = off_aligned + n;
        return total <= current->cap;
    }
    ArenaPage *allocate_page(size_t cap) {
        size_t page_size = cap + sizeof(ArenaPage) - 1;
        unsigned char *raw = static_cast<unsigned char*>(::operator new(page_size));
        ArenaPage *page = reinterpret_cast<ArenaPage*>(raw);
        page->cap = cap;
        page->offset = 0;
        page->next = nullptr;
        total_capacity_ += cap;
        return page;
        
    }
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

};


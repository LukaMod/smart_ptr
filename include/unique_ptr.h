#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H


#include <utility>
#include <memory>

namespace xy {

template <typename T>
class unique_ptr {
public:
    // member types
    using element_type = T;
    using pointer = T*;

    // constructor
    constexpr unique_ptr() noexcept: ptr_(pointer()) {}
    constexpr unique_ptr(nullptr_t) noexcept: ptr_(pointer()) {}
    explicit unique_ptr(pointer p) noexcept: ptr_(p) {}
    unique_ptr(unique_ptr&& u) noexcept: ptr_(u.release()) {}
    template <typename U>
    unique_ptr(unique_ptr<U>&& u) noexcept: ptr_(u.release()) {}
    // destructor
    ~unique_ptr() { reset(); }
    // operator=
    unique_ptr& operator=( unique_ptr&& r ) noexcept { ptr_ = r.release(); return *this; }
    unique_ptr& operator=( std::nullptr_t ) noexcept { reset(); return *this; }

    // modifiers
    pointer release() noexcept {
        pointer ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }

    void reset(pointer ptr = pointer()) noexcept { 
        pointer tmp = ptr_;
        ptr_ = ptr;
        if (tmp) {
            delete tmp;
        }
    }

    void swap( unique_ptr& other ) noexcept { std::swap(ptr_, other.ptr_); }

    // observers
    pointer get() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    typename std::add_lvalue_reference<T>::type operator*() const noexcept(noexcept(*std::declval<pointer>())) { return *ptr_; }
    pointer operator->() const noexcept { return ptr_; }

private:
    pointer ptr_;
};



template <typename T>
class unique_ptr<T[]> {
public:
    // member types
    using element_type = T;
    using pointer = T*;

    // constructor
    constexpr unique_ptr() noexcept: ptr_(pointer()) {}
    constexpr unique_ptr(nullptr_t) noexcept: ptr_(pointer()) {}
    explicit unique_ptr( pointer p ) noexcept: ptr_(p) {}
    unique_ptr( unique_ptr&& u ) noexcept: ptr_(u.release()) {}
    template <typename U>
    unique_ptr(unique_ptr<U>&& u) noexcept: ptr_(u.release()) {}
    // destructor
    ~unique_ptr() { reset(); }
    // operator=
    unique_ptr& operator=( unique_ptr&& r ) noexcept { ptr_ = r.release(); return *this; }
    unique_ptr& operator=( std::nullptr_t ) noexcept { reset(); return *this; }

    // modifiers
    pointer release() noexcept {
        pointer ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }

    template <typename U>
    void reset(U ptr) noexcept { 
        pointer tmp = ptr_;
        ptr_ = ptr;
        if (tmp) {
            delete[] tmp;
        }
    }

    void reset(std::nullptr_t = nullptr) {
        pointer tmp = ptr_;
        ptr_ = nullptr;
        if (tmp) {
            delete[] tmp;
        }
    }

    void swap( unique_ptr& other ) noexcept { std::swap(ptr_, other.ptr_); }

    // observers
    pointer get() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    T& operator[](std::size_t i) const noexcept { return ptr_[i]; }

private:
    pointer ptr_;
};

// unique type checker 
template <typename T>
struct __unique_if {
    typedef unique_ptr<T> __unique_single;
};

template <typename T>
struct __unique_if<T[]> {
    typedef unique_ptr<T[]> __unique_unknown_bound;
};

template <typename T, size_t N>
struct __unique_if<T[N]> {
    typedef void __unique_known_bound;
};

// make unique
template <typename T, typename... Args>
inline typename __unique_if<T>::__unique_single
make_unique(Args&&... args) {
    return unique_ptr<T>(new T{std::forward<Args>(args)...});
}

template <typename T, typename... Args>
inline typename __unique_if<T>::__unique_unknown_bound
make_unique(size_t n) {
    typedef typename std::remove_extent<T>::type U;
    return unique_ptr<T>(new U[n]());
}

template <typename T, typename... Args>
typename __unique_if<T>::__unique_known_bound
make_unique(Args&&... args) = delete;

// compare
template <typename T, typename U>
inline bool operator==(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    return lhs.get() == rhs.get();
}

template <typename T>
inline bool operator==(const unique_ptr<T>& up, std::nullptr_t) {
    return !up;
}

template <typename T>
inline bool operator==(std::nullptr_t, const unique_ptr<T>& up) {
    return !up;
}

template <typename T, typename U>
inline bool operator!=(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
inline bool operator!=(const unique_ptr<T>& up, std::nullptr_t) {
    return static_cast<bool>(up);
}

template <typename T>
inline bool operator!=(std::nullptr_t, const unique_ptr<T>& up) {
    return static_cast<bool>(up);
}

template <typename T, typename U>
inline bool operator<(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    typedef typename unique_ptr<T>::pointer P1;
    typedef typename unique_ptr<U>::pointer P2;
    typedef typename std::common_type<P1, P2>::type VP;
    return std::less<VP>()(lhs.get(), rhs.get()); 
}

template <typename T>
inline bool operator<(const unique_ptr<T>& up, std::nullptr_t) {
    typedef typename unique_ptr<T>::pointer P;
    return std::less<P>(up.get(), nullptr);
}

template <typename T>
inline bool operator<(std::nullptr_t, const unique_ptr<T>& up) {
    typedef typename unique_ptr<T>::pointer P;
    return std::less<P>(nullptr, up.get());
}

template <typename T, typename U>
inline bool operator>(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    return rhs < lhs;
}

template <typename T>
inline bool operator>(const unique_ptr<T>& up, std::nullptr_t) {
    return nullptr < up;
}

template <typename T>
inline bool operator>(std::nullptr_t, const unique_ptr<T>& up) {
    return up < nullptr;
}

template <typename T, typename U>
inline bool operator<=(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    return !(lhs > rhs);
}

template <typename T>
inline bool operator<=(const unique_ptr<T>& up, std::nullptr_t) {
    return !(up > nullptr);
}

template <typename T>
inline bool operator<=(std::nullptr_t, const unique_ptr<T>& up) {
    return !(nullptr > up);
}

template <typename T, typename U>
inline bool operator>=(const unique_ptr<T>& lhs, const unique_ptr<U>& rhs) {
    return !(lhs < rhs);
}

template <typename T>
inline bool operator>=(const unique_ptr<T>& up, std::nullptr_t) {
    return !(up < nullptr);
}

template <typename T>
inline bool operator>=(std::nullptr_t, const unique_ptr<T>& up) {
    return !(nullptr < up);
}

} // ~end namespace xy


#endif
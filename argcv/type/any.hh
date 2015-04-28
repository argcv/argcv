#ifndef ARGCV_TYPE_ANY_HH
#define ARGCV_TYPE_ANY_HH

#include <type_traits>  // std::decay

//TODO it seems not work

namespace argcv {
namespace type {
class any {
public:  // structors
    any() : content(0) {}

    template <typename T>
    any(const T& value)
        : content(new holder<T>(value)) {}
    any(const any& other) : content(other.content ? other.content->clone() : 0) {}

    // Move constructor
    any(any&& other) : content(other.content) { other.content = 0; }

    // Perfect forwarding of ValueType
    template <typename T>
    any(T&& value)
        : content(new holder<typename std::decay<T>::type>(static_cast<T&&>(value))) {}

    ~any() { delete content; }

public:  // modifiers
    any& swap(any& rhs) {
        std::swap(content, rhs.content);
        return *this;
    }

    any& operator=(const any& rhs) {
        any(rhs).swap(*this);
        return *this;
    }

    // move assignement
    any& operator=(any&& rhs) {
        rhs.swap(*this);
        any().swap(rhs);
        return *this;
    }

    // Perfect forwarding of T
    // C++ 11 , ref: http://en.cppreference.com/w/cpp/types/decay
    template <class T>
    any& operator=(T&& rhs) {
        any(static_cast<T&&>(rhs)).swap(*this);
        return *this;
    }

public:  // queries
    bool empty() const { return !content; }
    void clear() { any().swap(*this); }

public:  // types (public so any_cast can be non-friend)
    class placeholder {
    public:  // structors
        virtual ~placeholder() {}

    public:  // queries
        virtual placeholder* clone() const = 0;
    };

    template <typename T>
    class holder : public placeholder {
    public:  // structors
        holder(const T& value) : held(value) {}

        holder(T&& value) : held(static_cast<T&&>(value)) {}

    public:  // queries
        virtual placeholder* clone() const { return new holder(held); }

    public:  // representation
        T held;

    private:  // intentionally left unimplemented
        holder& operator=(const holder&);
    };

private:  // representation
    template <typename T>
    friend T* any_cast(any*);

    template <typename T>
    friend T* unsafe_any_cast(any*);

    placeholder* content;
};

inline void swap(any& lhs, any& rhs) { lhs.swap(rhs); }

template <typename T>
inline const T* any_cast(const any* operand) {
    return any_cast<T>(const_cast<any*>(operand));
}

template <typename T>
inline T any_cast(any&& operand) {
    return any_cast<T>(operand);
}

// Note: The "unsafe" versions of any_cast are not part of the
// public interface and may be removed at any time. They are
// required where we know what type is stored in the any and can't
// use typeid() comparison, e.g., when our types may travel across
// different shared libraries.
template <typename T>
inline T* unsafe_any_cast(any* operand) {
    return &static_cast<any::holder<T>*>(operand->content)->held;
}

template <typename T>
inline const T* unsafe_any_cast(const any* operand) {
    return unsafe_any_cast<T>(const_cast<any*>(operand));
}
}
}

#endif  //  ARGCV_TYPE_ANY_HH
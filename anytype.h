#ifndef ANYTYPE_H
#define ANYTYPE_H

#include <stdexcept>
#include <string>

class anytype {

public:

    anytype() noexcept {}

    ~anytype() noexcept {
        free(ptr);
    }

    template <typename T>
    anytype(T const* p) noexcept {
        ptr = static_cast<void*>(const_cast<T*>(p));
        type_id = get_type_id(p);
        free = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        copy = [](void* ptr) -> void* {
            return static_cast<void*>(new T(*static_cast<T*>(ptr)));
        };
    }


    template <typename T>
    anytype(T const& i) noexcept {
        T* p = new T(i);
        ptr = static_cast<void*>(p);
        type_id = get_type_id(p);
        free = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        copy = [](void* ptr) -> void* {
            return static_cast<void*>(new T(*static_cast<T*>(ptr)));
        };
    }

    anytype& operator=(anytype const& p) noexcept {

        free = p.free;
        copy = p.copy;
        type_id = p.type_id;
        ptr = p.copy(p.ptr);
        return *this;

    }

    template <typename T, typename callable>
    anytype& match(callable const& func) noexcept {

        if (type_id == get_type_id<T>()) {

            func(*static_cast<T*>(ptr));

        }

        return *this;

    }


    template <typename T>
    bool match() noexcept {

        return type_id == get_type_id<T>();

    }

    template <typename T>
    void set(T const& i) noexcept {

        T* p = new T(i);
        ptr = static_cast<void*>(p);
        type_id = get_type_id(p);
        free = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        copy = [](void* ptr) -> void* {
            return static_cast<void*>(new T(*static_cast<T*>(ptr)));
        };

    }

    template <typename T>
    void set(T const* p) noexcept {

        ptr = static_cast<void*>(const_cast<T*>(p));
        type_id = get_type_id<T>();
        free = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        copy = [](void* ptr) -> void* {
            return static_cast<void*>(new T(*static_cast<T*>(ptr)));
        };

    }

    template <typename T>
    T& value() const {
        if (type_id == get_type_id<T>()) {
            return *static_cast<T*>(ptr);
        } else {
            throw std::runtime_error("Invalid anytype value");
        }
    }

    template <typename T>
    T cast() const noexcept {
        return T(*static_cast<T*>(ptr));
    }

private:

    int type_id;
    void* ptr;
    void (*free)(void*);
    void* (*copy)(void*);

    template <typename T>
    static int get_type_id(T* value = nullptr) {

        static char id;
        return reinterpret_cast<int>(&id);

    }

};

template <>
anytype::anytype<anytype>(anytype const& p) noexcept {

    free = p.free;
    copy = p.copy;
    type_id = p.type_id;
    ptr = p.copy(p.ptr);

}

template <>
anytype::anytype<char>(char const* i) noexcept {

    std::string* p = new std::string(i);
    ptr = static_cast<void*>(p);
    type_id = get_type_id(p);
    free = [](void* ptr) {
        delete static_cast<std::string*>(ptr);
    };
    copy = [](void* ptr) -> void* {
        return static_cast<void*>(new std::string(*static_cast<std::string*>(ptr)));
    };

}

template<>
void anytype::set<char>(char const* i) noexcept {

    std::string* p = new std::string(i);
    ptr = static_cast<void*>(p);
    type_id = get_type_id(p);
    free = [](void* ptr) {
        delete static_cast<std::string*>(ptr);
    };
    copy = [](void* ptr) -> void* {
        return static_cast<void*>(new std::string(*static_cast<std::string*>(ptr)));
    };

}


#endif // ANYTYPE_H

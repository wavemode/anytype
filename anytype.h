class anytype {

private:

    int type_id;
    void* ptr;
    void (*free)(void*);
    void* (*copy)(void*);

    template <typename T>
    static int get_type(T* value = nullptr) {

        static char id;
        return reinterpret_cast<int>(&id);

    }

public:

    template <typename T>
    anytype(T* p) {
        ptr = static_cast<void*>(p);
        type_id = get_type(p);
        free = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        copy = [](void* ptr) -> void* {
            return static_cast<void*>(new T(*static_cast<T*>(ptr)));
        };
    }

    anytype(anytype const& p) {

        free = p.free;
        copy = p.copy;
        type_id = p.type_id;
        ptr = p.copy(p.ptr);

    }

    anytype& operator=(anytype const& p) {

        free = p.free;
        copy = p.copy;
        type_id = p.type_id;
        ptr = p.copy(p.ptr);
        return *this;

    }

    ~anytype() {
        free(ptr);
    }

    template <typename T, typename callable>
    anytype& match(callable func) {

        if (type_id == get_type<T>()) {

            func(static_cast<T*>(ptr));

        }

        return *this;

    }

};

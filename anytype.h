#ifndef ANYTYPE_H
#define ANYTYPE_H

#include <stdexcept>
#include <string>

class anytype {

public:

	anytype() {}

	~anytype() {
		clear();
	}

	anytype(anytype const& p) {

		clear();
		free = p.free;
		copy = p.copy;
		type_id = p.type_id;
		if (p.type_id) ptr = p.copy(p.ptr);

	}

	anytype(char const* i) {

		clear();
		std::string* p = new std::string(i);
		ptr = static_cast<void*>(p);
		type_id = get_type_id<std::string>();
		free = [](void* ptr) {
			delete static_cast<std::string*>(ptr);
		};
		copy = [](void* ptr) -> void* {
			return static_cast<void*>(new std::string(*static_cast<std::string*>(ptr)));
		};

	}

	template <typename T>
	anytype(T* p) {

		clear();
		ptr = static_cast<void*>(p);
		type_id = get_type_id<T>();
		free = [](void* ptr) {
			delete static_cast<T*>(ptr);
		};
		copy = [](void* ptr) -> void* {
			return static_cast<void*>(new T(*static_cast<T*>(ptr)));
		};
	}


	template <typename T>
	explicit anytype(T const& i) {

		clear();
		T* p = new T(i);
		ptr = static_cast<void*>(p);
		type_id = get_type_id<T>();
		free = [](void* ptr) {
			delete static_cast<T*>(ptr);
		};
		copy = [](void* ptr) -> void* {
			return static_cast<void*>(new T(*static_cast<T*>(ptr)));
		};
	}

	anytype& operator=(anytype const& p) {

		clear();
		free = p.free;
		copy = p.copy;
		type_id = p.type_id;
		if (p.type_id) ptr = p.copy(p.ptr);
		return *this;

	}

	template <typename T>
	anytype& operator=(T const& p) {
		set(p);
		return *this;
	}

	template <typename T, typename Callable>
	anytype& match(Callable const& func) {

		if (type_id == get_type_id<T>()) {

			func(*static_cast<T*>(ptr));

		}

		return *this;

	}


	template <typename T>
	bool match() {

		return type_id == get_type_id<T>();

	}

	template <typename T>
	void set(T const& i) {

		clear();
		T* p = new T(i);
		ptr = static_cast<void*>(p);
		type_id = get_type_id<T>();
		free = [](void* ptr) {
			delete static_cast<T*>(ptr);
		};
		copy = [](void* ptr) -> void* {
			return static_cast<void*>(new T(*static_cast<T*>(ptr)));
		};

	}

	void set(char const* i) {

		clear();
		std::string* p = new std::string(i);
		ptr = static_cast<void*>(p);
		type_id = get_type_id<std::string>();
		free = [](void* ptr) {
			delete static_cast<std::string*>(ptr);
		};
		copy = [](void* ptr) -> void* {
			return static_cast<void*>(new std::string(*static_cast<std::string*>(ptr)));
		};

	}

	template <typename T>
	void set(T* p) {

		clear();
		ptr = static_cast<void*>(p);
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
		}
		else {
			throw std::runtime_error("Invalid anytype value.");
		}
	}

	void clear() {

		if (type_id && ptr != nullptr) {
			free(ptr);
			ptr = nullptr;
		}
		type_id = 0;

	}

private:

	long long int type_id = 0;
	void* ptr;
	void(*free)(void*);
	void* (*copy)(void*);

	template <typename T>
	static long long int get_type_id() {

		static const char addr;

		return reinterpret_cast<long long int>(&addr);

	}

};



#endif // ANYTYPE_H

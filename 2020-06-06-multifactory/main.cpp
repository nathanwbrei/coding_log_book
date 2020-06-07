#include <iostream>
#include <vector>

/// TODO:
/// 1. Assert that the types are unique
/// 2. Make sense of this: https://stackoverflow.com/questions/3273504/is-it-possible-to-transform-the-types-in-a-parameter-pack
/// 3.

/// Base case
template <typename... Ts>
struct Multifactory {
	void print() {
		std::cout << std::endl;
	};
	void set_all() {};
	void set_everything() {};

	template <typename U>
	U& get() {
		// TODO: Want a compile-time error instead
	 	throw std::runtime_error("Not found!");
	}

	template <typename U>
	void set(U u) {
		throw std::runtime_error("Not found!");
	}

};

/// Specialization for non-empty template parameters
template <typename T, typename... Ts>
struct Multifactory<T, Ts...> {
	T data;
	Multifactory<Ts...> f;

	explicit Multifactory(T t, Ts... ts) : f(ts...), data(t) {}


	void print() {
		std::cout << data << ", ";
		f.print();
	}

	void set_all(T t, Ts... ts) {
		data = t;
		f.set_all(ts...);
	}

	template <typename... Us>
	void set_all(T* t, Us... us) {
		data = *t;
		f.set_all(us...);
	}

	template <typename U>
	U& get() {
		return f.template get<U>();
	}

	template <>
	T& get() {
		return data;
	}

	template <typename U>
	void set(U u) {
		f.template set<U>(u);
	}

	template <>
	void set(T t) {
		data = t;
	}

	void set_everything(std::vector<T*> t, std::vector<Ts*>... ts) {
		data = *t[0];
		f.set_everything(ts...);
	}


};



int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	Multifactory<int, double, char> f (7, 9.0, 'c');

	f.print();
	f.set_all(8, 10.0, 'd');

	int x = f.get<int>();
	double y = f.get<double>();
	char z = f.get<char>();
	//char a = f.get<float>(); // runtime error

	std::cout << x << ", " << y << ", " << z << std::endl;

	f.set<double>(22.2);
	f.print();

	int xx = 77;
	double yy = 19.9;
	char zz = 'w';

	f.set_all(&xx, &yy, &zz);
	f.print();

	xx = 88;
	yy = 33.3;
	zz = 'z';
	std::vector<int*> xxx = {&xx};
	std::vector<double*> yyy = {&yy};
	std::vector<char*> zzz = {&zz};

	f.set_everything(xxx, yyy, zzz);
	f.print();

}



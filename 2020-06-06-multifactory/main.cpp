#include <iostream>
#include <vector>
#include <tuple>

/// Implementation of std::apply, which only becomes available in C++17
/// Copied from
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3915.pdf
template <typename F, typename Tuple, size_t... I>
decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>) {
	return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename F, typename Tuple>
decltype(auto) my_apply(F&& f, Tuple&& t) {
	using Indices = std::make_index_sequence< std::tuple_size< std::decay_t< Tuple> >::value>;
	return apply_impl(std::forward<F>(f), std::forward<Tuple>(t), Indices{});
}


template <typename... Ts>
struct Multifactory {

	std::tuple<std::vector<Ts*>...> data;

	void set(std::vector<Ts*>... ts) {
		data = std::make_tuple(ts...);
	}

	template<typename T>
	std::vector<T*>& get() {
		return std::get<std::vector<T*>>(data);
	}

	virtual void process(int event_nr, std::vector<Ts*>&... destinations) = 0;

	void do_process(int event_nr) {
		std::cout << "Calling do_process" << std::endl;
		using Indices = std::make_index_sequence<sizeof...(Ts)>;
		do_process_helper(event_nr, Indices{});
	}

	template <size_t... I>
	void do_process_helper(int event_nr, std::index_sequence<I...>) {
		process(event_nr, std::get<I>(data)...);
	}



	template <int level = 0>
	void print() {
		auto x = std::get<level>(data);
		for (int i=0; i<x.size(); ++i) {
			std::cout << *(x[i]) << ", ";
		}
		std::cout << "; ";
		print<level+1>();
	}

	template <>
	void print<sizeof...(Ts)>() {
		std::cout << std::endl;
	}
};

struct MyFac : public Multifactory<int, double, char> {

	void process(int event_nr, std::vector<int*>& ao, std::vector<double*>& bo, std::vector<char*>& co) override {
		std::cout << "Calling process" << std::endl;
		ao.push_back(new int(22));
		co.push_back(new char('k'));
	}
};

int main() {
	std::cout << "Hello world from multifactory" << std::endl;
	MyFac f;

	f.print();
	f.do_process(0);
	f.print();

	auto ints = f.get<int>();
	std::cout << *ints[0];
}



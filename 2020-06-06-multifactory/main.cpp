#include <iostream>
#include <vector>


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
		std::tuple<std::vector<Ts*>&...> data_refs = data;
		auto all_args = std::tuple_cat(std::make_tuple(this, event_nr), data_refs);
		std::apply(&Multifactory::process, all_args);
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
}



#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <any>

struct BaseData {
    double d;
    BaseData(double d) : d(d) {};
};

struct DerivedData : public BaseData {
    int i;
    DerivedData(double d, int i): BaseData(d), i(i) {};
};

struct UnrelatedData {
    float f;
};

class JFactory;


template <typename T> class JFactoryT;

struct JFactory {
    std::unordered_map<std::type_index, std::any> vtable;

    template<typename S>
    const std::vector<S*> get_as() {
        std::vector<S*> results;
        auto ti = std::type_index(typeid(S));
        auto search = vtable.find(ti);
        if (search != vtable.end()) {
            std::cout << "Found conversion to " << typeid(S).name() << std::endl;
            std::any upcast = search->second;
            auto upcast_fn = std::any_cast<std::function<std::vector<S*>()>> (upcast);
            results = upcast_fn();
        }
        return results;
    }
};


template <typename T> struct JFactoryT : public JFactory {

    std::vector<T*> data;

    void insert(T* item) {
        data.push_back(item);
    }

    JFactoryT() {
        allow_cast_to<T>();
    }


    const std::vector<T*>& get() const {
        return data;
    }

    // Consider:
    // - Allow user to provide their own lambdas?
    // - Does static cast always work? When does it and when doesn't it?
    // - Do we want to try sharing vtables? Right now it is one vtable per instance
    template <typename S>
    void allow_cast_to() {

        auto upcast_lambda = [this]() {
            std::vector<S*> results;
            for (auto t : data) {
                results.push_back(static_cast<S*>(t));
            }
            return results;
        };

        auto upcast_fn = std::function<std::vector<S*>()> (upcast_lambda);
        auto key = std::type_index(typeid(S));
        vtable[key] = std::any(upcast_fn);
        std::cout << "Adding conversion to " << typeid(S).name() << std::endl;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    auto fdd = new JFactoryT<DerivedData>;
    fdd->insert(new DerivedData {7.0, 2});
    fdd->allow_cast_to<BaseData>();

    std::cout << "From DerivedData" << std::endl;
    auto bs = fdd->get_as<BaseData>();
    for (auto x: bs) {
        std::cout << "Got " << x->d << std::endl;
    }

    std::cout << "From UnrelatedData" << std::endl;
    auto fud = new JFactoryT<UnrelatedData>;
    fud->insert(new UnrelatedData {9.0});
    JFactory* f = static_cast<JFactory*>(fud);

    bs = f->get_as<BaseData>();
    for (auto x: bs) {
        std::cout << "Got " << x->d << std::endl;
    }

    std::cout << "Get DerivedData as DerivedData" << std::endl;
    auto dds = fdd->get_as<DerivedData>();
    for (auto x: dds) {
        std::cout << "Got " << x->d << ", " << x->i << std::endl;
    }
}

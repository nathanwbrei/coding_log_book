#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>

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


struct JFactory {

};


template <typename T> struct JFactoryT : public JFactory {

    std::vector<T*> data;

    void insert(T* item) {
        data.push_back(item);
    }

    const std::vector<T*>& get() const {
        return data;
    }

    template <typename S>
    const std::vector<S*> get_as() const {
        std::vector<S*> results;
        for (auto t : data) {
            results.push_back(static_cast<S*>(t));
        }
        return results;
    };
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    JFactoryT<DerivedData> fdd;
    fdd.insert(new DerivedData {7.0, 2});

    JFactoryT<UnrelatedData> fud;
    fud.insert(new UnrelatedData {9.0});


    JFactory* factory = static_cast<JFactory*>(&fdd);

    auto output = factory.get_as<BaseData>();
    for (auto item : output) {
        std::cout << item->d << std::endl;
    }
    return 0;
}

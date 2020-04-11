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

struct JFactoryVisitor {
    std::vector<BaseData*> results;

    template <typename T> void visit(T* t) {
        std::cout << "Visiting " << typeid(T).name() << std::endl;
    }
};

struct JFactory {

    virtual void accept(JFactoryVisitor& visitor) = 0;
// template<typename S>
//     std::vector<S*> get_downcasted() {
//         std::vector<S*> results;
//         for (auto i : data) {
//             results.push_back(i);
//         }
//         return results;
//     }
};


template <typename T> struct JFactoryT : public JFactory {
    std::vector<T*> data;

    void accept(JFactoryVisitor& visitor) override {
        visitor.visit(this);
    }

    void insert(T* item) {
        data.push_back(item);
    }

    const std::vector<T*>& get() const {
        return data;
    }
};

template <typename B, typename D>
std::vector<B*> get_downcasted(JFactoryT<D>* f) {
    auto input = f->get();
    std::vector<B*> output;
    for (auto i : input) {
        output.push_back(static_cast<B*>(output));
    }
    return output;
}


struct JEvent {
    std::map<std::type_index, JFactory*> factories;

    template <typename T>
    void put(T* item) {
        auto ti = std::type_index(typeid(T));
        auto f = new JFactoryT<T>();
        f->insert(item);
        factories.insert(std::make_pair(ti, f));
    }

    template <typename T>
    const std::vector<T*>& get() {
        auto ti = std::type_index(typeid(T));
        auto f = factories[ti];
        auto tf = static_cast<JFactoryT<T>*>(f);
        return tf->get();
    }

    template <typename S>
    const std::vector<S*> getAll() {
        JFactoryVisitor visitor;
        std::vector<S*> results;
        for (auto p: factories) {
            auto f = p.second;
            p.second->accept(visitor);
        }
        return results;
    }

};


int main() {
    std::cout << "Hello, World!" << std::endl;

    JEvent event;
    event.put(new BaseData {3.0});
    event.put(new DerivedData {7.0, 2});
    event.put(new UnrelatedData {7.0});

    //auto output = event.get<DerivedData>();
    //std::cout << output[0]->i << std::endl;

    auto all = event.getAll<BaseData>();
    for (auto item : all) {
        std::cout << item->d << std::endl;
    }
    return 0;
}

#include <iostream>

// The thing the user is supposed to extend
// NOTE: JEventProcessor ought to be a Concept (from C++20), which obviously none of our compilers will support.
// For ergonomics we can make the base class be runtime-polymorphic but use static polymorphism behind the scenes.
class JEventProcessor {
public:
    virtual void process_parallel() {
        std::cout << "Base process_parallel" << std::endl;
    };
    virtual void process_sequential() {
        std::cout << "Base process_sequential" << std::endl;
    };
};

// The JEventProcessor backend. The logic for preserving invariants lives here.
// If the user declares their virtual functions final, the optimizer can devirtualize entirely.
template <typename EP>
class JAbstractEventProcessor {
public:
    EP* underlying;
    JAbstractEventProcessor(EP* es) : underlying(es) {};
    void do_process() {
        if (typeid(&EP::process_parallel) != typeid(&JEventProcessor::process_parallel)) {
            underlying->process_parallel();
        }
        if (typeid(&EP::process_sequential) != typeid(&JEventProcessor::process_sequential)) {
            underlying->process_sequential();
        }
    }
};

// A user-defined JEventProcessor. Note that they only implement process_parallel and the user is able to
class MyEventProcessor : public JEventProcessor {
public:
    void process_parallel() override final {
        std::cout << "Derived process_parallel" << std::endl;
    }
};

class JApplication {
public:
    template<typename JEventProcessorT>
    void run(JEventProcessorT* es) {
        static_assert(std::is_base_of<JEventProcessor, JEventProcessorT>::value);
        JAbstractEventProcessor<JEventProcessorT> aes(es);
        aes.do_process();
    }
};

int main() {
    MyEventProcessor es;
    JApplication app;
    app.run(&es);
    return 0;
}



#include <iostream>

template <typename T>
concept JEventProcessorConcept = requires(T t) {
    { t.process_parallel() } -> void;
    { t.process_sequential() } -> void;
};

template <JEventProcessorConcept EP>
class JAbstractEventProcessor {
public:
    EP underlying;
    JAbstractEventProcessor(EP&& ep) : underlying(ep) {};
    void do_process() {
        underlying.process_parallel();
        underlying.process_sequential();
    }
};

class JEventProcessor {
public:
    virtual void process_parallel() {};
    virtual void process_sequential() {};
};

class MyEventProcessor : public JEventProcessor {
public:
    void process_parallel() override final {
        std::cout << "Hello from process_parallel" << std::endl;
    }
};

class JApplication {
public:
    template<JEventProcessorConcept EP>
    void add(EP event_processor) {};
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    MyEventProcessor ep;
    JAbstractEventProcessor<MyEventProcessor> aes(std::move(ep));
    aes.do_process();

    // JApplication app; app.add(es);
    return 0;
}

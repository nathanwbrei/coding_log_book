
#ifndef _TriggerDecision_Factory_Slow_h_
#define _TriggerDecision_Factory_Slow_h_

#include <JANA/JFactoryT.h>

#include "TriggerDecision.h"

class TriggerDecision_Factory_Slow : public JFactoryT<TriggerDecision> {

    // Insert any member variables here

public:
    TriggerDecision_Factory_Slow() : JFactoryT<TriggerDecision>(NAME_OF_THIS) {};
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override;
    void Process(const std::shared_ptr<const JEvent> &event) override;

};

#endif // _TriggerDecision_Factory_Slow_h_


#ifndef _ProtoECalTrackFactory_h_
#define _ProtoECalTrackFactory_h_

#include <JANA/JFactoryT.h>

#include "ProtoECalTrack.h"

class ProtoECalTrackFactory : public JFactoryT<ProtoECalTrack> {

    // Insert any member variables here

public:
    ProtoECalTrackFactory() : JFactoryT<ProtoECalTrack>(NAME_OF_THIS) {};
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override;
    void Process(const std::shared_ptr<const JEvent> &event) override;

};

#endif // _ProtoECalTrackFactory_h_


#ifndef _ProtoECalTrackFactory_h_
#define _ProtoECalTrackFactory_h_

#include <JANA/JFactoryT.h>

#include "ProtoECalTrack.h"

class ProtoECalTrack_Factory : public JFactoryT<ProtoECalTrack> {

    static const size_t GRID_HEIGHT = 3;
	static const size_t GRID_WIDTH = 3;

public:
    ProtoECalTrack_Factory() : JFactoryT<ProtoECalTrack>(NAME_OF_THIS) {};
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override;
    void Process(const std::shared_ptr<const JEvent> &event) override;

};

#endif // _ProtoECalTrackFactory_h_

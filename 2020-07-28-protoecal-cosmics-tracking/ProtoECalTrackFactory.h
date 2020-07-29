
#ifndef _ProtoECalTrackFactory_h_
#define _ProtoECalTrackFactory_h_

#include <JANA/JFactoryT.h>

#include "ProtoECalTrack.h"

class ProtoECalTrackFactory : public JFactoryT<ProtoECalTrack> {

    // Insert any member variables here
    static const size_t GRID_HEIGHT = 3;
	static const size_t GRID_WIDTH = 3;

public:
    ProtoECalTrackFactory() : JFactoryT<ProtoECalTrack>(NAME_OF_THIS) {};
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override;
    void Process(const std::shared_ptr<const JEvent> &event) override;

	bool test_for_tracks(bool* pixel_grid[GRID_HEIGHT][GRID_WIDTH]);

};

#endif // _ProtoECalTrackFactory_h_


#ifndef _TriggerDecision_Factory_Fast_h_
#define _TriggerDecision_Factory_Fast_h_

#include <JANA/JFactoryT.h>

#include "TriggerDecision.h"

class TriggerDecision_Factory_Fast : public JFactoryT<TriggerDecision> {

    // Insert any member variables here
    static const size_t GRID_HEIGHT = 3;
	static const size_t GRID_WIDTH = 3;

public:
    TriggerDecision_Factory_Fast() : JFactoryT<TriggerDecision>(NAME_OF_THIS) {};
    void Init() override;
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override;
    void Process(const std::shared_ptr<const JEvent> &event) override;

	static int match_track_pattern(bool pixel_grid[GRID_HEIGHT][GRID_WIDTH]);
};

#endif // _TriggerDecision_Factory_Fast_h_

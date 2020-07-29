
#include "TriggerDecision_Factory_Fast.h"
#include "ProtoECalHit.h"

#include <JANA/JEvent.h>

void TriggerDecision_Factory_Fast::Init() {
    auto app = GetApplication();
    
    /// Acquire any parameters
    // app->GetParameter("parameter_name", m_destination);
    
    /// Acquire any services
    // m_service = app->GetService<ServiceT>();
    
    /// Set any factory flags
    // SetFactoryFlag(JFactory_Flags_t::NOT_OBJECT_OWNER);
}

void TriggerDecision_Factory_Fast::ChangeRun(const std::shared_ptr<const JEvent> &event) {
    /// This is automatically run before Process, when a new run number is seen
    /// Usually we update our calibration constants by asking a JService
    /// to give us the latest data for this run number
    
    auto run_nr = event->GetRunNumber();
    // m_calibration = m_service->GetCalibrationsForRun(run_nr);
}

void TriggerDecision_Factory_Fast::Process(const std::shared_ptr<const JEvent> &event) {

    /// JFactories are local to a thread, so we are free to access and modify
    /// member variables here. However, be aware that events are _scattered_ to
    /// different JFactory instances, not _broadcast_: this means that JFactory 
    /// instances only see _some_ of the events. 
    
    /// Acquire inputs (This may recursively call other JFactories)
    // auto inputs = event->Get<...>();

	// Initialize pixel grid
	bool pixels[GRID_HEIGHT][GRID_WIDTH];
	for (int i=0; i<GRID_HEIGHT; ++i) {
		for (int j=0; j<GRID_WIDTH; ++j) {
			pixels[i][j] = false;
		}
	}

	// Update pixel grid with hit information
	auto hits = event->Get<ProtoECalHit>();
	for (auto hit : hits) {
		assert(hit->row >= 0);
		assert(hit->col >= 0);
		assert(hit->row < 3);
		assert(hit->col < 3);
		pixels[hit->row][hit->col] = true;
	}

	/// Publish outputs
	if (match_track_pattern(pixels)) {
		Insert(new TriggerDecision(true, "Matched pattern"));
	}
	else {
		Insert(new TriggerDecision(true, "No match found"));
	}
}

int TriggerDecision_Factory_Fast::match_track_pattern(bool pixel_grid[GRID_HEIGHT][GRID_WIDTH]) {
	//    +-+-+-+
	//    |1|2|3|
	//    +-+-+-+
	//    |4|5|6|
	//    +-+-+-+
	//    |7|8|9|
	//    +-+-+-+

	// Convert the pixel grid into a bitset so that we can quickly compare all acceptable patterns
	uint32_t bitset = 0;
	for (uint32_t row=0; row<GRID_HEIGHT; row++) {
		for (uint32_t col=0; col<GRID_WIDTH; col++) {
			bitset |= (1u << (row*3 + col));
		}
	}

	// Patterns are all known ahead of time
	const uint32_t patterns[13] = {0b100'100'100, 0b010'010'010, 0b001'001'001,  // Verticals
	                               0b100'010'001, 0b001'010'100, // Diagonals
	                               0b100'010'010, 0b010'100'100, 0b010'001'001, 0b001'010'010,
	                               0b001'010'001, 0b100'010'100, 0b010'100'010, 0b010'001'010};

	// Return true on the first matching pattern we find
	for (uint32_t pattern : patterns) {
		if ((bitset & pattern) != 0) {
			return true;
		}
	}
	return false;
}


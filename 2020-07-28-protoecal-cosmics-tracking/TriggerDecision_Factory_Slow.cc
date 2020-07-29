
#include "TriggerDecision_Factory_Slow.h"
#include "ProtoECalTrack.h"

#include <JANA/JEvent.h>

void TriggerDecision_Factory_Slow::Init() {
    auto app = GetApplication();
    
    /// Acquire any parameters
    // app->GetParameter("parameter_name", m_destination);
    
    /// Acquire any services
    // m_service = app->GetService<ServiceT>();
    
    /// Set any factory flags
    // SetFactoryFlag(JFactory_Flags_t::NOT_OBJECT_OWNER);
}

void TriggerDecision_Factory_Slow::ChangeRun(const std::shared_ptr<const JEvent> &event) {
    /// This is automatically run before Process, when a new run number is seen
    /// Usually we update our calibration constants by asking a JService
    /// to give us the latest data for this run number
    
    auto run_nr = event->GetRunNumber();
    // m_calibration = m_service->GetCalibrationsForRun(run_nr);
}

void TriggerDecision_Factory_Slow::Process(const std::shared_ptr<const JEvent> &event) {

    auto tracks = event->Get<ProtoECalTrack>();
    if (tracks.empty()) {
	    Insert(new TriggerDecision(false, "No tracks found"));
    }
    else {
	    Insert(new TriggerDecision(true, "Tracks found"));
    }

}


#include "ProtoECalTrack_Factory.h"

#include <JANA/JEvent.h>

void ProtoECalTrack_Factory::Init() {
    auto app = GetApplication();
    
    /// Acquire any parameters
    // app->GetParameter("parameter_name", m_destination);
    
    /// Acquire any services
    // m_service = app->GetService<ServiceT>();
    
    /// Set any factory flags
    // SetFactoryFlag(JFactory_Flags_t::NOT_OBJECT_OWNER);
}

void ProtoECalTrack_Factory::ChangeRun(const std::shared_ptr<const JEvent> &event) {
    /// This is automatically run before Process, when a new run number is seen
    /// Usually we update our calibration constants by asking a JService
    /// to give us the latest data for this run number
    
    auto run_nr = event->GetRunNumber();
    // m_calibration = m_service->GetCalibrationsForRun(run_nr);
}

void ProtoECalTrack_Factory::Process(const std::shared_ptr<const JEvent> &event) {

    /// JFactories are local to a thread, so we are free to access and modify
    /// member variables here. However, be aware that events are _scattered_ to
    /// different JFactory instances, not _broadcast_: this means that JFactory 
    /// instances only see _some_ of the events.


	/// Do some computation
	//ProtoECalTrack* track = new ProtoECalTrack(hits);

    /// Publish outputs
    std::vector<ProtoECalTrack*> results;
    //results.push_back(track);
    Set(results);
}


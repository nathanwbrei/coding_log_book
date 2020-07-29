
#include "ProtoECalHit.h"
#include "ProtoECalTrack.h"
#include "ProtoECalTrack_Factory.h"
#include "TriggerDecision.h"
#include "TriggerDecision_Factory_Fast.h"

#include <JANA/JEvent.h>

int main() {

	// Create JEvent
	auto event = std::make_shared<JEvent>();
	event->SetFactorySet(new JFactorySet());

	// Create and insert input hits
	//    +-+-+-+
	//    |x| | |
	//    +-+-+-+
	//    |x| | |
	//    +-+-+-+
	//    |x| | |
	//    +-+-+-+

	std::vector<ProtoECalHit*> hits;
	hits.push_back(new ProtoECalHit(0,0,1.0,1));
	hits.push_back(new ProtoECalHit(1,0,1.0,1));
	hits.push_back(new ProtoECalHit(2,0,1.0,1));
	event->Insert(hits);

	// Run factory
	JApplication app;
	ProtoECalTrack_Factory f;
	auto result = f.GetOrCreate(event, &app, 0);

	// Check output tracks
	int count = 0;
	for (auto track_iter = result.first; track_iter != result.second; track_iter++) {
		std::cout << "Track " << count++ << std::endl;
		for (auto hit : (*track_iter)->hits) {
			std::cout << "   Hit " << hit->row << ", " << hit->col << std::endl;
		}
	}

	TriggerDecision_Factory_Fast ff;
	auto res2 = ff.GetOrCreate(event, &app, 0);
	std::cout << "TriggerDecision = " << (*res2.first)->decision << std::endl;

}

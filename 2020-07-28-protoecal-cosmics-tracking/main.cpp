
#include "ProtoECalHit.h"
#include "ProtoECalTrack.h"
#include "ProtoECalTrackFactory.h"

#include <JANA/JEvent.h>

int main() {

	// Create JEvent
	auto event = std::make_shared<JEvent>();
	event->SetFactorySet(new JFactorySet());

	// Create and insert input hits
	std::vector<ProtoECalHit*> hits;
	event->Insert(hits);

	// Run factory
	JApplication app;
	ProtoECalTrackFactory f;
	auto result = f.GetOrCreate(event, &app, 0);

	// Check output tracks
	int count = 0;
	for (auto track_iter = result.first; track_iter != result.second; track_iter++) {
		std::cout << "Track " << count++ << std::endl;
		for (auto hit : (*track_iter)->hits) {
			std::cout << "   Hit " << hit->row << ", " << hit->col << std::endl;
		}
	}

}

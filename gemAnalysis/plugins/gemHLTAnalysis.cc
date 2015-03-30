// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <DataFormats/DetId/interface/DetId.h>
#include <DataFormats/MuonDetId/interface/MuonSubdetId.h>
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

class gemHLTAnalysis : public edm::EDAnalyzer {
public:
  explicit gemHLTAnalysis(const edm::ParameterSet&);
  ~gemHLTAnalysis();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::InputTag recoMuonInput_;

};

gemHLTAnalysis::gemHLTAnalysis(const edm::ParameterSet& iConfig)

{
  recoMuonInput_ = iConfig.getParameter<edm::InputTag>("recoMuonInput");
}


gemHLTAnalysis::~gemHLTAnalysis()
{
 
}

void
gemHLTAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
   
  Handle<reco::TrackCollection> muons;
  if (!iEvent.getByLabel(recoMuonInput_,muons)) return;

  
  for (const reco::Track & m: *muons){
      cout << "m.pt "<< m.pt()
	   << " gemStationsWithAnyHits "<< m.hitPattern().gemStationsWithAnyHits()
	   << " cscStationsWithAnyHits "<< m.hitPattern().cscStationsWithAnyHits()
	   <<endl;
  }
  
  //   for (trackingRecHit_iterator hit = m.recHitsBegin(); hit != m.recHitsEnd(); ++hit) {
//       // DetId recoid = (*hit)->geographicalId();
//       // if ( recoid.det() == DetId::Muon ){
	
//       // rawId
//       // if((*hit)->isValid()) {
//       //   // unsigned int id = (*hit)->geographicalId().rawId();
//       //   // unsigned int subd = id >> (DetId::kSubdetOffset);

//       //     //	    DetId recoid = (*hit)->geographicalId();
//       //   //if ( recoid.det() == DetId::Muon ){
//       // 	    cout << "m.pt "<< m.pt()
//       // 	 // << " id "<< id
//       // 	 // << " subd "<< subd
//       // 	 <<endl;
//       // }
//     }
//   }
// }
}


// ------------ method called once each job just before starting event loop  ------------
void 
gemHLTAnalysis::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
gemHLTAnalysis::endJob() 
{
}
void
gemHLTAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(gemHLTAnalysis);

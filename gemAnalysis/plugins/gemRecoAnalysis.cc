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

#include <DataFormats/DetId/interface/DetId.h>
#include <DataFormats/MuonDetId/interface/MuonSubdetId.h>
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

class gemRecoAnalysis : public edm::EDAnalyzer {
public:
  explicit gemRecoAnalysis(const edm::ParameterSet&);
  ~gemRecoAnalysis();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::InputTag recoMuonInput_;

};

gemRecoAnalysis::gemRecoAnalysis(const edm::ParameterSet& iConfig)

{
  recoMuonInput_ = iConfig.getParameter<edm::InputTag>("recoMuonInput");
}


gemRecoAnalysis::~gemRecoAnalysis()
{
 
}

void
gemRecoAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
   
  Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(recoMuonInput_,muons);

  for (const reco::Muon & m: *muons){
    if (m.isGlobalMuon()){
      reco::TrackRef track = m.combinedMuon();

      cout << "m.pt "<< m.pt()
	   << " gemStationsWithAnyHits "<< track->hitPattern().gemStationsWithAnyHits()
	   << " cscStationsWithAnyHits "<< track->hitPattern().cscStationsWithAnyHits()
	   <<endl;

      // for (trackingRecHit_iterator hit = track->recHitsBegin(); hit != track->recHitsEnd(); ++hit) {
      // 	cout << "m.pt "<< m.pt()
      // 	     << " rawId "<< (*hit)->rawId()
      // 		 <<endl;

      // 	// rawId
      // 	// if((*hit)->isValid()) {
      // 	//   // unsigned int id = (*hit)->geographicalId().rawId();
      // 	//   // unsigned int subd = id >> (DetId::kSubdetOffset);

      // 	//     //	    DetId recoid = (*hit)->geographicalId();
      // 	//   //if ( recoid.det() == DetId::Muon ){
      // 	// 	    cout << "m.pt "<< m.pt()
      // 	// 	 // << " id "<< id
      // 	// 	 // << " subd "<< subd
      // 	// 	 <<endl;
      // 	// }
      // }
    }
  }
}



// ------------ method called once each job just before starting event loop  ------------
void 
gemRecoAnalysis::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
gemRecoAnalysis::endJob() 
{
}
void
gemRecoAnalysis::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(gemRecoAnalysis);

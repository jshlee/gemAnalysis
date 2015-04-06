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
  int m_ngemHits,m_ntracks,m_ntracksWithGem;

};

gemRecoAnalysis::gemRecoAnalysis(const edm::ParameterSet& iConfig)

{
  recoMuonInput_ = iConfig.getParameter<edm::InputTag>("recoMuonInput");
  m_ngemHits = 0;
  m_ntracks = 0;
  m_ntracksWithGem = 0;
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
      if (fabs(m.eta()) < 1.5 ) continue;
      if (fabs(m.eta()) > 2.5 ) continue;
      
      reco::TrackRef track = m.combinedMuon();
      m_ntracks++;

      bool hasGem = false;
      if (track.isNonnull()){
	for (trackingRecHit_iterator hit = track->recHitsBegin(); hit != track->recHitsEnd(); ++hit) {
	  DetId recoid = (*hit)->geographicalId();
	  cout << " subdetId()  "<< recoid.subdetId() 
	       <<endl;
	  if (recoid.subdetId() == 4){
	    m_ngemHits++;
	    hasGem = true;
	  }
	  // gem is subdetId == 4
	}
	if (hasGem)
	  m_ntracksWithGem++;
      }
      
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
  std::cout << " total tracks        : "<< m_ntracks << std::endl;
  std::cout << " total gem hits      : "<< m_ngemHits << std::endl;
  std::cout << " tracks with gem hits: "<< m_ntracksWithGem << std::endl;
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

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <DataFormats/DetId/interface/DetId.h>
#include <DataFormats/MuonDetId/interface/MuonSubdetId.h>
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"

#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "TH1F.h"
#include "TFile.h"

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

  int m_ngemHits,m_ntracks,m_ntracksWithGem;
  edm::Service<TFileService> fs;
  TH1F* h_gemRecHit;
  TH1F* h_gemDigi;
  
};

gemHLTAnalysis::gemHLTAnalysis(const edm::ParameterSet& iConfig)

{
  recoMuonInput_ = iConfig.getParameter<edm::InputTag>("recoMuonInput");
  m_ngemHits = 0;
  m_ntracks = 0;
  m_ntracksWithGem = 0;
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

  Handle<GEMRecHitCollection> gemRecHits; 
  iEvent.getByLabel("gemRecHits", gemRecHits);
  Handle<GEMDigiCollection> gemDigis;
  iEvent.getByLabel("simMuonGEMDigis", gemDigis);

  for(GEMDigiCollection::DigiRangeIterator cItr = gemDigis->begin(); cItr != gemDigis->end(); ++cItr){
    GEMDetId id = (*cItr).first; 
    cout << "gemSimHits region " << id.region()
	 << " ring " << id.ring()
	 << " station " << id.station()
	 << " layer " << id.layer()
	 << " chamber " << id.chamber()
	 << " roll " << id.roll()
	 << endl;
    int r = id.layer();
    if (id.station() == 2) r +=2;
    h_gemDigi->Fill(id.region()*r);
  }
  
  for (GEMRecHitCollection::const_iterator recHit = gemRecHits->begin(); recHit != gemRecHits->end(); ++recHit){
    GEMDetId id((*recHit).gemId());
    cout << "gemRecHits region " << id.region()
	 << " ring " << id.ring()
	 << " station " << id.station()
	 << " layer " << id.layer()
	 << " chamber " << id.chamber()
	 << " roll " << id.roll()
	 << endl;
    int r = id.layer();
    if (id.station() == 2) r +=2;
    h_gemRecHit->Fill(id.region()*r);
  }

    

	
  for (const reco::Track & m: *muons){
    if (fabs(m.eta()) < 1.5 ) continue;
    if (fabs(m.eta()) > 2.5 ) continue;
    m_ntracks++;
    cout << "m.pt "<< m.pt()
	 << " gemStationsWithAnyHits "<< m.hitPattern().gemStationsWithAnyHits()
	 << " cscStationsWithAnyHits "<< m.hitPattern().cscStationsWithAnyHits()
	 <<endl;
    bool hasGem = false;
    for (trackingRecHit_iterator hit = m.recHitsBegin(); hit != m.recHitsEnd(); ++hit) {
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
  h_gemRecHit=fs->make<TH1F>("gemRecHitStation","gemRecHitStation",9,-4.5,4.5);
  h_gemRecHit->GetXaxis()->SetTitle("station");
  h_gemRecHit->GetYaxis()->SetTitle("Counts");
  h_gemDigi=fs->make<TH1F>("gemDigiStation","gemDigiStation",9,-4.5,4.5);
  h_gemDigi->GetXaxis()->SetTitle("station");
  h_gemDigi->GetYaxis()->SetTitle("Counts");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
gemHLTAnalysis::endJob() 
{
  std::cout << " total tracks        : "<< m_ntracks << std::endl;
  std::cout << " total gem hits      : "<< m_ngemHits << std::endl;
  std::cout << " tracks with gem hits: "<< m_ntracksWithGem << std::endl;
	    
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

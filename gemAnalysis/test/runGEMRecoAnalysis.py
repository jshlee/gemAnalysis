
import FWCore.ParameterSet.Config as cms

process = cms.Process("GEMRECOANA")
detectorVer = 1
## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
if detectorVer == 0:
    process.load('Configuration.Geometry.GeometryExtended2019_cff')
    process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
if detectorVer == 1:
    process.load('Configuration.Geometry.GeometryExtended2023Muon4Eta_cff')
    process.load('Configuration.Geometry.GeometryExtended2023Muon4EtaReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

## global tag for 2019 upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
if detectorVer == 0:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')
if detectorVer == 1:
    process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

# the analyzer configuration
process.gemRecoAnalysis = cms.EDAnalyzer("gemRecoAnalysis",
    recoMuonInput = cms.InputTag("muons")
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring('file:h2mu_ggh_M125GeV_14TeV_2023WithGem_PU0.root')
)


process.p = cms.Path(process.gemRecoAnalysis)

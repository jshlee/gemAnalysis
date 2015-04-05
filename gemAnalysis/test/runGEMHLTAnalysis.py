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
process.gemHLTAnalysis = cms.EDAnalyzer("gemHLTAnalysis",
    recoMuonInput = cms.InputTag("hltL2Muons")
    #recoMuonInput = cms.InputTag("standAloneMuons")
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:singleMu2019WithGem_HLT.root')
    #fileNames = cms.untracked.vstring('file:singleMu2019_HLT.root')
)

process.p = cms.Path(process.gemHLTAnalysis)

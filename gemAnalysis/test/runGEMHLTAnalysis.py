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
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.source = cms.Source("PoolSource",
  #fileNames = cms.untracked.vstring('file:/pnfs/user/jlee/datagem/crab_singleMu_2023Muon_PU50/results/singleMu_2023Muon_PU50_89.root')
  #fileNames = cms.untracked.vstring('file:/pnfs/user/jlee/datagem/crab_singleMu_2019WithGem_PU50/results/singleMu_2019WithGem_PU50_74.root')
  #fileNames = cms.untracked.vstring('file:/pnfs/user/jlee/gem/src/singleMu2023_PU50_HLT.root')
  #fileNames = cms.untracked.vstring('file:/pnfs/user/jlee/gem/src/singleMu2019_PU50_WithGem_HLT.root')
  fileNames = cms.untracked.vstring('file:/pnfs/user/jlee/gem/src/singleMu2019_PU50_HLT.root')
)


process.p = cms.Path(process.gemHLTAnalysis)

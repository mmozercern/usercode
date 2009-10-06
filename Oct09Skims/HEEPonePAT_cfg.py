
import FWCore.ParameterSet.Config as cms
process = cms.Process("HEEPone")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkSummary = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(500),
    limit = cms.untracked.int32(10000000)
)
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(500),
    limit = cms.untracked.int32(10000000)
)


# Load geometry
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V3::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(    
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/9AB61A32-BA9A-DE11-8298-001D09F29619.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/80CF3175-C59A-DE11-AB6E-0016177CA778.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/7047D992-FC9A-DE11-863B-0019B9F72BAA.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/5CC96C97-B59A-DE11-8CAA-0019B9F6C674.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/126AA31F-BE9A-DE11-97E9-001D09F290BF.root'
    )
                            )

process.highetele = cms.EDFilter("GsfElectronSelector",
                                 src =cms.InputTag("gsfElectrons"),
                                 cut =cms.string("superCluster().get().energy()*sin(theta())> 30 &&  hadronicOverEm() < 0.05")
)
process.highetFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("highetele"),
                                    minNumber = cms.uint32(1),
                                    )

#no configuration of the pat is necessary for us at the moment
process.load("PhysicsTools.PatAlgos.patSequences_cff");

from PhysicsTools.PatAlgos.tools.coreTools import restrictInputToAOD
restrictInputToAOD(process, ['All'])

from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *
process.heepId = cms.EDProducer("HEEPAttStatusToPAT",
                                eleLabel = cms.InputTag("allLayer1Electrons"),
                                barrelCuts = cms.PSet(heepBarrelCuts),
                                endcapCuts = cms.PSet(heepEndcapCuts)
                                )


process.lowetpath = cms.Path(process.highetele*process.highetFilter * process.patDefaultSequence* process.heepId )

process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule("PoolOutputModule",
                               process.AODSIMEventContent,
                               fileName = cms.untracked.string('file:test.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('lowetpath'))
)
process.out.outputCommands.append('keep pat*_*_*_*')
process.out.outputCommands.append('keep *_heepId_*_*')


process.outpath = cms.EndPath(process.out)



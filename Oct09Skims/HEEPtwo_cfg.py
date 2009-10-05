
import FWCore.ParameterSet.Config as cms
process = cms.Process("cuttest")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.highetele = cms.EDFilter("GsfElectronSelector",
                                 src =cms.InputTag("gsfElectrons"),
                                 cut =cms.string("superCluster().get().energy()*sin(theta())> 30")
)
process.highetFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("highetele"),
                                    minNumber = cms.uint32(2),
                                    )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(    
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/9AB61A32-BA9A-DE11-8298-001D09F29619.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/80CF3175-C59A-DE11-AB6E-0016177CA778.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/7047D992-FC9A-DE11-863B-0019B9F72BAA.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/5CC96C97-B59A-DE11-8CAA-0019B9F6C674.root',
    '/store/relval/CMSSW_3_2_6/RelValZEE/GEN-SIM-RECO/STARTUP31X_V7-v1/0013/126AA31F-BE9A-DE11-97E9-001D09F290BF.root'
    )
                            )


process.lowetpath = cms.Path(process.highetele*process.highetFilter)


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule("PoolOutputModule",
                               process.FEVTSIMEventContent,
                               fileName = cms.untracked.string('file:test.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('lowetpath'))
)
process.out.outputCommands.append('drop *_*_*_*')
process.out.outputCommands.append('keep *_gsfElectrons_*_*')


process.outpath = cms.EndPath(process.out)

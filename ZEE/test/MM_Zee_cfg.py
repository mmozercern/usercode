import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)


# source
process.source = cms.Source("PoolSource", 
     fileNames = cms.untracked.vstring(
    'file:/data/mmozer/VB_PAT/CMSSW_3_6_0/src/ElectroWeakAnalysis/ZEE/test/D688E63D-B649-DF11-B26C-0026189438ED.root'    
    #'file:zee_Summer09-MC_31X_V3_AODSIM_v1_AODSIM.root'
    #'file:/tmp/rompotis/Run123505_LS70-80_BscMinBiasInnerThreshold.root',
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", fileName = cms.string("counters.root") )

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('PATLayer1_Output.fromAOD_full.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent ) 
                               )


## Load additional processes
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START36_V5::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


################################################################################################
###    P r e p a r a t i o n      o f    t h e    P A T    O b j e c t s   f r o m    A O D  ###
################################################################################################

## pat sequences to be loaded:
process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("ElectroWeakAnalysis.ZEE.VBSequence_cff")


from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run36xOn35xInput(process)
addJetCollection35X(process,cms.InputTag('ak7CaloJets'),
                 'AK7', 'Calo',
                 doJTA        = True,
                 doBTagging   = False,
                 jetCorrLabel = ('AK7', 'Calo'),
                 doType1MET   = True,
                 doL1Cleaning = True,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak7GenJets"),
                 doJetID      = True,
                 jetIdLabel   = "ak7"
                 )

addJetCollection35X(process,cms.InputTag('ak5CaloJets'),
                 'AK5', 'Calo',
                 doJTA        = True,
                 doBTagging   = False,
                 jetCorrLabel = ('AK5', 'Calo'),
                 doType1MET   = True,
                 doL1Cleaning = True,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True,
                 jetIdLabel   = "ak5"
                 )

switchJetCollection35X(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = None,
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
                 )


## from PhysicsTools.PatAlgos.tools.pfTools import *
## postfix = "PFlow"
## usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 


#disbale jet corrections:
#process.patJets.addJetCorrFactors = cms.bool(True)

process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.p = cms.Path( process.patDefaultSequence+process.VBSequence )
#process.p = cms.Path( getattr(process,"patPF2PATSequence"+postfix) +process.VBSequence)


process.zeeOutputModule = cms.OutputModule( "PoolOutputModule",
                                            fileName = cms.untracked.string("zeeCandidates.root"),
                                            outputCommands = cms.untracked.vstring('drop *',
                                                                                   'keep *_goodZToEEEdmNtuple_*_*',
                                                                                    ),
                                            SelectEvents = cms.vstring('p')
                                            )
process.outpath = cms.EndPath(process.zeeOutputModule)

#to do
# Mt cut
# ETmiss cut
# muon veto
# vertex requirement
# dummy MC copy for real data
# non-selective writing (with user-data)
# selective writing: NTuplizer + ouptumodule
# simultaneous W/Z analysis

import FWCore.ParameterSet.Config as cms
from VBcuts import cutList

tmp = cms.SequencePlaceholder("tmp")
VBSequence = cms.Sequence(tmp)

## tightIn = "patElectronsPFlow"
## looseIn = "patElectronsPFlow"
## looseInExclusive = ""
## jetsIn  = "patJetsPFlow"   # "ak5CaloJets"  
## genIn   = "genParticles"     ## tightIn = "cleanPatElectrons"
tightIn = "cleanPatElectrons"
looseIn = "cleanPatElectrons"
looseInExclusive = ""
jetsIn  = "patJetsAK5Calo"   # "ak5CaloJets"  
genIn   = "genParticles"
matchTo = "MC" ## options: RECO/MC
zIn = " "
if ( matchTo=="MC" ): print "Jet counts matched to MC electrons\n"
else:  print "Jet counts matched to reco electrons\n"



counterConfig = cms.EDAnalyzer('EffAggregator',
                              filters=cms.VPSet()
                              )

#######################################
# Loop over cuts to assemble sequence #
# first the actual selection          #
#######################################

for cutnum in range(len(cutList)):
    if  cutList[cutnum].type == "e" or cutList[cutnum].type == "e ex" or cutList[cutnum].type == "trigger"  : # add filters for electrons
        # filter tight collection
        modulename = cutList[cutnum].label+"TightSelector"
        globals()[modulename] = cms.EDFilter('PATElectronSelector')
        setattr(globals()[modulename],"src",cms.InputTag(tightIn) )
        setattr(globals()[modulename],"cut",cms.string(cutList[cutnum].tight) )
        VBSequence *= globals()[modulename]
        if cutList[cutnum].type == "e" or cutList[cutnum].type == "e ex": tightIn = modulename # only propagate per electron cuts
        # filter loose collection
        modulename = cutList[cutnum].label+"LooseSelector"
        globals()[modulename] = cms.EDFilter('PATElectronSelector')
        setattr(globals()[modulename],"src",cms.InputTag(looseIn) )
        setattr(globals()[modulename],"cut",cms.string(cutList[cutnum].loose) )
        VBSequence *= globals()[modulename]
        if cutList[cutnum].type == "e" : looseIn = modulename # only propagate per electron cuts
        # remove tight from loose collection for proper counting
        modulename = cutList[cutnum].label+"LooseExclusive"
        globals()[modulename] = cms.EDFilter('PATElectronCleaner')
        setattr(globals()[modulename],"src",cms.InputTag(cutList[cutnum].label+"LooseSelector"))
        setattr(globals()[modulename],"preselection",cms.string('') )
        setattr(globals()[modulename],"finalCut", cms.string('') )
        tmpclean = cms.PSet( electrons = cms.PSet(
            src = cms.InputTag(cutList[cutnum].label+"TightSelector"),
            algorithm = cms.string("bySuperClusterSeed"),
            requireNoOverlaps = cms.bool(True),
            ))
        setattr(globals()[modulename],"checkOverlaps",tmpclean )    
        VBSequence *= globals()[modulename]
        looseInExclusive = modulename
        if  cutList[cutnum].type == "e ex" : looseIn = modulename # propagate exclusive selection for Vecbos compatibility (Eid)
        # construct PSet for counter
        counterConfig.filters.append( cms.PSet(order = cms.double(cutList[cutnum].order),
                                               cutVal= cms.int32(cutList[cutnum].n),
                                               tightIn = cms.InputTag(cutList[cutnum].label+"TightSelector"),
                                               looseIn = cms.InputTag(looseInExclusive),
                                               type = cms.string(cutList[cutnum].type)
                                               )
                                      )

        
    if cutList[cutnum].type == "gen": # add filters on generator particles
        assert ("genIn" in globals().keys()), "gen level cut specified, but no generator colelction given"
        modulename = cutList[cutnum].label+"GenSelector"
        globals()[modulename] = cms.EDFilter('GenParticleSelector')
        setattr(globals()[modulename],"src",cms.InputTag(genIn) )
        setattr(globals()[modulename],"cut",cms.string(cutList[cutnum].tight) )
        VBSequence *= globals()[modulename]
        genIn = modulename
        # construct PSet for counter
        counterConfig.filters.append( cms.PSet(order = cms.double(cutList[cutnum].order),
                                               cutVal= cms.int32(cutList[cutnum].n),
                                               tightIn = cms.InputTag(genIn),
                                               looseIn = cms.InputTag(''),
                                               type = cms.string(cutList[cutnum].type)
                                               )
                                      )
        
       
    if cutList[cutnum].type == "jet": # add filters for jets
        modulename = cutList[cutnum].label+"JetSelector"
        globals()[modulename] = cms.EDFilter('PATJetSelector')
        setattr(globals()[modulename],"src",cms.InputTag(jetsIn) )
        setattr(globals()[modulename],"cut",cms.string(cutList[cutnum].tight) )
        VBSequence *= globals()[modulename]
        jetsIn = modulename
        # no cuts on N => no PSet for counter

    if cutList[cutnum].type == "mee": # add filter for Z
        # prepare merged electron collection
        modulename = cutList[cutnum].label+"mergedEleIn"
        globals()[modulename] = cms.EDProducer('CandViewMerger')
        setattr(globals()[modulename],'src',cms.VInputTag(tightIn,looseInExclusive))
        VBSequence *= globals()[modulename]
        mergedIn = modulename
        # combine the electrons
        modulename = cutList[cutnum].label+"Zcombiner" 
        globals()[modulename] = cms.EDProducer("CandViewShallowCloneCombiner")
        setattr(globals()[modulename],'decay',cms.string( mergedIn+" "+mergedIn))
        setattr(globals()[modulename],'cut',cms.string(cutList[cutnum].tight))
        setattr(globals()[modulename],'checkCharge',cms.bool(False))
        VBSequence *= globals()[modulename]
        zIn =  modulename
        # combine the electronsadd Chris Variable
        modulename = cutList[cutnum].label+"ZChris" 
        globals()[modulename] = cms.EDProducer("SinPhiMETMHT")
        setattr(globals()[modulename],'zIn',cms.InputTag( zIn ))
        setattr(globals()[modulename],'metIn',cms.InputTag( "patMETs"  ))
        VBSequence *= globals()[modulename]
        zIn =  modulename
        # construct PSet for counter
        counterConfig.filters.append( cms.PSet(order = cms.double(cutList[cutnum].order),
                                               cutVal= cms.int32(cutList[cutnum].n),
                                               tightIn = cms.InputTag(modulename),
                                               looseIn = cms.InputTag(''),
                                               type = cms.string(cutList[cutnum].type)
                                               )
                                      )




#prepare collection for jet counting: MC or RECO
modulename = "mergedEleIn"
globals()[modulename] = cms.EDProducer('CandViewMerger')
if (matchTo == "MC" ): setattr(globals()[modulename],'src',cms.VInputTag(genIn ))
else: setattr(globals()[modulename],'src',cms.VInputTag(tightIn,looseIn ))
VBSequence *= globals()[modulename] 
#clean the jet collection before counting
modulename = "JetCleanerForCounting"
globals()[modulename] = cms.EDFilter('PATJetCleaner')
setattr(globals()[modulename],'src',cms.InputTag(jetsIn))
setattr(globals()[modulename],'preselection',cms.string(''))
setattr(globals()[modulename],'finalCut',cms.string(''))
tmpclean = cms.PSet( electrons = cms.PSet(
    src       = cms.InputTag("mergedEleIn"),
    algorithm = cms.string("byDeltaR"),
    preselection        = cms.string(""),
    deltaR              = cms.double(0.3),
    checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
    pairCut             = cms.string(""),
    requireNoOverlaps   = cms.bool(True) # overlaps don't cause the jet to be discared
    ))
setattr(globals()[modulename],"checkOverlaps",tmpclean )
VBSequence *= globals()[modulename]
jetsIn=modulename
# finish counter configuration
counterConfig.jetsIn = cms.InputTag(jetsIn)
counterConfig.jetLabel = cms.string("recojets")
counterConfig.ncutsintree = cms.int32(18)  # 18=Z 20=W
VBSequence*=counterConfig
#cut before ntuple creation
ntupleFilter = cms.EDFilter("CandViewCountFilter",
                            src = cms.InputTag(zIn),
                            minNumber = cms.uint32(1)
                            )
VBSequence*=ntupleFilter
#produce ntuple
goodZToEEEdmNtuple = cms.EDProducer("CandViewNtpProducer",
                                    src = cms.InputTag( zIn ),
                                    lazyParser = cms.untracked.bool(True),
                                    prefix = cms.untracked.string("z"),
                                    eventInfo = cms.untracked.bool(True),
                                    variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("Mass"),
                                                                     quantity = cms.untracked.string("mass")
                                                                     ),
                                                           cms.PSet( tag = cms.untracked.string("Pt"),
                                                                     quantity = cms.untracked.string("pt")
                                                                     ),
                                                           cms.PSet( tag = cms.untracked.string("Eta"),
                                                                     quantity = cms.untracked.string("eta")
                                                                     ),
                                                           cms.PSet( tag = cms.untracked.string("Phi"),
                                                                     quantity = cms.untracked.string("phi")
                                                                     ), 
                                                           cms.PSet( tag = cms.untracked.string("e1Eta"),
                                                                     quantity = cms.untracked.string("daughter(0).eta")
                                                                     ),
                                                           cms.PSet( tag = cms.untracked.string("e1Phi"),
                                                                     quantity = cms.untracked.string("daughter(0).phi")
                                                                     ), 
                                                           cms.PSet( tag = cms.untracked.string("e2Eta"),
                                                                     quantity = cms.untracked.string("daughter(1).eta")
                                                                     ),
                                                           cms.PSet( tag = cms.untracked.string("e2Phi"),
                                                                     quantity = cms.untracked.string("daughter(1).phi")
                                                                     ), 
                                                           cms.PSet( tag = cms.untracked.string("sinPhiMETMHT"),
                                                                     quantity = cms.untracked.string("userFloat('sinPhiMETMHT')")
                                                                     ),                                                            
                                                           )  
                                    )
VBSequence*=goodZToEEEdmNtuple
        

VBSequence.remove(tmp)






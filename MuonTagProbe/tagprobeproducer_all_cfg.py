class dummy:
    pass

import FWCore.ParameterSet.Config as cms

process = cms.Process("tagprobe")

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    #wantSummary = cms.untracked.bool(True)
)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
## source
#process.source = cms.Source("PoolSource",
#     fileNames = cms.untracked.vstring(
#    'file:PF2PAT_MuonFilter_962_1_z3Z.root'
#    )
#)
process.load("PFAnalyses.WZJetsRatio.Sources.source_data_muonfilter_cff")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('test.root'),
                               # save only events passing the full path
                               #SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('keep *' )
                               )
process.TFileService = cms.Service("TFileService", fileName = cms.string("tptree.root") )


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR10_P_V10::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

###################################
# preselection                    #
###################################


#acceptance
process.EE = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("triggeredPatMuons"),
    cut = cms.string('abs(eta)<2.4 && pt>10')
)


#############################
# Will/Sara muon selection  #
#############################
varnames       = ["isGlobalMuon","globalTrack.normalizedChi2","globalTrack.hitPattern.numberOfValidTrackerHits","globalTrack.hitPattern.numberOfValidPixelHits","globalTrack.hitPattern.numberOfValidMuonHits","abs(userFloat('DxyVtx'))","isTrackerMuon","numberOfMatches","userFloat('RelR03Iso')"]
cutboundup     = [1             ,10.                         ,99999.                                           ,9999.                                          ,9999.                                         ,0.2                       ,1              ,100              ,0.15       ]
cutboundlow    = [1             ,-100                        ,10.5                                             ,0.5                                            ,0.5                                           ,-10                       ,1              ,1.5              ,-100       ]
bool           = [True          ,False                       ,False                                            ,False                                          ,False                                         ,False                     ,True           ,False            ,False      ]


#prepare tag sequence (doubles as accepted probes)

process.wp95EE = cms.Sequence(process.EE )
source = "EE"
for cutnum in range(len(varnames)):
    modulename = varnames[cutnum].translate(None,'.\')(')+"SelectorEE"
    if bool[cutnum] == True:
        cut = varnames[cutnum]
    else:
        cut = " " + str(cutboundlow[cutnum]) + " < " + varnames[cutnum] + " < " + str(cutboundup[cutnum])

    setattr(process,modulename,cms.EDFilter('PATMuonSelector',
                                            src=cms.InputTag(source),
                                            cut=cms.string(cut),
                                            lazyParsing = cms.untracked.bool(True))
            )
    process.wp95EE += getattr(process,modulename)
    source = modulename

process.wsmuons = cms.Sequence( process.wp95EE  )



######################################
# tag & probe production             #
######################################

#tags differ from passing probes by the trigger requirement
process.tags = cms.EDFilter('PATMuonSelector',
                            src=cms.InputTag(source),
                            cut=cms.string("!triggerObjectMatchesByPath('HLT_Mu9').empty || !triggerObjectMatchesByPath('HLT_Mu11').empty || !triggerObjectMatchesByPath('HLT_Mu13').empty || !triggerObjectMatchesByPath('HLT_Mu15').empty || !triggerObjectMatchesByPath('HLT_Mu13_v1').empty || !triggerObjectMatchesByPath('HLT_Mu15_v1').empty"),
                            lazyParsing = cms.untracked.bool(True)
                            )


                            
#t&p tool can't follow the selection chain => match to passing probes
process.passingprobessel = cms.EDProducer("PATMuonCleaner",
                               src = cms.InputTag("EE"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = cms.PSet(     muons = cms.PSet( src = cms.InputTag(source),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string(""),  # don't preselect the muons
                                                                               deltaR              = cms.double(0.01),
                                                                               #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                               checkRecoComponents = cms.bool(False),
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),                     
                                                             pfjets30 = cms.PSet( src = cms.InputTag("patJetsCachedReClusterZPUL1Corr"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30 && (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15 = cms.PSet( src = cms.InputTag("patJetsCachedReClusterZPUL1Corr"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15 && (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               )
                                                             ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)



# tp pairs for efficiency "any muon -> selected muon"
process.tagProbessel = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobessel"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreesel = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size")
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbessel"),
                                arbitration   = cms.string("OneProbe"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("muons")')),
                                isMC          = cms.bool(False)
)


process.tpseqsel = cms.Sequence(process.passingprobessel + process.tagProbessel  + process.tptreesel )


##################################
# generate/select probe tracks   #
##################################
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')
process.load('TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff')

process.patAODTrackCandsUnfiltered = cms.EDProducer("ConcreteChargedCandidateProducer",
    src          = cms.InputTag("highPtTrackProducer"),
    particleType = cms.string('mu+')   # to fix mass hypothesis
)

process.patAODTrackCands = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("patAODTrackCandsUnfiltered"),
    cut = cms.string('pt > 10 && abs(eta)<2.4')
)


# sequence to run on AOD before PAT cleaners
process.patAODTrackCandSequence = cms.Sequence(
        process.patAODTrackCandsUnfiltered *
        process.patAODTrackCands 
)

#make track into pat::genericCnadidate
from PhysicsTools.PatAlgos.producersLayer1.genericParticleProducer_cfi import patGenericParticles

process.allPatTracks = patGenericParticles.clone(
    src = cms.InputTag("patAODTrackCands"),
    # isolation configurables
    userIsolation = cms.PSet( ),
    isoDeposits = cms.PSet(  ),
    addGenMatch = cms.bool(False),
    genParticleMatch = cms.InputTag("trackMuMatch")
)

process.pfjetsNoTag  = cms.EDProducer("PATJetCleaner",
                                      src = cms.InputTag("patJetsCachedReClusterZPUL1Corr"),
                                      # preselection (any string-based cut for pat::Muon)
                                      preselection = cms.string(''),
                                      # overlap checking configurables
                                      checkOverlaps = cms.PSet(     muons = cms.PSet( src = cms.InputTag("tags"),
                                                                                                                algorithm = cms.string("byDeltaR"),
                                                                                      preselection        = cms.string(""),  # don't preselect the muons
                                                                                      deltaR              = cms.double(0.3),
                                                                                      checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                      pairCut             = cms.string(""),
                                                                                      requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                                                )
                                                                    ),
                                      # finalCut (any string-based cut for pat::Muon)
                                      finalCut = cms.string('!hasOverlaps("muons")'),
                                      )



#match to reco muons
process.passingprobestrack = cms.EDProducer("PATGenericParticleCleaner",
                               src = cms.InputTag("allPatTracks"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = cms.PSet(     muons = cms.PSet( src = cms.InputTag("EE"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string(""),  # don't preselect the muons
                                                                               deltaR              = cms.double(0.01),
                                                                               #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                               checkRecoComponents = cms.bool(False),
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               )
                                                             ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)




######################################
# tag & probe production track       #
######################################1
# tp pairs for efficiency "any track -> any muon"
process.tagProbestrack = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobestrack"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreetrack = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size")
                                                    ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestrack"),
                                arbitration   = cms.string("None"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("muons")')),
                                isMC          = cms.bool(False)
)
process.tpseqtrack = cms.Sequence( process.patAODTrackCandSequence + process.allPatTracks  + process.passingprobestrack+ process.tagProbestrack  + process.tptreetrack )




######################
# trigger            #
######################
process.passingprobestrigger = cms.EDProducer("PATMuonCleaner",
                               src = cms.InputTag(source), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = cms.PSet(    pfjets30 = cms.PSet( src = cms.InputTag("patJetsCachedReClusterZPUL1Corr"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15 = cms.PSet( src = cms.InputTag("patJetsCachedReClusterZPUL1Corr"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               )
                                                             ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)
# tp pairs for efficiency "any muon -> triggering muon"
process.tagProbestrigger = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobestrigger"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreetrigger = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size")
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestrigger"),
                                arbitration   = cms.string("OneProbe"),
                                flags = cms.PSet( passselection = cms.string("!triggerObjectMatchesByPath('HLT_Mu9').empty || !triggerObjectMatchesByPath('HLT_Mu11').empty || !triggerObjectMatchesByPath('HLT_Mu13').empty || !triggerObjectMatchesByPath('HLT_Mu15').empty || !triggerObjectMatchesByPath('HLT_Mu13_v1').empty || !triggerObjectMatchesByPath('HLT_Mu15_v1').empty")),
                                isMC          = cms.bool(False)
)

process.tpseqtrigger = cms.Sequence(process.passingprobestrigger+ process.tagProbestrigger  + process.tptreetrigger )


###############################
# selection + trigger         #
###############################
process.tptreeseltrig = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size")
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbessel"),
                                arbitration   = cms.string("OneProbe"),
                                flags = cms.PSet( passselection = cms.string("hasOverlaps('muons') && (!triggerObjectMatchesByPath('HLT_Mu9').empty || !triggerObjectMatchesByPath('HLT_Mu11').empty || !triggerObjectMatchesByPath('HLT_Mu13').empty || !triggerObjectMatchesByPath('HLT_Mu15').empty || !triggerObjectMatchesByPath('HLT_Mu13_v1').empty || !triggerObjectMatchesByPath('HLT_Mu15_v1').empty  )")),
                                isMC          = cms.bool(False)
)


###############################
# reco + selection            #
###############################

process.passingprobestracksel = cms.EDProducer("PATGenericParticleCleaner",
                               src = cms.InputTag("allPatTracks"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = cms.PSet(     muons = cms.PSet( src = cms.InputTag(source),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string(""),  # don't preselect the muons
                                                                               deltaR              = cms.double(0.01),
                                                                               #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                               checkRecoComponents = cms.bool(False),
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               )
                                                             ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)

process.tagProbestracksel = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobestracksel"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreetracksel = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size")
                                                    ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestracksel"),
                                arbitration   = cms.string("None"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("muons")')),
                                isMC          = cms.bool(False)
)


###############################
# all                         #
###############################
process.passingprobestrackseltrig = cms.EDProducer("PATGenericParticleCleaner",
                               src = cms.InputTag("allPatTracks"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = cms.PSet(     muons = cms.PSet( src = cms.InputTag("tags"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string(""),  # don't preselect the muons
                                                                               deltaR              = cms.double(0.01),
                                                                               #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                               checkRecoComponents = cms.bool(False),
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(999.),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets30Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>30&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               ),
                                                             pfjets15Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                                               algorithm = cms.string("byDeltaR"),
                                                                               preselection        = cms.string("abs(eta)<2.4 && pt>15&& (userInt('Number_1')+userInt('Number_2')+userInt('Number_3')+userInt('Number_4')+userInt('Number_5')+userInt('Number_6'))>1 && userInt('Number_1')>0 && userFloat('Fraction_1')>0 && userFloat('Fraction_5')<0.99 && userFloat('Fraction_4')<0.99 && userFloat('Fraction_2')<0.99 "),  # don't preselect the muons
                                                                               deltaR              = cms.double(.3),
                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                               pairCut             = cms.string(""),
                                                                               requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                               )
                                                             ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)

process.tagProbestrackseltrig = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobestrackseltrig"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreetrackseltrig = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size")
                                                    ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestrackseltrig"),
                                arbitration   = cms.string("None"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("muons")')),
                                isMC          = cms.bool(False)
)



process.producetpsel = cms.Path(process.wsmuons + process.tags  + process.tpseqsel)
process.producetptrack = cms.Path(process.wsmuons + process.tags +  process.pfjetsNoTag +process.tpseqtrack)
process.producetptrigger = cms.Path(process.wsmuons + process.tags + process.tpseqtrigger)
process.producecombinations = cms.Path(process.tptreeseltrig + process.passingprobestracksel + process.tagProbestracksel + process.tptreetracksel + process.passingprobestrackseltrig + process.tagProbestrackseltrig + process.tptreetrackseltrig)


#process.outp = cms.EndPath(process.out)

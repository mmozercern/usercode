class dummy:
    pass

import FWCore.ParameterSet.Config as cms

process = cms.Process("tagprobe")

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
# source
process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring(
    'file:../../double_eff/src/DEB0A5CB-DF01-E011-9B72-0018F3D096E0.root'
    )
)

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
process.GlobalTag.globaltag = cms.string('GR_R_39X_V5::All')#'GR10_P_V10::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

###################################
# preselection                    #
###################################

process.triggersel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag  = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths           = cms.vstring("HLT_Ele10_LW_L1R",
                                     "HLT_Ele15_SW_L1R",
                                     "HLT_Ele15_SW_CaloEleId_L1R",
                                     "HLT_Ele17_SW_CaloEleId_L1R",
                                     "HLT_Ele17_SW_TightEleId_L1R",
                                     "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2",
                                     "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3",
                                     ),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)

########################################
# pat sequence                         #
########################################

#pileup correction
process.load('RecoJets.JetProducers.ak5PFJets_cfi')
process.ak5PFJetsPU = process.ak5PFJets.clone()
process.ak5PFJetsPU.doAreaFastjet = True
process.ak5PFJetsPU.Rho_EtaMax = cms.double(4.5)
process.ak5PFJetsPU.Ghost_EtaMax = cms.double(6)

process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(4.5)
process.kt6PFJets.Ghost_EtaMax = cms.double(6)

process.load('JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff')
process.L1Fastjet.algorithm = cms.string('AK5Calo') #DUMMY needs to read an existing file
process.L1Fastjet.era = 'Spring10' #DUMMY needs to read an existing file
process.L1Fastjet.level = cms.string('L2Relative') #DUMMY needs to read an existing file
process.L1Fastjet.useCondDB = cms.untracked.bool(False)
process.L1Fastjet.srcMedianPt = cms.InputTag('kt6PFJets','rho') # 39X: 

process.ak5PFJetsL1.src = cms.InputTag("ak5PFJetsPU")

process.subtraction = cms.Sequence( process.ak5PFJetsPU * process.kt6PFJets * process.ak5PFJetsL1 )

process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
process.patJets.addTagInfos = False

removeMCMatching(process, ['All'])

from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(process,cms.InputTag('ak5PFJetsL1'),'PF','L1',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute','L2L3Residual'])),
                 doL1Cleaning = True,
                 doL1Counters = True,
                 doType1MET   = True,
                 doJetID      = True
                 )

from PhysicsTools.PatAlgos.tools.jetTools import switchJetCollection
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute','L2L3Residual'])),
                 doType1MET   = True,
                 doJetID      = True
                 )



from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )



process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")

process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
)

from PhysicsTools.PatAlgos.tools.trackTools import *
makeTrackCandidates(process,
    label        = 'patAODTrack',                  
    tracks       = cms.InputTag('generalTracks'), 
    particleType = 'e+',                         
    preselection = 'pt > 10',                     
    selection    = 'pt > 10',                     
    isolation    = {'tracker':0.3, 'ecalTowers':0.4, 'hcalTowers':0.4},                            
    isoDeposits  = [],                            
    #mcAs         = 'muons'           
)      

process.eletrigmatcher = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR"
                                         , src     = cms.InputTag( "EE" )
                                         , matched = cms.InputTag( "patTrigger" )
                                         , andOr          = cms.bool( False )
                                         , filterIdsEnum  = cms.vstring( '*' )
                                         , filterIds      = cms.vint32( 0 )
                                         , filterLabels   = cms.vstring( '*' )
                                         , pathNames      = cms.vstring("HLT_Ele10_LW_L1R",
                                                                        "HLT_Ele15_SW_L1R",
                                                                        "HLT_Ele15_SW_CaloEleId_L1R",
                                                                        "HLT_Ele17_SW_CaloEleId_L1R",
                                                                        "HLT_Ele17_SW_TightEleId_L1R",
                                                                        "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2",
                                                                        "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3",
                                                                        )
                                         # , pathLastFilterAcceptedOnly = cms.bool( True )
                                         , collectionTags = cms.vstring( '*' )
                                         , maxDPtRel = cms.double( 0.5 )
                                         , maxDeltaR = cms.double( 0.3 )
                                         , resolveAmbiguities    = cms.bool( True )
                                         , resolveByMatchQuality = cms.bool( False )
                                         )


process.cleanPatElectronsTriggerMatch = cms.EDProducer("PATTriggerMatchElectronEmbedder",
                                                       src     = cms.InputTag( "EE" ),
                                                       matches = cms.VInputTag( "eletrigmatcher" )
                                                       )

#acceptance
process.EE = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("cleanPatElectrons"),
    cut = cms.string('(abs(eta)<1.4442 || ( abs(eta)>1.566 && abs(eta)<2.5 ) )&& pt>10')
)


process.preparepat = cms.Sequence(process.triggersel + process.simpleEleIdSequence + process.subtraction +process.patDefaultSequence + process.EE + process.eletrigmatcher + process.cleanPatElectronsTriggerMatch )


## ######################################
## # tag & probe production             #
## ######################################

#tags differ from passing probes by the trigger requirement
process.tags = cms.EDFilter('PATElectronSelector',
                            src=cms.InputTag("cleanPatElectronsTriggerMatch"),
                            cut=cms.string("!triggerObjectMatches.empty && electronID('simpleEleId80relIso')==7"),
                            lazyParsing = cms.untracked.bool(True)
                            )

# clean jets 
process.pfjetsNoTag  = cms.EDProducer("PATJetCleaner",
                                      src = cms.InputTag("patJets"),
                                      # preselection (any string-based cut for pat::Muon)
                                      preselection = cms.string(''),
                                      # overlap checking configurables
                                      checkOverlaps = cms.PSet(     tageles = cms.PSet( src = cms.InputTag("tags"),
                                                                                                                algorithm = cms.string("byDeltaR"),
                                                                                      preselection        = cms.string(""),  # don't preselect the muons
                                                                                      deltaR              = cms.double(0.3),
                                                                                      checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                      pairCut             = cms.string(""),
                                                                                      requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                                                )
                                                                    ),
                                      # finalCut (any string-based cut for pat::Muon)
                                      finalCut = cms.string('!hasOverlaps("tageles") && abs(eta)<2.4 && (chargedHadronMultiplicity+neutralHadronMultiplicity+photonMultiplicity+electronMultiplicity+muonMultiplicity)>1 && neutralHadronEnergyFraction < 0.99 && photonEnergyFraction < 0.99  && chargedHadronEnergyFraction >0 && chargedHadronMultiplicity > 0 && chargedEmEnergyFraction < 0.99'),
                                      )

process.pfjetsPUNoTag  = cms.EDProducer("PATJetCleaner",
                                      src = cms.InputTag("patJetsPFL1"),
                                      # preselection (any string-based cut for pat::Muon)
                                      preselection = cms.string(''),
                                      # overlap checking configurables
                                      checkOverlaps = cms.PSet(     tageles = cms.PSet( src = cms.InputTag("tags"),
                                                                                                                algorithm = cms.string("byDeltaR"),
                                                                                      preselection        = cms.string(""),  # don't preselect the muons
                                                                                      deltaR              = cms.double(0.3),
                                                                                      checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                      pairCut             = cms.string(""),
                                                                                      requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                                                )
                                                                    ),
                                      # finalCut (any string-based cut for pat::Muon)
                                      finalCut = cms.string('!hasOverlaps("tageles")&& abs(eta)<2.4 && (chargedHadronMultiplicity+neutralHadronMultiplicity+photonMultiplicity+electronMultiplicity+muonMultiplicity)>1 && neutralHadronEnergyFraction < 0.99 && photonEnergyFraction < 0.99  && chargedHadronEnergyFraction >0 && chargedHadronMultiplicity > 0 && chargedEmEnergyFraction < 0.99'),
                                      )

jetOverlaps = cms.PSet(pfjets30 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                            algorithm = cms.string("byDeltaR"),
                                            preselection        = cms.string(" pt>30 "),  # don't preselect the muons
                                            deltaR              = cms.double(999.),
                                            checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                            pairCut             = cms.string(""),
                                                                                         requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                            ),
                       pfjets15 = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                            algorithm = cms.string("byDeltaR"),
                                            preselection        = cms.string("pt>15"),  # don't preselect the muons
                                            deltaR              = cms.double(999.),
                                            checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                            pairCut             = cms.string(""),
                                            requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                            ),
                       pfjets30Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                algorithm = cms.string("byDeltaR"),
                                                preselection        = cms.string("pt>30"),  # don't preselect the muons
                                                deltaR              = cms.double(.3),
                                                checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                pairCut             = cms.string(""),
                                                requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                ),
                       pfjets15Veto = cms.PSet( src = cms.InputTag("pfjetsNoTag"),
                                                algorithm = cms.string("byDeltaR"),
                                                preselection        = cms.string("pt>15"),  # don't preselect the muons
                                                deltaR              = cms.double(.3),
                                                checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                pairCut             = cms.string(""),
                                                requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                ),
                       pfjets30PU = cms.PSet( src = cms.InputTag("pfjetsPUNoTag"),
                                            algorithm = cms.string("byDeltaR"),
                                            preselection        = cms.string(" pt>30 "),  # don't preselect the muons
                                            deltaR              = cms.double(999.),
                                            checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                            pairCut             = cms.string(""),
                                            requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                            ),
                       pfjets15PU = cms.PSet( src = cms.InputTag("pfjetsPUNoTag"),
                                            algorithm = cms.string("byDeltaR"),
                                            preselection        = cms.string("pt>15"),  # don't preselect the muons
                                            deltaR              = cms.double(999.),
                                            checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                            pairCut             = cms.string(""),
                                            requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                            ),
                       pfjets30VetoPU = cms.PSet( src = cms.InputTag("pfjetsPUNoTag"),
                                                algorithm = cms.string("byDeltaR"),
                                                preselection        = cms.string("pt>30"),  # don't preselect the muons
                                                deltaR              = cms.double(.3),
                                                checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                pairCut             = cms.string(""),
                                                requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                ),
                       pfjets15VetoPU = cms.PSet( src = cms.InputTag("pfjetsPUNoTag"),
                                                algorithm = cms.string("byDeltaR"),
                                                preselection        = cms.string("pt>15"),  # don't preselect the muons
                                                deltaR              = cms.double(.3),
                                                checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                pairCut             = cms.string(""),
                                                requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                )
                       )

                       
#t&p tool can't follow the selection chain => match to passing probes
process.passingprobessel = cms.EDProducer("PATElectronCleaner",
                                          src = cms.InputTag("EE"), 
                                          # preselection (any string-based cut for pat::Muon)
                                          preselection = cms.string(''),
                                          # overlap checking configurables
                                          checkOverlaps = jetOverlaps,
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
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size"),
                                                     npf30PU = cms.string("overlaps('pfjets30PU').size - overlaps('pfjets30VetoPU').size"),
                                                     npf15PU = cms.string("overlaps('pfjets15PU').size - overlaps('pfjets15VetoPU').size"),
                                                     wp80 = cms.string("electronID('simpleEleId80relIso')"),
                                                     wp95 = cms.string("electronID('simpleEleId95relIso')") 
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbessel"),
                                arbitration   = cms.string("OneProbe"),
                                flags = cms.PSet( passWP80 = cms.string("electronID('simpleEleId80relIso')==7"),
                                                  passWP95 = cms.string("electronID('simpleEleId95relIso')==7") ),
                                isMC          = cms.bool(False)
)


process.tpseqsel = cms.Sequence(process.passingprobessel + process.tagProbessel  + process.tptreesel )


##################################
# generate/select probe tracks   #
##################################
#process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')
#process.load('TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff')


process.patAODTrackCands = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("patAODpatAODTrack"),
    cut = cms.string('abs(eta)<2.5')
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



#match to reco muons
process.passingprobestrack = cms.EDProducer("PATGenericParticleCleaner",
                                            src = cms.InputTag("allPatTracks"), 
                                            # preselection (any string-based cut for pat::Muon)
                                            preselection = cms.string(''),
                                            # overlap checking configurables
                                            checkOverlaps = jetOverlaps.clone(electrons = cms.PSet( src = cms.InputTag("EE"),
                                                                                                    algorithm = cms.string("byDeltaR"),
                                                                                                    preselection        = cms.string(""),  # don't preselect the muons
                                                                                                    deltaR              = cms.double(0.1),
                                                                                                    #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                                                    checkRecoComponents = cms.bool(False),
                                                                                                    pairCut             = cms.string(""),
                                                                                                    requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                                    ),
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
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size"),
                                                     npf30PU = cms.string("overlaps('pfjets30PU').size - overlaps('pfjets30VetoPU').size"),
                                                     npf15PU = cms.string("overlaps('pfjets15PU').size - overlaps('pfjets15VetoPU').size"),
                                                    ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestrack"),
                                arbitration   = cms.string("None"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("electrons")')),
                                isMC          = cms.bool(False)
)
process.tpseqtrack = cms.Sequence(process.tags+ process.patAODTrackCands + process.allPatTracks  + process.passingprobestrack+ process.tagProbestrack  + process.tptreetrack )


######################################
# tag & probe production SC          #
######################################1
process.superClusters = cms.EDProducer("SuperClusterMerger",
   src = cms.VInputTag(cms.InputTag("correctedHybridSuperClusters","", "RECO"),
                       cms.InputTag("correctedMulti5x5SuperClustersWithPreshower","", "RECO"))  
)

process.superClusterCands = cms.EDProducer("ConcreteEcalCandidateProducer",
   src = cms.InputTag("superClusters"),
   particleType = cms.int32(11),
)

#   Get the above SC's Candidates and place a cut on their Et and eta
process.goodSuperClusters = cms.EDFilter("CandViewSelector",
      src = cms.InputTag("superClusterCands"),
      cut = cms.string("et>10.0 && abs(eta)<2.5 && !(1.4442< abs(eta) <1.566)"),
      filter = cms.bool(True)
)                                         

process.allSC = patGenericParticles.clone(
    src = cms.InputTag("goodSuperClusters"),
    # isolation configurables
    userIsolation = cms.PSet( ),
    isoDeposits = cms.PSet(  ),
    addGenMatch = cms.bool(False),
    genParticleMatch = cms.InputTag("trackMuMatch")
)
                                        
#### remove real jets (with high hadronic energy fraction) from SC collection
##### this improves the purity of the probe sample without affecting efficiency

process.goodSuperClustersClean = cms.EDProducer("PATGenericParticleCleaner",
                                                src = cms.InputTag("allSC"),
                                                preselection = cms.string(''),
                                                checkOverlaps = cms.PSet( calojets = cms.PSet( src = cms.InputTag("cleanPatJets"),
                                                                                               algorithm = cms.string("byDeltaR"),
                                                                                               preselection        = cms.string('pt>5 &&  chargedHadronEnergyFraction > 0.15'),  # don't preselect the muons
                                                                                               deltaR              = cms.double(.1),
                                                                                               checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
                                                                                               pairCut             = cms.string(""),
                                                                                               requireNoOverlaps = cms.bool(True), # overlaps cause the electron to be discared
                                                                                               ),
                                                                          ),
                                                finalCut = cms.string(''),
                                                )


process.sc_sequence = cms.Sequence( process.superClusters *
                                    process.superClusterCands *
                                    process.goodSuperClusters *
                                    process.allSC *
                                    process.goodSuperClustersClean)

process.passingprobessc = cms.EDProducer("PATGenericParticleCleaner",
                               src = cms.InputTag("goodSuperClustersClean"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = jetOverlaps.clone(electrons = cms.PSet( src = cms.InputTag("EE"),
                                                                                       algorithm = cms.string("byDeltaR"),
                                                                                       preselection        = cms.string(""),  # don't preselect the muons
                                                                                       deltaR              = cms.double(0.1),
                                                                                       #checkRecoComponents = cms.bool(True), # don't check if they share some AOD object ref
                                                                                       checkRecoComponents = cms.bool(False),
                                                                                       pairCut             = cms.string(""),
                                                                                       requireNoOverlaps = cms.bool(False), # overlaps don't cause the electron to be discared
                                                                                       ),
                                                                 ),
                               # finalCut (any string-based cut for pat::Muon)
                               finalCut = cms.string(''),
)


process.tagProbessc = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("tags passingprobessc"),
                                   # charge coniugate states are implied;
                                   checkCharge = cms.bool(False),
                                   cut = cms.string("60 < mass < 120")
)

process.tptreesc = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                variables = cms.PSet(pt = cms.string("pt"),
                                                     abseta = cms.string("abs(eta)"),
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size"),
                                                     npf30PU = cms.string("overlaps('pfjets30PU').size - overlaps('pfjets30VetoPU').size"),
                                                     npf15PU = cms.string("overlaps('pfjets15PU').size - overlaps('pfjets15VetoPU').size"),
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbessc"),
                                arbitration   = cms.string("None"),
                                flags = cms.PSet( passselection = cms.string('hasOverlaps("electrons")')),
                                isMC          = cms.bool(False)
)

process.tpseqsc = cms.Sequence(process.sc_sequence  + process.passingprobessc+ process.tagProbessc  + process.tptreesc )

######################
# trigger            #
######################
process.wp80eles = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("cleanPatElectronsTriggerMatch"),
    cut = cms.string("electronID('simpleEleId80relIso')==7")
)


process.passingprobestrigger = cms.EDProducer("PATElectronCleaner",
                               src = cms.InputTag("wp80eles"), 
                               # preselection (any string-based cut for pat::Muon)
                               preselection = cms.string(''),
                               # overlap checking configurables
                               checkOverlaps = jetOverlaps,
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
                                                     npf30 = cms.string("overlaps('pfjets30').size - overlaps('pfjets30Veto').size"),
                                                     npf15 = cms.string("overlaps('pfjets15').size - overlaps('pfjets15Veto').size"),
                                                     npf30PU = cms.string("overlaps('pfjets30PU').size - overlaps('pfjets30VetoPU').size"),
                                                     npf15PU = cms.string("overlaps('pfjets15PU').size - overlaps('pfjets15VetoPU').size"),
                                                     ),
                                # choice of tag and probe pairs, and arbitration
                                tagProbePairs = cms.InputTag("tagProbestrigger"),
                                arbitration   = cms.string("OneProbe"),
                                flags = cms.PSet( passselection = cms.string("!triggerObjectMatches.empty")),
                                isMC          = cms.bool(False)
)

process.tpseqtrigger = cms.Sequence(process.passingprobestrigger+ process.tagProbestrigger  + process.tptreetrigger )



## process.producetpsel = cms.Path(process.wsmuons + process.tags  + process.tpseqsel)
## process.producetptrack = cms.Path(process.wsmuons + process.tags + process.tpseqtrack)

## process.producecombinations = cms.Path(process.tptreeseltrig + process.passingprobestracksel + process.tagProbestracksel + process.tptreetracksel + process.passingprobestrackseltrig + process.tagProbestrackseltrig + process.tptreetrackseltrig)

process.producetptrigger = cms.Path(process.preparepat + process.tags + process.pfjetsNoTag  + process.pfjetsPUNoTag + process.wp80eles + process.tpseqtrigger)
process.producetpsc = cms.Path(process.preparepat + process.tags + process.pfjetsNoTag  + process.pfjetsPUNoTag + process.tpseqsc)
process.producetpsel = cms.Path(process.preparepat + process.tags  + process.pfjetsNoTag  + process.pfjetsPUNoTag + process.tpseqsel)
process.producetptrack = cms.Path(process.preparepat + process.pfjetsNoTag  + process.pfjetsPUNoTag + process.tpseqtrack )

#process.outp = cms.EndPath(process.out)

import FWCore.ParameterSet.Config as cms

tongguangweights600 = cms.EDProducer("TongguangReader",
                                     src = cms.InputTag("genParticles") ,
                                     pdgId = cms.int32(25),
                                     filename = cms.FileInPath("MMozer/powhegweight/data/mZZ_Higgs600_8TeV_W.txt_I.txt")
                           )

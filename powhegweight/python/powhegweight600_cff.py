import FWCore.ParameterSet.Config as cms

powhegweights600 = cms.EDProducer("PowhegWeightProducer",
                                  src = cms.InputTag("genParticles") ,
                                  pdgId = cms.int32(25),
                                  mh = cms.double(600),
                                  gh = cms.double(123.),
                                  mt = cms.double(175.),
                                  BWflag=cms.int32(0)
                           )

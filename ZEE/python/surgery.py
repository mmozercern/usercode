from FWCore.ParameterSet.SequenceTypes import *
import FWCore.ParameterSet.Config as cms

class ExtendVisitor(object):
    """ extends a given sequence """
    def __init__(self,seq,toadd):
        self.seq = seq
        self.toadd = toadd
    def enter(self,visitee):
        pass
    def leave(self,visitee):
        if hasattr(visitee, "_Labelable__label"):
            if  visitee.label_()==self.seq :
                visitee += self.toadd
                

def insertEsel(process,inSeq,lastModule,postfix=""):
    print "inserting electron selection"
    print "inserting " +  inSeq + " up to " + lastModule

    # move pat electron prodction within PF2PAT sequence
    getattr(process,"patPF2PATSequence"+postfix).remove( getattr(process,"makePatElectrons"+postfix) )
    
    vis = ExtendVisitor("pfElectronSequence"+postfix,getattr(process,"makePatElectrons"+postfix))
    getattr(process,"patPF2PATSequence"+postfix).visit(vis)

    # insert selection modules into PF2PAT Sequence
    vis2 = ExtendVisitor("pfElectronSequence"+postfix, getattr(process,inSeq))
    getattr(process,"patPF2PATSequence"+postfix).visit(vis2)

    # adjust name for cleaning module
    getattr(process,"pfNoElectron"+postfix).topCollection = cms.InputTag(lastModule)
    getattr(process,"pfNoElectron"+postfix).__dict__['_TypedParameterizable__type'] = 'TPElectronOnPFCandidates'
    getattr(process,"pfNoElectron"+postfix).verbose = cms.untracked.bool( True )

    #print the sequence to see that this has actually worked
    l=[]
    vis2 = DecoratedNodeNameVisitor(l)
    getattr(process,"patPF2PATSequence"+postfix).visit(vis2)
    print l
    

    


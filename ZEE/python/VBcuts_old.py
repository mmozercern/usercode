from cutObject import cutObject

RawCutList=[
    cutObject(1  ,"gen"      ,"gen","abs(pdgId)==11 && abs(mother.pdgId)==23 ",""            ,2),
    #cutObject(2  ,"HLT"      ,"trigger","triggerObjectMatchesByPath('HLT_Ele15_SW_L1R').size > 0  ","triggerObjectMatchesByPath('HLT_Ele15_SW_L1R').size > 0  "            ,1),    
    cutObject(3  ,"reco"     ,"e","pt>0"        ,"pt>0"        ,2),
    cutObject(3.5,"Acceta"   ,"e","abs(eta)<2.5","abs(eta)<2.5",2),
    cutObject(4  ,"Accpt"    ,"e","pt>10"       ,"pt>10"       ,2),
    cutObject(4.5,"AccptAsym","e","pt>20"       ,"pt>10"       ,2),    
    cutObject(5  ,"EleId"    ,"e ex","( !isGap() && isEB() && sigmaIetaIeta()<0.01 && hcalOverEcal()<0.02 && abs(deltaEtaSuperClusterTrackAtVtx)<0.006 && abs(deltaPhiSuperClusterTrackAtVtx)<0.02 ) ||"+
              "(!isGap() && isEE() && sigmaIetaIeta()<0.03 && hcalOverEcal()<0.0025 && abs(deltaEtaSuperClusterTrackAtVtx())<0.003 && abs(deltaPhiSuperClusterTrackAtVtx())<0.02 )","electronID('eidRobustLoose')==3 || electronID('eidRobustLoose')==1",2),    
    cutObject(6  ,"trackIso" ,"e"," (dr04TkSumPt() < 2.5 && isEB()) ||  (dr04TkSumPt() < 0.8 && isEE()) ",
              "dr04TkSumPt()/pt() < 0.15"       ,2),
    cutObject(7  ,"ecalIso"  ,"e"," (dr04EcalRecHitSumEt() < 3.0 && isEB()) ||  (dr04EcalRecHitSumEt() < 2.5 && isEE()) ",
              "pt > 0"       ,2),
    cutObject(8  ,"hcalIso"  ,"e"," (dr04HcalTowerSumEt() < 5.0 && isEB()) ||  (dr04HcalTowerSumEt() < 0.25 && isEE()) ",
              "pt > 0"       ,2),
    #cutObject(11  ,"dz"      ,"e","vertexAssociation.dz < 1.2","vertexAssociation.dz < 1.2",2),
    
    #cutObject(12  ,"dr"      ,"e","vertexAssociation.dr < 1.2","vertexAssociation.dr < 1.2",2),    
    cutObject(15  ,"zmass"   ,"mee"," mass > 60. && mass < 110. ","",1),
    cutObject(22  ,"jetacc"  ,"jet","pt > 30. && abs(eta) < 3.","",0), # && energyFractionHadronic >= 0.","",0),
    
    ]

cutList = sorted(RawCutList,key=lambda cutObject: cutObject.order)


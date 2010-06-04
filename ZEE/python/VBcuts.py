
from cutObject import cutObject

RawCutList=[
    cutObject(1  ,"gen"      ,"gen"    ,"abs(pdgId)==11 && abs(mother.pdgId)==23 ",""            ,2),
    #cutObject(2  ,"HLT"      ,"trigger","triggerObjectMatchesByPath('HLT_Ele15_SW_L1R').size > 0  ","triggerObjectMatchesByPath('HLT_Ele15_SW_L1R').size > 0  "            ,1),    
    cutObject(2  ,"HLT"      ,"t ext","",""            ,1),    
    cutObject(3  ,"reco"     ,"e"      ,"pt>0"        ,"pt>0"        ,2),
    cutObject(4  ,"Acceta"   ,"e"      ,"abs(eta)<2.5 && (isEE || isEB ) && !isGap","abs(eta)<2.5&& (isEE || isEB ) && !isGap",2),
    cutObject(5  ,"Accpt"    ,"e"      ,"pt>10"       ,"pt>10"       ,2),
    cutObject(6  ,"AccptAsym","trigger","pt>20"       ,"pt>10"       ,2), # do not reuire high et e to be tight    
    cutObject(7  ,"EleId"    ,"e ex","( isEB && sigmaIetaIeta<0.01 && sigmaIetaIeta>0.002  && hcalOverEcal<0.05  && abs(deltaEtaSuperClusterTrackAtVtx)<0.006  )||"+
              "(isEE  && sigmaIetaIeta<0.03 && sigmaIetaIeta>0.002 && hcalOverEcal<0.04 && abs(deltaEtaSuperClusterTrackAtVtx)<0.008  )",
              "(isEB  && sigmaIetaIeta<0.01 && sigmaIetaIeta>0.002 && hcalOverEcal<0.05   && abs(deltaEtaSuperClusterTrackAtVtx)<0.006   ) ||"+
              "(isEE  && sigmaIetaIeta<0.03 && sigmaIetaIeta>0.002 && hcalOverEcal<0.04   && abs(deltaEtaSuperClusterTrackAtVtx)<0.008   )",2),
    cutObject(8  ,"trackIso" ,"e"," (dr04TkSumPt < 7.0 && isEB)         ||  (dr04TkSumPt < 8.0 && isEE) ",
              "                     (dr04TkSumPt < 7.0 && isEB)         ||  (dr04TkSumPt < 8.0 && isEE)"       ,2),
    cutObject(9  ,"ecalIso"  ,"e"," (dr04EcalRecHitSumEt < 5.0 && isEB) ||  (dr04EcalRecHitSumEt < 3.0 && isEE) ",
              "                     (dr04EcalRecHitSumEt < 5.0 && isEB) ||  (dr04EcalRecHitSumEt < 3.0 && isEE) "       ,2),
    cutObject(10 ,"hcalIso"  ,"e"," (dr04HcalTowerSumEt < 5.0 && isEB)  ||  (dr04HcalTowerSumEt < 2.0 && isEE) ",
              "                     (dr04HcalTowerSumEt < 5.0 && isEB)  ||  (dr04HcalTowerSumEt < 2.0  && isEE)"       ,2),
    cutObject(11 ,"conv"     ,"e"," gsfTrack.trackerExpectedHitsInner.numberOfHits<=1","gsfTrack.trackerExpectedHitsInner.numberOfHits<=1"       ,2), # do not reuire high et e to be tight    
    cutObject(12 ,"separator","separator","",""       ,2), # only the above are executed before the jet clustering    
    cutObject(15 ,"zmass"   ,"mee"," mass > 60. && mass < 110. ","",1),
    cutObject(22 ,"jetacc"  ,"jet","pt > 15. && abs(eta) < 2.5","",0), # && energyFractionHadronic >= 0.","",0),
   
    ]

cutList = sorted(RawCutList,key=lambda cutObject: cutObject.order)


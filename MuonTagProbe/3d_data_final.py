import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


##############################################################
# reco efficiency. No njet binning, include all phase-space  #
##############################################################
process.TagProbeFitTreeAnalyzerTrack = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                      # IO parameters:
                                                      InputFileNames = cms.vstring("tptree_dataFinal.root"),
                                                      InputDirectoryName = cms.string("tptreetrack"),
                                                      InputTreeName = cms.string("fitter_tree"),
                                                      OutputFileName = cms.string("3deff_reco_data_final.root"),
                                                      #numbrer of CPUs to use for fitting
                                                      NumCPU = cms.uint32(2),
                                                      # specifies wether to save the RooWorkspace containing the data for each bin and
                                                      # the pdf object with the initial and final state snapshots
                                                      SaveWorkspace = cms.bool(True),
                                                      floatShapeParameters = cms.bool(True),
                                                      fixVars = cms.vstring(""),
                                                      
                                                      # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                                      Variables = cms.PSet(mass = cms.vstring("Tag-Probe Mass", "70.0", "120.0", "GeV/c^{2}"),
                                                                           pt = cms.vstring("Probe p_{T}", "0", "100", "GeV/c"),
                                                                           abseta = cms.vstring("Probe #eta", "0", "2.4", ""),
                                                                           npf15 = cms.vstring("N_{PFjet} (15)", "-0.5", "10.5", ""),
                                                                           npf30 = cms.vstring("N_{PFjet} (30)", "-0.5", "10.5", "")
                                                                           ),
                                                      # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
                                                      Categories = cms.PSet(passselection = cms.vstring("passselection", "dummy[pass=1,fail=0]") ),
                                                      # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
                                                      # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
                                                      PDFs = cms.PSet(gaussPlusLinear = cms.vstring("RooVoigtian::signal(mass, mean[91.2, 80.0, 100.0],gamma[10,0,100], sigma[2.3, 0.5, 10.0])",
                                                                                                    "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                    "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                    ##             "RooPolynomial::backgroundPass(mass, {cPass1[0,-100,100],cPass2[0,-100,100]},2)",
                                                                                                    ##             "RooPolynomial::backgroundFail(mass, {cFail1[0,-100,100],cFail2[0,-100,100]},2)",
                                                                                                    "efficiency[0.5,0,1]",
                                                                                                    "signalFractionInPassing[0.9]"
                                                                                                    ),
                                                                      ),

                                                                              #MAIN RESULT pt vs eta integraetd over njet
                                                      Efficiencies = cms.PSet(pt_eta = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                                UnbinnedVariables = cms.vstring("mass"),
                                                                                                BinnedVariables = cms.PSet(pt = cms.vdouble(10,20,25,30,35,40,50,100),
                                                                                                                           abseta = cms.vdouble(0,0.8,1.7,2.1,2.4),
                                                                                                                           ),
                                                                                                BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                ),
                                                                                                                                                        
                                                                              #control results: 1d distributions
                                                                              npf30 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf30 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                              npf15 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf15 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                              pt = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                             UnbinnedVariables = cms.vstring("mass"),
                                                                                             #specifies the binning of parameters
                                                                                             BinnedVariables = cms.PSet(pt = cms.vdouble(10,  20, 25, 30, 35, 40, 45, 50, 55, 60, 75 , 100)
                                                                                                                        ),
                                                                                             #first string is the default followed by binRegExp - PDFname pairs
                                                                                             BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                             ),
                                                                              eta = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                              UnbinnedVariables = cms.vstring("mass"),
                                                                                              BinnedVariables = cms.PSet( abseta = cms.vdouble(0,0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4,1.5, 1.6,1.7,1.8,2.0,2.2,2.4)
                                                                                                                          ),
                                                                                              BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                              ),        
                                                                              )
                                                      )

#########################################################################
# trigger efficiency. No njet binning, include only leading kinematics  #
#########################################################################
process.TagProbeFitTreeAnalyzerTrigger = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                      # IO parameters:
                                                      InputFileNames = cms.vstring("tptree_dataFinal.root"),
                                                      InputDirectoryName = cms.string("tptreetrigger"),
                                                      InputTreeName = cms.string("fitter_tree"),
                                                      OutputFileName = cms.string("3deff_trigger_data_final.root"),
                                                      #numbrer of CPUs to use for fitting
                                                      NumCPU = cms.uint32(2),
                                                      # specifies wether to save the RooWorkspace containing the data for each bin and
                                                      # the pdf object with the initial and final state snapshots
                                                      SaveWorkspace = cms.bool(True),
                                                      floatShapeParameters = cms.bool(True),
                                                      fixVars = cms.vstring(""),
                                                      
                                                      # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                                      Variables = cms.PSet(mass = cms.vstring("Tag-Probe Mass", "70.0", "120.0", "GeV/c^{2}"),
                                                                           pt = cms.vstring("Probe p_{T}", "20", "100", "GeV/c"),
                                                                           abseta = cms.vstring("Probe #eta", "0", "2.1", ""),
                                                                           npf15 = cms.vstring("N_{PFjet} (15)", "-0.5", "10.5", ""),
                                                                           npf30 = cms.vstring("N_{PFjet} (30)", "-0.5", "10.5", "")
                                                                           ),
                                                      # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
                                                      Categories = cms.PSet(passselection = cms.vstring("passselection", "dummy[pass=1,fail=0]") ),
                                                      # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
                                                      # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
                                                      PDFs = cms.PSet(gaussPlusLinear = cms.vstring("RooVoigtian::signal(mass, mean[91.2, 80.0, 100.0],gamma[10,0,100], sigma[2.3, 0.5, 10.0])",
                                                                                                    "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                    "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                    ##             "RooPolynomial::backgroundPass(mass, {cPass1[0,-100,100],cPass2[0,-100,100]},2)",
                                                                                                    ##             "RooPolynomial::backgroundFail(mass, {cFail1[0,-100,100],cFail2[0,-100,100]},2)",
                                                                                                    "efficiency[0.5,0,1]",
                                                                                                    "signalFractionInPassing[0.9]"
                                                                                                    ),
                                                                      ),

                                                                              #MAIN RESULT pt vs eta integraetd over njet
                                                      Efficiencies = cms.PSet(pt_eta = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                                UnbinnedVariables = cms.vstring("mass"),
                                                                                                BinnedVariables = cms.PSet(pt = cms.vdouble(20,25,30,35,40,50,100),
                                                                                                                           abseta = cms.vdouble(0,0.8,1.2,1.7,2.1),
                                                                                                                           ),
                                                                                                BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                ),
                                                                                                                                                        
                                                                              #control results: 1d distributions
                                                                              npf30 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf30 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                              npf15 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf15 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                              pt = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                             UnbinnedVariables = cms.vstring("mass"),
                                                                                             #specifies the binning of parameters
                                                                                             BinnedVariables = cms.PSet(pt = cms.vdouble(20, 25, 30, 35, 40, 45, 50, 55, 60, 75 , 100)
                                                                                                                        ),
                                                                                             #first string is the default followed by binRegExp - PDFname pairs
                                                                                             BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                             ),
                                                                              eta = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                              UnbinnedVariables = cms.vstring("mass"),
                                                                                              BinnedVariables = cms.PSet( abseta = cms.vdouble(0,0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4,1.5, 1.6,1.7,1.8,2.0,2.1)
                                                                                                                          ),
                                                                                              BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                              ),        
                                                                              )
                                                      )


#########################################################################
# selection efficiency.  njet binning, include only leading kinematics  #
#########################################################################
process.TagProbeFitTreeAnalyzerSel = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                      # IO parameters:
                                                      InputFileNames = cms.vstring("tptree_dataFinal.root"),
                                                      InputDirectoryName = cms.string("tptreesel"),
                                                      InputTreeName = cms.string("fitter_tree"),
                                                      OutputFileName = cms.string("3deff_sel_data_final.root"),
                                                      #numbrer of CPUs to use for fitting
                                                      NumCPU = cms.uint32(2),
                                                      # specifies wether to save the RooWorkspace containing the data for each bin and
                                                      # the pdf object with the initial and final state snapshots
                                                      SaveWorkspace = cms.bool(True),
                                                      floatShapeParameters = cms.bool(True),
                                                      fixVars = cms.vstring(""),
                                                      
                                                      # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                                      Variables = cms.PSet(mass = cms.vstring("Tag-Probe Mass", "70.0", "120.0", "GeV/c^{2}"),
                                                                           pt = cms.vstring("Probe p_{T}", "20", "100", "GeV/c"),
                                                                           abseta = cms.vstring("Probe #eta", "0", "2.1", ""),
                                                                           npf15 = cms.vstring("N_{PFjet} (15)", "-0.5", "10.5", ""),
                                                                           npf30 = cms.vstring("N_{PFjet} (30)", "-0.5", "10.5", "")
                                                                           ),
                                                      # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
                                                      Categories = cms.PSet(passselection = cms.vstring("passselection", "dummy[pass=1,fail=0]") ),
                                                      # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
                                                      # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
                                                      PDFs = cms.PSet(gaussPlusLinear = cms.vstring("RooVoigtian::signal(mass, mean[91.2, 80.0, 100.0],gamma[10,0,100], sigma[2.3, 0.5, 10.0])",
                                                                                                    "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                    "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                    ##             "RooPolynomial::backgroundPass(mass, {cPass1[0,-100,100],cPass2[0,-100,100]},2)",
                                                                                                    ##             "RooPolynomial::backgroundFail(mass, {cFail1[0,-100,100],cFail2[0,-100,100]},2)",
                                                                                                    "efficiency[0.5,0,1]",
                                                                                                    "signalFractionInPassing[0.9]"
                                                                                                    ),
                                                                      ),

                                                                              #MAIN RESULT pt vs eta for njet=0/1 and njet distributions
                                                      Efficiencies = cms.PSet(pt_eta_npf15 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                                UnbinnedVariables = cms.vstring("mass"),
                                                                                                BinnedVariables = cms.PSet(pt = cms.vdouble(20,25,30,35,40,50,100),
                                                                                                                           abseta = cms.vdouble(0,0.8,1.2,1.7,2.1),
                                                                                                                           npf15 = cms.vdouble(-0.5,0.5,1.5),
                                                                                                                           ),
                                                                                                BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                ),
                                                                                                                                                        
                                                                              pt_eta_npf30 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                                UnbinnedVariables = cms.vstring("mass"),
                                                                                                BinnedVariables = cms.PSet(pt = cms.vdouble(20,25,30,35,40,50,100),
                                                                                                                           abseta = cms.vdouble(0,0.8,1.2,1.7,2.1),
                                                                                                                           npf30 = cms.vdouble(-0.5,0.5,1.5),
                                                                                                                           ),
                                                                                                BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                ),
                                                                                                                                                        
                                                                              npf30 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf30 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                              npf15 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                               UnbinnedVariables = cms.vstring("mass"),
                                                                                               BinnedVariables = cms.PSet(npf15 = cms.vdouble(-0.5,0.5,1.5,2.5,3.5,4.5),
                                                                                                                          ),
                                                                                               BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                               ),
                                                                               #control results: 1d distributions
                                                                             pt = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                             UnbinnedVariables = cms.vstring("mass"),
                                                                                             #specifies the binning of parameters
                                                                                             BinnedVariables = cms.PSet(pt = cms.vdouble(20, 25, 30, 35, 40, 45, 50, 55, 60, 75 , 100)
                                                                                                                        ),
                                                                                             #first string is the default followed by binRegExp - PDFname pairs
                                                                                             BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                             ),
                                                                              eta = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                              UnbinnedVariables = cms.vstring("mass"),
                                                                                              BinnedVariables = cms.PSet( abseta = cms.vdouble(0,0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4,1.5, 1.6,1.7,1.8,2.0,2.1)
                                                                                                                          ),
                                                                                              BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                              ),        
                                                                              )
                                                      )




process.fit = cms.Path( process.TagProbeFitTreeAnalyzerTrack + process.TagProbeFitTreeAnalyzerTrigger + process.TagProbeFitTreeAnalyzerSel)

import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.TagProbeFitTreeAnalyzerSC = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                   # IO parameters:
                                                   InputFileNames = cms.vstring("tptreeX.root"),
                                                   InputDirectoryName = cms.string("tptreesc"),
                                                   InputTreeName = cms.string("fitter_tree"),
                                                   OutputFileName = cms.string("3deff_final_sc.root"),
                                                   #numbrer of CPUs to use for fitting
                                                   NumCPU = cms.uint32(2),
                                                   # specifies wether to save the RooWorkspace containing the data for each bin and
                                                   # the pdf object with the initial and final state snapshots
                                                   SaveWorkspace = cms.bool(True),
                                                   floatShapeParameters = cms.bool(True),
                                                   fixVars = cms.vstring(""),
                                                   
                                                   # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
                                                   Variables = cms.PSet( mass = cms.vstring("Tag-Probe Mass", "70.0", "120.0", "GeV/c^{2}"),
                                                                         pt = cms.vstring("Probe p_{T}", "10", "100", "GeV/c"),
                                                                         abseta = cms.vstring("Probe #eta", "0", "2.5", ""),
                                                                         npf15 = cms.vstring("N_{PFjet} (15)", "-0.5", "10.5", ""),
                                                                         npf30 = cms.vstring("N_{PFjet} (30)", "-0.5", "10.5", "")
                                                                         ),
                                                   # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
                                                   Categories = cms.PSet( passselection = cms.vstring("passselection", "dummy[pass=1,fail=0]")
                                                                          ),
                                                   # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
                                                   # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
                                                   PDFs = cms.PSet( gaussPlusLinear = cms.vstring("EXPR::signal('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',mean[91.2, 80.0, 100.0],mass, sigmaL[2.3, 0.5, 10.0],alphaL[0.23],sigmaR[2.3, 0.5, 10.0],alphaR[0.2,0,1])",
                                                                                                  "RooExponential::backgroundPass(mass, cPass[0,-1,1])",
                                                                                                  "RooExponential::backgroundFail(mass, cFail[0,-1,1])",
                                                                                                  ##             "RooPolynomial::backgroundPass(mass, {cPass1[0,-100,100],cPass2[0,-100,100]},2)",
                                                                                                  ##             "RooPolynomial::backgroundFail(mass, {cFail1[0,-100,100],cFail2[0,-100,100]},2)",
                                                                                                  "efficiency[0.5,0,1]",
                                                                                                  "signalFractionInPassing[0.9]"
                                                                                                  ),
                                                                    
                                                                    gaussPlusLinearFix = cms.vstring("EXPR::signal('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',mean[91.2, 80.0, 100.0],mass, sigmaL[4.2],alphaL[0.23],sigmaR[3.2],alphaR[0.20])",
                                                                                                     "RooExponential::backgroundPass(mass, cPass[0,-1,1])",
                                                                                                     "RooExponential::backgroundFail(mass, cFail[0,-1,1])",
                                                                                                     ##             "RooPolynomial::backgroundPass(mass, {cPass1[0,-100,100],cPass2[0,-100,100]},2)",
                                                                                                     ##             "RooPolynomial::backgroundFail(mass, {cFail1[0,-100,100],cFail2[0,-100,100]},2)",
                                                                                                     "efficiency[0.5,0,1]",
                                                                                                     "signalFractionInPassing[0.9]"
                                                                                                     ),
                                                                    ),


                                                   # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
                                                   # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
                                                   #main result: 2d eff vs pt/eta
                                                   Efficiencies = cms.PSet( pt_eta = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                         UnbinnedVariables = cms.vstring("mass"),
                                                                                         BinnedVariables = cms.PSet(pt = cms.vdouble(10,20,30,40,50,100),
                                                                                                                    abseta = cms.vdouble(0,0.5,1.0,1.4442,1.566,2.0,2.5)
                                                                                                                    ),
                                                                                         BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                         ),
                                                                            pt_eta_coarse = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                         UnbinnedVariables = cms.vstring("mass"),
                                                                                         BinnedVariables = cms.PSet(pt = cms.vdouble(10,20),
                                                                                                                    abseta = cms.vdouble(0,1.4442,1.566,2.5)
                                                                                                                    ),
                                                                                         BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                         ),
                                                                            
                                                                            #control distributions
                                                                            pt = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                          UnbinnedVariables = cms.vstring("mass"),
                                                                                          BinnedVariables = cms.PSet(pt = cms.vdouble(10,20,30,40,50,100),
                                                                                                                     ),
                                                                                          BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                          ),
                                                                            
                                                                            npf30float = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                             UnbinnedVariables = cms.vstring("mass"),
                                                                                             BinnedVariables = cms.PSet( npf30 = cms.vdouble(-0.5,0.5,1.5,2.5),
                                                                                                                         ),
                                                                                             BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                             ),
                                                                            npf30fix = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                             UnbinnedVariables = cms.vstring("mass"),
                                                                                             BinnedVariables = cms.PSet( npf30 = cms.vdouble(2.5,3.5,4.5),
                                                                                                                         ),
                                                                                             BinToPDFmap = cms.vstring("gaussPlusLinearFix")
                                                                                             ),
                                                                            
                                                                            eta = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passselection","pass"),
                                                                                            UnbinnedVariables = cms.vstring("mass"),
                                                                                            BinnedVariables = cms.PSet(abseta = cms.vdouble(0,0.4,0.8,1.2,1.4442,1.566,1.9,2.3,2.5)
                                                                                                                       ),
                                                                                            BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                            ),        
                                                                            )
                                                   )

process.fit = cms.Path( process.TagProbeFitTreeAnalyzerSC)

import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.TagProbeFitTreeAnalyzerSel = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                                                   # IO parameters:
                                                   InputFileNames = cms.vstring("tptree_MC.root"),
                                                   InputDirectoryName = cms.string("tptreesel"),
                                                   InputTreeName = cms.string("fitter_tree"),
                                                   OutputFileName = cms.string("3deff_MC_sel.root"),
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
                                                   Categories = cms.PSet( passWP80 = cms.vstring("passWP80", "dummy[pass=1,fail=0]"),
                                                                          passWP95 = cms.vstring("passWP95", "dummy2[pass=1,fail=0]"),
                                                                          ),
                                                   # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
                                                   # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
                                                   PDFs = cms.PSet( gaussPlusLinear = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[91.2, 80.0, 100.0],mass, sigmaLP[2.3, 0.5, 10.0],alphaLP[0.23],sigmaRP[2.3, 0.5, 10.0],alphaRP[0.2])",
                                                                                                  "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[2.3, 0.5, 10.0],alphaLF[0.23],sigmaRF[2.3, 0.5, 10.0],alphaRF[0.2,0,1])", 
                                                                                                  "RooExponential::backgroundPass(mass, cPass[-0.1,-1,0])",
                                                                                                  "RooExponential::backgroundFail(mass, cFail[-0.1,-1,0])",
                                                                                                  "SUM::backgroundFail(frac[0.5,0,1]*backgroundFail1 , backgroundFail2)",
                                                                                                  "efficiency[0.5,0,1]",
                                                                                                  "signalFractionInPassing[0.9]"
                                                                                                  ),
                                                                    
                                                                    gaussPlusLinear1Fix80 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[89.3],mass, sigmaLP[7],alphaLP[0.23],sigmaRP[4.2],alphaRP[0.2])",
                                                                                                        "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[5.1],alphaLF[0.23],sigmaRF[2.4],alphaRF[0.207])", 
                                                                                                        "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                        "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                        "efficiency[0.5,0,1]",
                                                                                                        "signalFractionInPassing[0.9]"
                                                                                                        ),
                                                                    gaussPlusLinear1SemiFix80 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[89.3,80,100],mass, sigmaLP[7],alphaLP[0.23],sigmaRP[4.2],alphaRP[0.2])",
                                                                                                            "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[2.3, 0.5, 10.0],alphaLF[0.23],sigmaRF[2.3, 0.5, 10.0],alphaRF[0.2,0,1])", 
                                                                                                            "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                            "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                            "efficiency[0.5,0,1]",
                                                                                                            "signalFractionInPassing[0.9]"
                                                                                                            ),
                                                                    gaussPlusLinear0SemiFix80 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[89.7, 80.0, 100.0],mass, sigmaLP[7.69,1,10],alphaLP[0.23],sigmaRP[3.57,1,10],alphaRP[0.2])",
                                                                                                            "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[2.3, 0.5, 10.0],alphaLF[0.23],sigmaRF[2.3, 0.5, 10.0],alphaRF[0.2,0,1])", 
                                                                                                            "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                            "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                            "efficiency[0.5,0,1]",
                                                                                                            "signalFractionInPassing[0.9]"
                                                                                                            ),
                                                                    gaussPlusLinear1Fix95 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[88.3],mass, sigmaLP[7.1],alphaLP[0.23],sigmaRP[4.1],alphaRP[0.2])",
                                                                                                        "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[5.52],alphaLF[0.23],sigmaRF[2.63],alphaRF[0.207])",
                                                                                                        "RooExponential::backgroundPass(mass, cPass[0,-10,10])",
                                                                                                        "RooExponential::backgroundFail(mass, cFail[0,-10,10])",
                                                                                                        "efficiency[0.5,0,1]",
                                                                                                        "signalFractionInPassing[0.9]"
                                                                                                        ),
                                                                    gaussPlusLinear1SemiFix95 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[88.3,80,100],mass, sigmaLP[7.1],alphaLP[0.23],sigmaRP[4.1],alphaRP[0.2])",
                                                                                                            "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[2.3, 0.5, 10.0],alphaLF[0.23],sigmaRF[2.3, 0.5, 10.0],alphaRF[0.2,0,1])", 
                                                                                                            "RooExponential::backgroundPass(mass, cPass[0,-10,0])",
                                                                                                            "RooExponential::backgroundFail(mass, cFail[0,-10,0])",
                                                                                                            "efficiency[0.5,0,1]",
                                                                                                            "signalFractionInPassing[0.9]"
                                                                                                            ),
                                                                    gaussPlusLinear0SemiFix95 = cms.vstring("EXPR::signalPass('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanP[90,80,100],mass, sigmaLP[9.0],alphaLP[0.23],sigmaRP[3.3],alphaRP[0.2])",
                                                                                                            "EXPR::signalFail('(@1<@0)*exp(-(@0-@1)*(@0-@1)/(@2*@2 + @3*(@0-@1)*(@0-@1))) + (@1>=@0)*exp(-(@0-@1)*(@0-@1)/(@4*@4 + @5*(@0-@1)*(@0-@1)))',meanF[91.2, 80.0, 100.0],mass, sigmaLF[2.3, 0.5, 10.0],alphaLF[0.23],sigmaRF[2.3, 0.5, 10.0],alphaRF[0.2,0,1])", 
                                                                                                            "RooExponential::backgroundPass(mass, cPass[0,-10,0])",
                                                                                                            "RooExponential::backgroundFail(mass, cFail[0,-10,0])",
                                                                                                            "efficiency[0.5,0,1]",
                                                                                                            "signalFractionInPassing[0.9]"
                                                                                                            ),
                                                                    ),


                                                   # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
                                                   # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
                                                   #main result: 2d eff vs pt/eta
                                                   Efficiencies = cms.PSet( pt_eta_80_0 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                         UnbinnedVariables = cms.vstring("mass"),
                                                                                         BinnedVariables = cms.PSet(pt = cms.vdouble(20,30,40,50,100),
                                                                                                                    abseta = cms.vdouble(0,0.5,1.0,1.4442,1.566,2.0,2.5),
                                                                                                                    npf30  = cms.vdouble(-0.5,0.5)
                                                                                                                    ),
                                                                                         BinToPDFmap = cms.vstring("gaussPlusLinear0SemiFix80")
                                                                                         ),
                                                                            pt_eta_80_1 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                         UnbinnedVariables = cms.vstring("mass"),
                                                                                         BinnedVariables = cms.PSet(pt = cms.vdouble(20,30,40,50,100),
                                                                                                                    abseta = cms.vdouble(0,0.5,1.0,1.4442,1.566,2.0,2.5),
                                                                                                                    npf30  = cms.vdouble(0.5,1.5)
                                                                                                                    ),
                                                                                         BinToPDFmap = cms.vstring("gaussPlusLinear1SemiFix80")
                                                                                         ),
                                                                           pt_eta_95_0 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                         UnbinnedVariables = cms.vstring("mass"),
                                                                                         BinnedVariables = cms.PSet(pt = cms.vdouble(10,30,40,50,100),
                                                                                                                    abseta = cms.vdouble(0,0.5,1.0,1.4442,1.566,2.0,2.5),
                                                                                                                    npf30  = cms.vdouble(-0.5,0.5)
                                                                                                                    ),
                                                                                         BinToPDFmap = cms.vstring("gaussPlusLinear0SemiFix95")
                                                                                         ),
                                                                            pt_eta_95_1 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                                   UnbinnedVariables = cms.vstring("mass"),
                                                                                                   BinnedVariables = cms.PSet(pt = cms.vdouble(10,30,40,50,100),
                                                                                                                              abseta = cms.vdouble(0,0.5,1.0,1.4442,1.566,2.0,2.5),
                                                                                                                              npf30  = cms.vdouble(0.5,1.5)
                                                                                                                              ),
                                                                                                   BinToPDFmap = cms.vstring("gaussPlusLinear1SemiFix95")
                                                                                                   ),

                                                                            #control distributions
                                                                            pt80 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                          UnbinnedVariables = cms.vstring("mass"),
                                                                                          BinnedVariables = cms.PSet(pt = cms.vdouble(10,20,30,40,50,100),
                                                                                                                     ),
                                                                                          BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                          ),
                                                                            eta80 = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                            UnbinnedVariables = cms.vstring("mass"),
                                                                                            BinnedVariables = cms.PSet(abseta = cms.vdouble(0,0.4,0.8,1.2,1.4442,1.566,1.9,2.3,2.5)
                                                                                                                       ),
                                                                                            BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                            ),
                                                                            pt95 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                          UnbinnedVariables = cms.vstring("mass"),
                                                                                          BinnedVariables = cms.PSet(pt = cms.vdouble(10,20,30,40,50,100),
                                                                                                                     ),
                                                                                          BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                          ),
                                                                            eta95 = cms.PSet( EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                            UnbinnedVariables = cms.vstring("mass"),
                                                                                            BinnedVariables = cms.PSet(abseta = cms.vdouble(0,0.4,0.8,1.2,1.4442,1.566,1.9,2.3,2.5)
                                                                                                                       ),
                                                                                            BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                            ),
                                                                            npf30_wp95 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                                  UnbinnedVariables = cms.vstring("mass"),
                                                                                                  BinnedVariables = cms.PSet( npf30 = cms.vdouble(-0.5,0.5,1.5),
                                                                                                                              ),
                                                                                                  BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                  ),
                                                                            npf30_wp95Fix = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP95","pass"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( npf30 = cms.vdouble(1.5,2.5,3.5,4.5),
                                                                                                                                 ),
                                                                                                     BinToPDFmap = cms.vstring("gaussPlusLinear1Fix95")
                                                                                                     ),
                                                                            npf30_wp80 = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                                  UnbinnedVariables = cms.vstring("mass"),
                                                                                                  BinnedVariables = cms.PSet( npf30 = cms.vdouble(-0.5,0.5,1.5),
                                                                                                                              pt = cms.vdouble(20,100),
                                                                                                                              ),
                                                                                                  BinToPDFmap = cms.vstring("gaussPlusLinear")
                                                                                                  ),
                                                                            npf30_wp80Fix = cms.PSet(EfficiencyCategoryAndState = cms.vstring("passWP80","pass"),
                                                                                                     UnbinnedVariables = cms.vstring("mass"),
                                                                                                     BinnedVariables = cms.PSet( npf30 = cms.vdouble(1.5,2.5,3.5,4.5),
                                                                                                                                pt = cms.vdouble(20,100),
                                                                                                                                ),
                                                                                                     BinToPDFmap = cms.vstring("gaussPlusLinear1Fix80")
                                                                                                     ),

                                                                            )
                                                   )

process.fit = cms.Path( process.TagProbeFitTreeAnalyzerSel)

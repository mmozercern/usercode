# Simple counting experiment, with one signal and one background process
#imax 1  number of channels
#jmax 1  number of backgrounds 
#kmax *  number of nuisance parameters (sources of systematical uncertainties)
------------
shapes ggH CMS_hzz2l2q_mm0b hzz2l2q_mm0b.input.root  w:signal
shapes VBF CMS_hzz2l2q_mm0b hzz2l2q_mm0b.input.root  w:signal 
shapes background CMS_hzz2l2q_mm0b hzz2l2q_mm0b.input.root w:background
###shapes data_obs CMS_hzz2l2q_mm0b hzz2l2q_mm0b.input.root w:data_obs
shapes data_obs CMS_hzz2l2q_mm0b hzz2l2q_mm0b.input.root w:dataset_obs
------------
bin         CMS_hzz2l2q_mm0b
observation <dummyobs>
------------
bin                CMS_hzz2l2q_mm0b	  CMS_hzz2l2q_mm0b	          CMS_hzz2l2q_mm0b
process       ggH       		  				  VBF                 background
process         -1                                                          0                        1        
rate	    <dummy1>
------------
lumi		lnN	1.045			1.045			1.0
pdf_ggH	         <dummypdfggH>
pdf_qqH	       <dummypdfqqH>
QCDscale_ggH				 <dummyggH>
QCDscale_qqH			 <dummyVBF>
#theory_gamma                      <dummygammaBW>
CMS_trigger_m	lnN	1.02	1.02	1.0	
CMS_eff_m	lnN	1.008	1.008	1.0
CMS_scale_m	lnN	1.01	1.01	1.0
#CMS_recom	lnN	1.015	1.015	1.0
CMS_scale_j	<dummyJES>
CMS_eff_b 	<dummybeff>
CMS_hzz2l2q_pu	lnN		1.02	        1.02		1.0			      	
CMS_hzz2l2q_qgsep0b   lnN	1.046            1.046          1.0
#CMS_hzz2l2q_sig0bp0
#...                                                  <developing...>
#CMS_hzz2l2q_sig0bp5
CMS_hzz2l2q_bkg0bmmp0   <dummybnorm>
#CMS_hzz2l2q_bkg0bp1        param  222.72  ---
CMS_hzz2l2q_bkg0bp2         param 0.23690   0.036068 
#CMS_hzz2l2q_bkg0bp3      param   13.3992   ----
CMS_hzz2l2q_bkg0bp4         param -53.7855  10.273 


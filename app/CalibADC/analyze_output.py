import os
import ROOT as rt
import numpy as np

inputfile = "mc.root"
nplanes = 3

fin = rt.TFile(inputfile)

tree = fin.Get("adc")

fout = rt.TFile("out_analysis.root","recreate")
c = rt.TCanvas("c","c",800,600)
c.Draw()
fit = rt.TF1("mygaus","gaus")
gmeans = {}
gsigmas = {}

chhists = {} # one for each plane, channel

for p in range(0,nplanes):
    gmeans[p]  = rt.TGraph(900)
    gsigmas[p] = rt.TGraph(900)
    for ch in range(0,900):
        histname = "hadc_p%d_ch%d"%(p,ch)
        hch = rt.TH1D(histname,"Plane %d, Chanel %d"%(p,ch),50,0,500)
        tree.Draw("peak>>%s"%(histname),"planeid==%d && wireid==%d"%(p,ch))
        chhists[(p,ch)] = hch
        hch.Write()
        print "filled: ",(p,ch),": ",hch.Integral()
        c.Update()
        if hch.Integral()>500 and hch.GetMean()>125:
            fit.SetParameter(0,hch.GetMaximum())
            fit.SetParameter(1,hch.GetMean())
            fit.SetParameter(2,hch.GetRMS())
            if p in [0,2]:
                hch.Fit( fit, "RQ", "", 75, 400 )
            else:
                hch.Fit( fit, "RQ", "", 60, 250 )
            m = fit.GetParameter(1)
            s = fit.GetParameter(2)
            if m<0:
                print "bad fit ch=%d"%(ch),m
                # use the mean
                m = hch.GetMean()
                s = hch.GetRMS()
            gmeans[p].SetPoint(ch,ch,m)
            gsigmas[p].SetPoint(ch,ch,s)
            #fit.Draw("same")
        else:
            if hch.GetMean()>125:
                # low gain channel?
                hch.GetXaxis().SetRange(11,41)
            m = hch.GetMean()
            if hch.Integral()>3:
                s = hch.GetRMS()
            else:
                s = 0.0
            gmeans[p].SetPoint(ch,ch,m)
            gsigmas[p].SetPoint(ch,ch,s)
        #raw_input()
        
for p in range(0,nplanes):
    gmeans[p].Write( "gmean_plane%d"%(p))
for p in range(0,nplanes):
    gsigmas[p].Write( "gsigma_plane%d"%(p) )


print "Number of hists: ",len(chhists)


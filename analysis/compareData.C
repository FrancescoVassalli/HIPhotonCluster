

void makeRatio(TFile* thisFile){
	gStyle->SetOptStat(0);
	TH1F *spec  = (TH1F*) thisFile->Get("eSpec");
	TH1F *core  = (TH1F*) thisFile->Get("eCoreSpec");
	TH1F *HIspec, *HIcore;
	HIspecsub  = (TH1F*) thisFile->Get("eSpecHIsub");
	HIcoresub  = (TH1F*) thisFile->Get("eCoreSpecHIsub");
	HIspec  = (TH1F*) thisFile->Get("eSpecHI");
	HIcore  = (TH1F*) thisFile->Get("eCoreSpecHI");
	HIspec->Divide(spec);
	HIcore->Divide(core);
	HIspecsub->Divide(spec);
	HIcoresub->Divide(core);
	HIspec->SetTitle(";E_{#gamma} [GeV];E_{EMC_HIJING}/E_{EMC_Single}");
	HIspecsub->SetLineColor(kGreen-3);
	HIcoresub->SetLineColor(kGreen-3);
	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	HIspec->Draw();
	HIspecsub->Draw("same");
	tl->AddEntry(HIspec,"unsubtracted","l");
	tl->AddEntry(HIspecsub,"subtracted","l");
	tl->Draw();
	TLegend *tl2 = new TLegend(.7,.1,.9,.3);
	TCanvas *tc2 = new TCanvas();
	HIcore->Draw();
	HIcoresub->Draw("same");
	tl2->AddEntry(HIcore,"unsubtracted","l");
	tl2->AddEntry(HIcoresub,"subtracted","l");
	tl2->Draw();
}

void compareEff(TFile *thisFile){
	gStyle->SetOptStat(0);
	TH1F *eff  = (TH1F*) thisFile->Get("eff");
	TH1F* effsub  = (TH1F*) thisFile->Get("effsub");
	TH1F* effHI= (TH1F*) thisFile->Get("effHI");
	eff->Scale(1/eff->Integral());
	effsub->Scale(1/effsub->Integral());
	effHI->Scale(1/effHI->Integral());
	eff->SetTitle(";#frac{E_{#it{cluster}}}{#it{p}_{T}^{#it{truth}}};");
	effsub->SetLineColor(kGreen-3);
	effHI->SetLineColor(kMagenta-2);
	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	eff->Draw();
	effsub->Draw("same");
	effHI->Draw("same");
	tl->AddEntry(effHI,"unsubtracted HIJING","l");
	tl->AddEntry(effsub,"subtracted HIJING","l");
	tl->AddEntry(eff,"Single Photon","l");
	tl->Draw();
}

void compareDist(TFile *thisFile){
	gStyle->SetOptStat(0);
	TH1F *Dist  = (TH1F*) thisFile->Get("dist5_15");
	TH1F* Distsub  = (TH1F*) thisFile->Get("distsub5_15");
	TH1F* DistHI= (TH1F*) thisFile->Get("distHI5_15");
	Dist->SetTitle("5-15 Gev;#frac{E_{#it{cluster}}}{#it{p}_{T}^{#it{truth}}};");
	Distsub->SetLineColor(kGreen-3);
	DistHI->SetLineColor(kMagenta-2);
	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	Dist->Draw();
	Distsub->Draw("same");
	DistHI->Draw("same");
	tl->AddEntry(DistHI,"unsubtracted HIJING","l");
	tl->AddEntry(Distsub,"subtracted HIJING","l");
	tl->AddEntry(Dist,"Single Photon","l");
	tl->Draw();

	TH1F *Dist2  = (TH1F*) thisFile->Get("dist15_25");
	TH1F* Distsub2  = (TH1F*) thisFile->Get("distsub15_25");
	TH1F* DistHI2= (TH1F*) thisFile->Get("distHI15_25");
	Dist2->SetTitle("15-25 Gev;#frac{E_{#it{cluster}}}{#it{p}_{T}^{#it{truth}}};");
	Distsub2->SetLineColor(kGreen-3);
	DistHI2->SetLineColor(kMagenta-2);
	TLegend *tl2 = new TLegend(.7,.1,.9,.3);
	TCanvas *tc2 = new TCanvas();
	tc2->Draw();
	Dist2->Draw();
	Distsub2->Draw("same");
	DistHI2->Draw("same");
	tl2->AddEntry(DistHI2,"unsubtracted HIJING","l");
	tl2->AddEntry(Distsub2,"subtracted HIJING","l");
	tl2->AddEntry(Dist2,"Single Photon","l");
	tl2->Draw();

	TH1F *Dist3  = (TH1F*) thisFile->Get("dist15_25");
	TH1F* Distsub3  = (TH1F*) thisFile->Get("distsub15_25");
	TH1F* DistHI3= (TH1F*) thisFile->Get("distHI15_25");
	Dist3->SetTitle(">25 Gev;#frac{E_{#it{cluster}}}{#it{p}_{T}^{#it{truth}}};");
	Distsub3->SetLineColor(kGreen-3);
	DistHI3->SetLineColor(kMagenta-2);
	TLegend *tl3 = new TLegend(.7,.1,.9,.3);
	TCanvas *tc3 = new TCanvas();
	tc3->Draw();
	Dist3->Draw();
	Distsub3->Draw("same");
	DistHI3->Draw("same");
	tl3->AddEntry(DistHI3,"unsubtracted HIJING","l");
	tl3->AddEntry(Distsub3,"subtracted HIJING","l");
	tl3->AddEntry(Dist3,"Single Photon","l");
	tl3->Draw();
}


void compareData(){
	TFile *anaData = new TFile("anadata.root","READ");
	//makeRatio(anaData);
	compareEff(anaData);
}
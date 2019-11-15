

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


void compareData(){
	TFile *anaData = new TFile("anadata.root","READ");
	makeRatio(anaData);
}
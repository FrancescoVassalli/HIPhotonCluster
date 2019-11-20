

void makeRatio(TFile* thisFile){
	gStyle->SetOptStat(0);
	TH1F *spec  = (TH1F*) thisFile->Get("eSpec");
	TH1F *core  = (TH1F*) thisFile->Get("eCoreSpec");
	TH1F *HIspec, *HIcore, *HIspecsub, *HIcoresub;
	HIspecsub  = (TH1F*) thisFile->Get("eSpecsub");
	HIcoresub  = (TH1F*) thisFile->Get("eCoreSpecsub");
	HIspec  = (TH1F*) thisFile->Get("eSpecHI");
	HIcore  = (TH1F*) thisFile->Get("eCoreSpecHI");
	cout<<"here"<<endl;
	if (!(spec&&core&&HIspecsub&&HIcore&&HIspec&&HIcoresub))
	{
		cerr<<"Missing Plot!"<<endl;
	}
	spec->Rebin();
	core->Rebin();
	HIspec->Rebin();
	HIcoresub->Rebin();
	HIcore->Rebin();
	HIspecsub->Rebin();
	HIspec->Divide(spec);
	HIcore->Divide(core);
	HIspecsub->Divide(spec);
	HIcoresub->Divide(core);
	HIspec->SetTitle(";E_{#gamma} [GeV];E_{EMC_HIJING}/E_{EMC_Single}");
	HIspecsub->SetLineColor(kGreen-3);
	cout<<"here"<<endl;
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

void compareResponse(TFile *thisFile){
	gStyle->SetOptStat(0);
	for (int i = 0; i < 40; ++i)
	{
		string name = "response";
		string name1, name2, name3;
		name1=name2=name3=name;
		name1+=to_string(i);
		name2+="sub";
		name3+="HI";
		name2+=to_string(i);
		name3+=to_string(i);
		TH1F *response  = (TH1F*) thisFile->Get(name1.c_str());
		TH1F* responsesub  = (TH1F*) thisFile->Get(name2.c_str());
		TH1F* responseHI= (TH1F*) thisFile->Get(name3.c_str());
		response->Scale(1/response->Integral());
		responsesub->Scale(1/responsesub->Integral());
		responseHI->Scale(1/responseHI->Integral());
		string title = to_string(i);
		title+=" GeV;#frac{E_{#it{cluster}}}{#it{p}_{T}^{#it{truth}}};";
		response->SetTitle(title.c_str());
		responsesub->SetLineColor(kGreen-3);
		responseHI->SetLineColor(kMagenta-2);
		TLegend *tl = new TLegend(.7,.7,.9,.9);
		TCanvas *tc = new TCanvas();
		tc->Draw();
		response->GetYaxis()->SetRangeUser(0,.34);
		response->Draw();
		responsesub->Draw("same");
		responseHI->Draw("same");
		tl->AddEntry(responseHI,"unsubtracted HIJING","l");
		tl->AddEntry(responsesub,"subtracted HIJING","l");
		tl->AddEntry(response,"Single Photon","l");
		tl->Draw();
	}
}

void compareDist(TFile *thisFile){
	gStyle->SetOptStat(0);
	TH1F *Dist  = (TH1F*) thisFile->Get("dist5_15");
	TH1F* Distsub  = (TH1F*) thisFile->Get("distsub5_15");
	TH1F* DistHI= (TH1F*) thisFile->Get("distHI5_15");
	Dist->Scale(1/Dist->Integral());
	DistHI->Scale(1/DistHI->Integral());
	Distsub->Scale(1/Distsub->Integral());
	Dist->SetTitle("5-15 Gev;E_{#it{cluster}};");
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
	Dist2->Scale(1/Dist2->Integral());
	DistHI2->Scale(1/DistHI2->Integral());
	Distsub2->Scale(1/Distsub2->Integral());
	Dist2->SetTitle("15-25 Gev;E_{#it{cluster}};");
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

	TH1F *Dist3  = (TH1F*) thisFile->Get("dist25_35");
	TH1F* Distsub3  = (TH1F*) thisFile->Get("distsub25_35");
	TH1F* DistHI3= (TH1F*) thisFile->Get("distHI25_35");
	Dist3->Scale(1/Dist3->Integral());
	DistHI3->Scale(1/DistHI3->Integral());
	Distsub3->Scale(1/Distsub3->Integral());
	Dist3->SetTitle(">25 Gev;E_{#it{cluster}};");
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

void compareError(TFile *thisFile){
	gStyle->SetOptStat(0);
	TH1F *Res  = (TH1F*) thisFile->Get("eRes");
	TH1F *core  = (TH1F*) thisFile->Get("eResCore");
	TH1F *HIRes, *HIcore, *HIRessub, *HIcoresub;
	HIRessub  = (TH1F*) thisFile->Get("eRessub");
	HIcoresub  = (TH1F*) thisFile->Get("eResCoresub");
	HIRes  = (TH1F*) thisFile->Get("eResHI");
	HIcore  = (TH1F*) thisFile->Get("eResCoreHI");
	cout<<"here"<<endl;
	if (!(Res&&core&&HIRessub&&HIcore&&HIRes&&HIcoresub))
	{
		cerr<<"Missing Plot!"<<endl;
	}
	Res->Rebin();
	core->Rebin();
	HIRes->Rebin();
	HIcoresub->Rebin();
	HIcore->Rebin();
	HIRessub->Rebin();
	HIRes->Divide(Res);
	HIcore->Divide(core);
	HIRessub->Divide(Res);
	HIcoresub->Divide(core);
	HIRes->SetTitle(";E_{#gamma} [GeV];#sigma_{EMC_HIJING}/#sigma_{EMC_Single}");
	HIRessub->SetLineColor(kGreen-3);
	cout<<"here"<<endl;
	HIcoresub->SetLineColor(kGreen-3);
	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	HIRes->Draw();
	HIRessub->Draw("same");
	tl->AddEntry(HIRes,"unsubtracted","l");
	tl->AddEntry(HIRessub,"subtracted","l");
	tl->Draw();
	TLegend *tl2 = new TLegend(.7,.1,.9,.3);
	TCanvas *tc2 = new TCanvas();
	HIcore->Draw();
	HIcoresub->Draw("same");
	tl2->AddEntry(HIcore,"unsubtracted","l");
	tl2->AddEntry(HIcoresub,"subtracted","l");
	tl2->Draw();
}

void compareAverageResponse(TFile *thisFile){
	gStyle->SetOptStat(0);
	TH1F *single  = (TH1F*) thisFile->Get("Response_E");
	TH1F* sub  = (TH1F*) thisFile->Get("Response_Esub");
	TH1F* HI= (TH1F*) thisFile->Get("Response_EHI");
	/*single->Rebin();
	single->Scale(1./2);
	sub->Rebin();
	sub->Scale(1./2);
	HI->Rebin();
	HI->Scale(1./2);*/
	sub->SetLineColor(kGreen-3);
	HI->SetLineColor(kMagenta-2);
	single->SetTitle(";E_{#it{truth}} GeV;#frac{E_{#it{cluster}}}{E_{#it{truth}}};");
	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	single->GetYaxis()->SetTitleOffset(1);
	single->GetYaxis()->SetRangeUser(.8,1.2);
	tl->AddEntry(single,"Single","l");
	tl->AddEntry(sub,"Subtracted HIJING","l");
	tl->AddEntry(HI,"HIJING","l");
	single->Draw();
	sub->Draw("same");
	HI->Draw("same");
	tl->Draw();
}


void compareData(){
	TFile *anaData = new TFile("anadata.root","READ");
	makeRatio(anaData);
	compareError(anaData);
	compareResponse(anaData);
	compareAverageResponse(anaData);
	compareDist(anaData);
}
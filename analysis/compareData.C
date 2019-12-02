

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
	HIcore->SetTitle("Core;E_{#gamma} [GeV];E_{EMC_HIJING}/E_{EMC_Single}");
	HIspec->GetYaxis()->SetTitleOffset(1);
	HIcore->GetYaxis()->SetTitleOffset(1);
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

void compareResponse(TFile *thisFile, string savename){
	const int kBINS=40;
	gStyle->SetOptStat(0);
	TH1F *low = new TH1F("lowpTResponse","",kBINS,0,2);
	TH1F *medium = new TH1F("medpTResponse","",kBINS,0,2);
	TH1F *high = new TH1F("highpTResponse","",kBINS,0,2);
	TH1F *extreme = new TH1F("extremepTResponse","",kBINS,0,2);


	TH1F *lowHI = new TH1F("lowpTResponseHI","",kBINS,0,2);
	TH1F *mediumHI = new TH1F("medpTResponseHI","",kBINS,0,2);
	TH1F *highHI = new TH1F("highpTResponseHI","",kBINS,0,2);
	TH1F *extremeHI = new TH1F("extremepTResponseHI","",kBINS,0,2);


	TH1F *lowSub = new TH1F("lowpTResponseSub","",kBINS,0,2);
	TH1F *mediumSub = new TH1F("medpTResponseSub","",kBINS,0,2);
	TH1F *highSub = new TH1F("highpTResponseSub","",kBINS,0,2);
	TH1F *extremeSub = new TH1F("extremepTResponseSub","",kBINS,0,2);


	for (int i = 0; i < kBINS; ++i)
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
		if (i<kBINS/3)
		{
			low->Add(response);
			lowHI->Add(responseHI);
			lowSub->Add(responsesub);
		}
		else if (i<2*kBINS/3)
		{
			medium->Add(response);
			mediumHI->Add(responseHI);
			mediumSub->Add(responsesub);
		}
		else if (i<35)
		{
			high->Add(response);
			highHI->Add(responseHI);
			highSub->Add(responsesub);
		}
		else{
			extreme->Add(response);
			extremeHI->Add(responseHI);
			extremeSub->Add(responsesub);
		}
	}
	low->Scale(1/low->Integral());
	lowSub->Scale(1/lowSub->Integral());
	lowHI->Scale(1/lowHI->Integral());
	medium->Scale(1/medium->Integral());
	mediumSub->Scale(1/mediumSub->Integral());
	mediumHI->Scale(1/mediumHI->Integral());
	high->Scale(1/high->Integral());
	highSub->Scale(1/highSub->Integral());
	highHI->Scale(1/highHI->Integral());
	extreme->Scale(1/extreme->Integral());
	extremeHI->Scale(1/extremeHI->Integral());
	extremeSub->Scale(1/extremeSub->Integral());
	
	lowSub->SetLineColor(kGreen-3);
	low->SetLineColor(kMagenta-2);
	mediumSub->SetLineColor(kGreen-3);
	medium->SetLineColor(kMagenta-2);
	highSub->SetLineColor(kGreen-3);
	high->SetLineColor(kMagenta-2);
	extreme->SetLineColor(kMagenta-2);
	extremeSub->SetLineColor(kGreen-3);

	TLegend *tl = new TLegend(.7,.7,.9,.9);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	low->GetYaxis()->SetTitleOffset(1);
	low->SetTitle("low;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	low->Draw();
	lowSub->Draw("same");
	lowHI->Draw("same");
	tl->AddEntry(lowHI,"unsubtracted HIJING","l");
	tl->AddEntry(lowSub,"subtracted HIJING","l");
	tl->AddEntry(low,"Single Photon","l");
	tl->Draw();

	TLegend *tl2 = new TLegend(.7,.7,.9,.9);
	TCanvas *tc2 = new TCanvas();
	tc2->Draw();
	medium->GetYaxis()->SetTitleOffset(1);
	medium->SetTitle("medium;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	medium->Draw();
	mediumSub->Draw("same");
	mediumHI->Draw("same");
	tl2->AddEntry(mediumHI,"unsubtracted HIJING","l");
	tl2->AddEntry(mediumSub,"subtracted HIJING","l");
	tl2->AddEntry(medium,"Single Photon","l");
	tl2->Draw();

	TLegend *tl3 = new TLegend(.7,.7,.9,.9);
	TCanvas *tc3 = new TCanvas();
	tc3->Draw();
	high->GetYaxis()->SetTitleOffset(1);
	high->SetTitle("26-35 GeV;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	high->Draw();
	highSub->Draw("same");
	highHI->Draw("same");
	tl3->AddEntry(highHI,"unsubtracted HIJING","l");
	tl3->AddEntry(highSub,"subtracted HIJING","l");
	tl3->AddEntry(high,"Single Photon","l");
	tl3->Draw();

	TLegend *tl4 = new TLegend(.7,.7,.9,.9);
	TCanvas *tc4 = new TCanvas();
	tc4->Draw();
	extreme->GetYaxis()->SetTitleOffset(1);
	extreme->SetTitle("35+ GeV;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	extreme->Draw();
	extremeSub->Draw("same");
	extremeHI->Draw("same");
	tl4->AddEntry(extremeHI,"unsubtracted HIJING","l");
	tl4->AddEntry(extremeSub,"subtracted HIJING","l");
	tl4->AddEntry(extreme,"Single Photon","l");
	tl4->Draw();

	if (savename.length()>0)
	{
		savename+="compareResponse.pdf";
		TCanvas *saveCanvas = new TCanvas();
		saveCanvas->Divide(2,2);
		saveCanvas->cd(1);
		low->Draw();
		lowHI->Draw("same");
		lowSub->Draw("same");
		saveCanvas->cd(2);
		medium->Draw();
		mediumHI->Draw("same");
		mediumSub->Draw("same");
		saveCanvas->cd(3);
		high->Draw();
		highHI->Draw("same");
		highSub->Draw("same");
		saveCanvas->cd(4);
		extreme->Draw();
		extremeSub->Draw("same");
		extremeHI->Draw("same");
		saveCanvas->SaveAs(savename.c_str());
	}

	//Redo the same thing with the core \\\\\\\\\\\\\\\\

	/*TH1F *lowCore = new TH1F("lowpTResponseCore","",kBINS,0,2);
	TH1F *mediumCore = new TH1F("medpTResponseCore","",kBINS,0,2);
	TH1F *highCore = new TH1F("highpTResponseCore","",kBINS,0,2);

	TH1F *lowHICore = new TH1F("lowpTResponseHICore","",kBINS,0,2);
	TH1F *mediumHICore = new TH1F("medpTResponseHICore","",kBINS,0,2);
	TH1F *highHICore = new TH1F("highpTResponseHICore","",kBINS,0,2);

	TH1F *lowSubCore = new TH1F("lowpTResponseSubCore","",kBINS,0,2);
	TH1F *mediumSubCore = new TH1F("medpTResponseSubCore","",kBINS,0,2);
	TH1F *highSubCore = new TH1F("highpTResponseSubCore","",kBINS,0,2);

	for (int i = 0; i < kBINS; ++i)
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
		if (i<kBINS/3)
		{
			low->Add(response);
			lowHI->Add(responseHI);
			lowSub->Add(responsesub);
		}
		else if (i<2*kBINS/3)
		{
			medium->Add(response);
			mediumHI->Add(responseHI);
			mediumSub->Add(responsesub);
		}
		else
		{
			high->Add(response);
			highHI->Add(responseHI);
			highSub->Add(responsesub);
		}
	}
	low->Scale(1/low->Integral());
	lowSub->Scale(1/lowSub->Integral());
	lowHI->Scale(1/lowHI->Integral());
	medium->Scale(1/medium->Integral());
	mediumSub->Scale(1/mediumSub->Integral());
	mediumHI->Scale(1/mediumHI->Integral());
	high->Scale(1/high->Integral());
	highSub->Scale(1/highSub->Integral());
	highHI->Scale(1/highHI->Integral());
	
	lowSub->SetLineColor(kGreen-3);
	low->SetLineColor(kMagenta-2);
	mediumSub->SetLineColor(kGreen-3);
	medium->SetLineColor(kMagenta-2);
	highSub->SetLineColor(kGreen-3);
	high->SetLineColor(kMagenta-2);

	TLegend *tl = new TLegend(.7,.7,.9,.9);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	low->GetYaxis()->SetTitleOffset(1);
	low->SetTitle("low;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	low->Draw();
	lowSub->Draw("same");
	lowHI->Draw("same");
	tl->AddEntry(lowHI,"unsubtracted HIJING","l");
	tl->AddEntry(lowSub,"subtracted HIJING","l");
	tl->AddEntry(low,"Single Photon","l");
	tl->Draw();

	TLegend *tl2 = new TLegend(.7,.7,.9,.9);
	TCanvas *tc2 = new TCanvas();
	tc2->Draw();
	medium->GetYaxis()->SetTitleOffset(1);
	medium->SetTitle("medium;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	medium->Draw();
	mediumSub->Draw("same");
	mediumHI->Draw("same");
	tl2->AddEntry(mediumHI,"unsubtracted HIJING","l");
	tl2->AddEntry(mediumSub,"subtracted HIJING","l");
	tl2->AddEntry(medium,"Single Photon","l");
	tl2->Draw();

	TLegend *tl3 = new TLegend(.7,.7,.9,.9);
	TCanvas *tc3 = new TCanvas();
	tc3->Draw();
	high->GetYaxis()->SetTitleOffset(1);
	high->SetTitle("high;#frac{E_{#it{cluster}}}{E_{#it{truth}}};dN/dN");
	high->Draw();
	highSub->Draw("same");
	highHI->Draw("same");
	tl3->AddEntry(highHI,"unsubtracted HIJING","l");
	tl3->AddEntry(highSub,"subtracted HIJING","l");
	tl3->AddEntry(high,"Single Photon","l");
	tl3->Draw();*/
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
	/*Res->Rebin();
	core->Rebin();
	HIRes->Rebin();
	HIcoresub->Rebin();
	HIcore->Rebin();
	HIRessub->Rebin();*/
	HIRes->Divide(Res);
	HIcore->Divide(core);
	HIRessub->Divide(Res);
	HIcoresub->Divide(core);
	HIRes->SetTitle(";E_{#gamma} [GeV];#sigma_{EMC_HIJING}/#sigma_{EMC_Single}");
	HIcore->SetTitle("Core;E_{#gamma} [GeV];#sigma_{EMC_HIJING}/#sigma_{EMC_Single}");
	HIRes->GetYaxis()->SetTitleOffset(1);
	HIcore->GetYaxis()->SetTitleOffset(1);
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

void baseError(TFile *thisFile,string savename){
	gStyle->SetOptStat(0);
	TH1F *spec  = (TH1F*) thisFile->Get("eSpec");
	TH1F *core  = (TH1F*) thisFile->Get("eCoreSpec");
	TH1F* HIspecsub  = (TH1F*) thisFile->Get("eSpecsub");
	TH1F* HIcoresub  = (TH1F*) thisFile->Get("eCoreSpecsub");
	TH1F* HIspec  = (TH1F*) thisFile->Get("eSpecHI");
	TH1F* HIcore  = (TH1F*) thisFile->Get("eCoreSpecHI");

	TH1F *Res  = (TH1F*) thisFile->Get("eRes");
	TH1F *coreres  = (TH1F*) thisFile->Get("eResCore");
	TH1F* HIRessub  = (TH1F*) thisFile->Get("eRessub");
	TH1F* HIRescoresub  = (TH1F*) thisFile->Get("eResCoresub");
	TH1F* HIRes  = (TH1F*) thisFile->Get("eResHI");
	TH1F* HIRescore  = (TH1F*) thisFile->Get("eResCoreHI");

	/*if (!(Res&&core&&HIRessub&&HIcore&&HIRes&&HIcoresub))
	{
		cerr<<"Missing Plot!"<<endl;
	}*/
	/*Res->Rebin();
	core->Rebin();
	HIRes->Rebin();
	HIcoresub->Rebin();
	HIcore->Rebin();
	HIRessub->Rebin();*/

	Res->Divide(spec);
	coreres->Divide(core);
	HIRes->Divide(HIspec);
	HIRescore->Divide(HIcore);
	HIRessub->Divide(HIspecsub);
	HIRescoresub->Divide(HIcoresub);


	HIRes->SetTitle("Cluster Energy;E_{#gamma} [GeV];#frac{#sigma}{#mu}");
	HIcore->SetTitle("Core;E_{#gamma} [GeV];#frac{#sigma}{#mu}");
	HIRes->GetYaxis()->SetTitleOffset(1);
	HIcore->GetYaxis()->SetTitleOffset(1);

	HIRessub->SetLineColor(kGreen-3);
	Res->SetLineColor(kMagenta-2);
	HIcoresub->SetLineColor(kGreen-3);
	coreres->SetLineColor(kMagenta-2);

	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	HIRes->Draw();
	HIRessub->Draw("same");
	Res->Draw("same");
	tl->AddEntry(HIRes,"unsubtracted","l");
	tl->AddEntry(HIRessub,"subtracted","l");
	tl->AddEntry(Res,"single","l");
	tl->Draw();
	TLegend *tl2 = new TLegend(.7,.1,.9,.3);
	TCanvas *tc2 = new TCanvas();
	HIcore->Draw();
	HIcoresub->Draw("same");
	coreres->Draw("same");
	tl2->AddEntry(HIcore,"unsubtracted","l");
	tl2->AddEntry(HIcoresub,"subtracted","l");
	tl2->AddEntry(coreres,"single","l");
	tl2->Draw();
	if (savename.length()>0)
	{
		savename+="baseError.pdf";
		TCanvas *saveCanvas = new TCanvas();
		saveCanvas->Divide(2,1);
		saveCanvas->cd(1);
		Res->Draw();
		Res->GetYaxis()->SetRangeUser(.001,.05);
		HIRes->Draw("same");
		HIRessub->Draw("same");
		saveCanvas->cd(2);
		coreres->Draw();
		coreres->GetYaxis()->SetRangeUser(.001,.05);
		HIRescore->Draw("same");
		HIRescoresub->Draw("same");
		saveCanvas->SaveAs(savename.c_str());
	}
}

void baseResponseError(TFile *thisFile,string savename){
	gStyle->SetOptStat(0);

	TH1F *spec  = (TH1F*) thisFile->Get("Response_E");
	TH1F* HIspecsub  = (TH1F*) thisFile->Get("Response_Esub");
	TH1F* HIspec  = (TH1F*) thisFile->Get("Response_EHI");

	TH1F *Res  = (TH1F*) thisFile->Get("responseRes");
	TH1F* HIRessub  = (TH1F*) thisFile->Get("responseRessub");
	TH1F* HIRes  = (TH1F*) thisFile->Get("responseResHI");

	Res->Divide(spec);
	HIRes->Divide(HIspec);
	HIRessub->Divide(HIspecsub);


	HIRes->SetTitle("Cluster Energy Response;E_{#gamma} [GeV];#frac{#sigma}{#mu}");
	HIRes->GetYaxis()->SetTitleOffset(1);

	HIRessub->SetLineColor(kGreen-3);
	Res->SetLineColor(kMagenta-2);

	TLegend *tl = new TLegend(.7,.1,.9,.3);
	TCanvas *tc = new TCanvas();
	tc->Draw();
	HIRes->Draw();
	HIRessub->Draw("same");
	Res->Draw("same");
	tl->AddEntry(HIRes,"unsubtracted","l");
	tl->AddEntry(HIRessub,"subtracted","l");
	tl->AddEntry(Res,"single","l");
	tl->Draw();
	
	/*if (savename.length()>0)
	{
		savename+="baseError.pdf";
		TCanvas *saveCanvas = new TCanvas();
		saveCanvas->Divide(2,1);
		saveCanvas->cd(1);
		Res->Draw();
		Res->GetYaxis()->SetRangeUser(.001,.05);
		HIRes->Draw("same");
		HIRessub->Draw("same");
		saveCanvas->cd(2);
		coreres->Draw();
		coreres->GetYaxis()->SetRangeUser(.001,.05);
		HIRescore->Draw("same");
		HIRescoresub->Draw("same");
		saveCanvas->SaveAs(savename.c_str());
	}*/
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


void compareData(string savename=""){

	TFile *anaData = new TFile("anadata.root","READ");
	//makeRatio(anaData);
	//compareError(anaData);
	//compareResponse(anaData,savename);
	//compareAverageResponse(anaData);
	//compareDist(anaData);
	baseError(anaData,savename);
	baseResponseError(anaData,savename);
}
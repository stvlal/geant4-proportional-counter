void makeAnalysis()
{
    double energy = 11.; // keV
    int rootFileNumber = 2;

    double rightBorderInitialHist = 0.012; // MeV
    int nBinsInitialHist = 91;

    double rightBorderNewHist = 0.015; // MeV
    int nBinsNewHist = 96;

    int binShift = 0;


    stringstream rootFileN;
    rootFileN << rootFileNumber;

    stringstream energyStr;
    energyStr << energy;

    string rootFile = "output_" + rootFileN.str() + "_.root";
    TFile *inputFile = new TFile(rootFile.c_str(), "READ");

    TTree *tree = (TTree*)inputFile->Get("Scoring");

    int entries = tree->GetEntries();

    double fEdep;

    tree->SetBranchAddress("fEdep", &fEdep);

    TH1D *hist = new TH1D("hist", " ", nBinsInitialHist, 0., rightBorderInitialHist * 1e3);
    hist->GetXaxis()->SetTitle("E_{#gamma}, keV");
    hist->GetYaxis()->SetTitle("counts");
    hist->GetXaxis()->SetTitleSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.04);
    hist->GetXaxis()->SetTitleOffset(1.1);
    hist->GetYaxis()->SetTitleOffset(0.9);
    hist->SetLineWidth(1);
    hist->SetStats(0);

    for (int i = 0; i < entries; ++i)
    {
        tree->GetEntry(i);
        hist->Fill(fEdep * 1e3);
    }

    string initialHist = "deposition_" + energyStr.str() +"_keV";
    TCanvas *c1 = new TCanvas(initialHist.c_str(), initialHist.c_str());
    c1->cd();
    c1->SetLogy();
    hist->Draw();
    c1->RedrawAxis();
    c1->SetLeftMargin(0.085);
    c1->SetRightMargin(0.03);
    c1->SetTopMargin(0.04);
    c1->SetBottomMargin(0.1);

    TH1D *hist_new = new TH1D("hist_new", " ", nBinsNewHist, 0., rightBorderNewHist * 1e3);
    hist_new->GetXaxis()->SetTitle("E_{#gamma}, keV");
    hist_new->GetYaxis()->SetTitle("counts");
    hist_new->GetXaxis()->SetTitleSize(0.04);
    hist_new->GetYaxis()->SetTitleSize(0.04);
    hist_new->GetXaxis()->SetTitleOffset(1.1);
    hist_new->GetYaxis()->SetTitleOffset(0.9);
    hist_new->SetLineWidth(1);
    hist_new->SetStats(0);

    TRandom2 *rand = new TRandom2(2);

    double binEnergy;
    double binEnergy_new;
    double delta_E;
    double n_0 = energy / 0.022;
    for (int i = 0; i < hist->GetNbinsX(); ++i)
    {
        binEnergy = hist->GetBinCenter(i);

        for (int j = 0; j < hist->GetBinContent(i); ++j)
        {
            delta_E = sqrt( ((0.34+0.67)/n_0) + 1e-4 + (100/(n_0*1e3))*(100/(n_0*1e3)) );
            binEnergy_new = binEnergy + rand->Gaus(0., binEnergy * delta_E);
            hist_new->Fill(binEnergy_new);
        }
    }

    int rightBorder_bin;
    for (int i = 0; i < hist_new->GetNbinsX(); ++i)
    {
        if (hist_new->GetBinContent(i) != 0)
            rightBorder_bin = i;
    }

    int gaussPeak_bin = hist_new->GetMaximumBin();
    int leftBorder_bin = gaussPeak_bin - (rightBorder_bin - gaussPeak_bin) + binShift;

    double leftBorder, rightBorder, gaussPeak;
    leftBorder = hist_new->GetXaxis()->GetBinCenter(leftBorder_bin);
    rightBorder = hist_new->GetXaxis()->GetBinCenter(rightBorder_bin);


    TH1D *hist_new_clone = (TH1D*)hist_new->Clone();
    hist_new_clone->SetFillColor(42);
    hist_new_clone->GetXaxis()->SetRange(leftBorder_bin,rightBorder_bin);

    TH1D *hist_new_clone1 = (TH1D*)hist_new->Clone();
    hist_new_clone1->SetFillColor(kGreen-9);
    hist_new_clone1->GetXaxis()->SetRange(0, leftBorder_bin-1);

    string newHist = "new_deposition_" + energyStr.str() +"_keV";
    TCanvas *c2 = new TCanvas(newHist.c_str(), newHist.c_str());
    c2->cd();
    c2->SetLogy();
    hist_new->Draw();
    hist_new_clone->Draw("][, same");
    hist_new_clone1->Draw("][, same");
    c2->RedrawAxis();
    c2->SetLeftMargin(0.085);
    c2->SetRightMargin(0.03);
    c2->SetTopMargin(0.04);
    c2->SetBottomMargin(0.1);

    //TLegend *leg = new TLegend(0.25, 0.7, 0.6, 0.9);
    TLegend *leg = new TLegend(0.15, 0.7, 0.5, 0.9);
    leg->SetBorderSize(0);
    leg->AddEntry(hist_new_clone, "Total absorption peak", "f");
    leg->AddEntry(hist_new_clone1, "Scattered gamma-quanta", "f");
    leg->Draw();

    string c1PDF = initialHist + ".pdf";
    string c2PDF = newHist + ".pdf";
    c1->SaveAs(c1PDF.c_str());
    c2->SaveAs(c2PDF.c_str());

    TFile *outputFile = new TFile("outputFile.root", "UPDATE");
    c1->Write();
    c2->Write();

    outputFile->Close();

    double sigma = 2 * 3.1415926535898 * (1 - (100 / sqrt(5*5 + 100*100) ) );
    double n_reg = hist_new->Integral(leftBorder_bin, rightBorder_bin);

    double epsilon = (sigma / (4 * 3.1415926535898) ) * ( n_reg * n_0 / 1e7 );

    cout << "energy = " << energy << endl;
    cout << "n_reg = " << n_reg << endl;
    cout << "epsilon = " << epsilon << endl;

}

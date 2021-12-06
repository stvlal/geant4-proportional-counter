void makeAnalysis()
{
    TFile *inputFile = new TFile("output_0_.root", "read");

    TTree *tree = (TTree*)inputFile->Get("Scoring");

    int entries = tree->GetEntries();

    double fEdep;

    tree->SetBranchAddress("fEdep", &fEdep);

    TH1D *hist = new TH1D("hist", "hist", 91, 0., 0.045);

    for (int i = 0; i < entries; ++i)
    {
        tree->GetEntry(i);
        hist->Fill(fEdep);
    }

    TCanvas *c1 = new TCanvas();
    c1->cd();
    c1->SetLogy();
    hist->Draw();

    TH1D *hist_new = new TH1D("hist_new", "hist_new", 96, 0., 0.05);

    TRandom2 *rand = new TRandom2(2);

    double binEnergy;
    double binEnergy_new;
    double delta_E;
    double n_0 = 40. / 0.022;
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
    int leftBorder_bin = gaussPeak_bin - (rightBorder_bin - gaussPeak_bin);

    double leftBorder, rightBorder, gaussPeak;
    leftBorder = hist_new->GetXaxis()->GetBinCenter(leftBorder_bin);
    rightBorder = hist_new->GetXaxis()->GetBinCenter(rightBorder_bin);

    TLine *leftBorder_line = new TLine(leftBorder, 0., leftBorder, hist_new->GetMaximum());
    TLine *rightBorder_line = new TLine(rightBorder, 0., rightBorder, hist_new->GetMaximum());
    leftBorder_line->SetLineColor(kRed);
    rightBorder_line->SetLineColor(kRed);
    leftBorder_line->SetLineWidth(2);
    rightBorder_line->SetLineWidth(2);

    TCanvas *c2 = new TCanvas();
    c2->cd();
    c2->SetLogy();
    hist_new->Draw();
    leftBorder_line->Draw();
    rightBorder_line->Draw();

    TFile *outputFile = new TFile("outputFile.root", "RECREATE");
    c1->Write();
    c2->Write();

    outputFile->Close();

    double sigma = 2 * 3.1415926535898 * (1 - (100 / sqrt(5*5 + 100*100) ) );
    double n_reg = hist_new->Integral(leftBorder_bin, rightBorder_bin);

    double epsilon = (sigma / (4 * 3.1415926535898) ) * ( n_reg / n_0 );

    cout << "epsilon = " << epsilon << endl;

}

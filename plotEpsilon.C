void plotEpsilon()
{
    double energy1[11] = {2., 5., 8., 11., 14., 17., 20., 23., 26., 29., 32.};
    double energy2[9] = {35., 38., 41., 44., 47., 50., 53., 56., 59. };

    double epsilon1_not_per[11]= {6.47976e-05, 0.0844603, 0.180084, 0.187051, 0.155773,
                                  0.123021, 0.0967731, 0.0770715, 0.0622958,
                                  0.0511386, 0.0421065};
    double epsilon2_not_per[9] = {0.211115, 0.188153, 0.168979, 0.151497, 0.135617,
                                  0.122882, 0.109437, 0.0984248, 0.088495};

    double epsilon1[11];
    double epsilon2[9];
    int n_prim = 1e6;
    double omega = 2 * 3.1415926535898 * (1 - (100 / sqrt(5*5 + 100*100) ) );


    for (int i = 0; i < 11; ++i)
    {
        epsilon1[i] = epsilon1_not_per[i] * 100 / (energy1[i] / 0.022);
        cout << energy1[i] << " & " << n_prim << " & " <<
         epsilon1_not_per[i] * 4 * 3.14159265359 * n_prim / omega << " & " << epsilon1[i] << endl;
    }


    for (int i = 0; i < 9; ++i)
    {
        epsilon2[i] = epsilon2_not_per[i] * 100 / (energy2[i] / 0.022);
        cout << energy2[i] << " & " << n_prim << " & " <<
         epsilon2_not_per[i] * 4 * 3.14159265359 * n_prim / omega << " & " << epsilon2[i] << endl;
    }


    TGraph *myPlot = new TGraph(11, energy1, epsilon1);
    myPlot->SetMarkerStyle(21);
    myPlot->SetTitle(" ");
    myPlot->GetXaxis()->SetTitle("E_{#gamma}, keV");
    myPlot->GetYaxis()->SetTitle("Registration efficiency, %");
    myPlot->GetXaxis()->SetTitleSize(0.04);
    myPlot->GetYaxis()->SetTitleSize(0.04);
    myPlot->GetXaxis()->SetTitleOffset(1.1);
    myPlot->GetYaxis()->SetTitleOffset(1.1);
    myPlot->SetLineColor(2);
    myPlot->SetLineWidth(3);
    myPlot->GetYaxis()->SetRangeUser(0.,0.00055 * 100);
    myPlot->GetXaxis()->SetLimits(0.,61.);

    TGraph *myPlot2 = new TGraph(9, energy2, epsilon2);
    myPlot2->SetMarkerStyle(21);
    myPlot2->SetTitle(" ");
    myPlot2->SetLineColor(2);
    myPlot2->SetLineWidth(3);
    myPlot2->GetYaxis()->SetRangeUser(0.,0.00055 * 100);
    myPlot2->GetXaxis()->SetLimits(0.,61.);

    TLine *myLine = new TLine(33.5, epsilon1[10], 33.5, epsilon2[0]);
    myLine->SetLineColor(2);
    myLine->SetLineWidth(3);
    myLine->SetLineStyle(2);

/*
    TCanvas *c1 = new TCanvas();
    c1->cd();
    //c1->SetLogy();
    c1->SetLeftMargin(0.085);
    c1->SetRightMargin(0.03);
    c1->SetTopMargin(0.04);
    c1->SetBottomMargin(0.1);
    myPlot->Draw("APC");
    myPlot2->Draw("same, PC");
    myLine->Draw("same");
    //myPlot->Draw("P");
    c1->SaveAs("epsilon.pdf"); */
}

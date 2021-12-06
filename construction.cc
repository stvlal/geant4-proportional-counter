#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    filename = "";
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    nist = G4NistManager::Instance();

    // world -- air
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // counter -- stainless steel
    counterMat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    // window -- beryllium
    windowMat = new G4Material("windowMat", 1.848*g/cm3, 1);
    windowMat->AddElement(nist->FindOrBuildElement("Be"), 1);

    // radiator material -- aerogel
    G4double density = 2.55 * mg/cm3;
    G4double pressure = 0.5 * atmosphere;
    G4double temperature = 293. * kelvin;

    Xe = nist->FindOrBuildElement("Xe");

    C2H5OH = new G4Material("C2H5OH", 0.7895 * g/cm3, 4);
    C2H5OH->AddElement(nist->FindOrBuildElement("C"), 2);
    C2H5OH->AddElement(nist->FindOrBuildElement("H"), 5);
    C2H5OH->AddElement(nist->FindOrBuildElement("O"), 1);
    C2H5OH->AddElement(nist->FindOrBuildElement("H"), 1);

    radiatorMat = new G4Material("radiatorMat", density, 2, kStateGas,
                                               temperature, pressure);
    radiatorMat->AddElement(Xe, 90*perCent);
    radiatorMat->AddMaterial(C2H5OH, 10*perCent);


    // world volume (not drawn)
    solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld,
                                           "physWorld", 0, false, 0, true);

    // color for counter (brown)
    counterVisAtt = new G4VisAttributes(G4Colour(0.5,0.3,0.2));
    counterVisAtt->SetForceSolid(true);
    counterVisAtt->SetVisibility(true);

    // ------ proportional counter barrel geometry ------ //

    G4double Rmin = 17.5 * mm, Rmax = 18.5 * mm, deltaZ = 75. * mm;
    G4double Phimin = 0., deltaPhi = 360. * degree;
    solidBarrel_cyl = new G4Tubs("solidBarrel_cyl", Rmin, Rmax, deltaZ, Phimin, deltaPhi);
    solidBarrel_box = new G4Box("solidBarrel_box", 16.9 * mm, 5 * mm, 20 * mm);
    solidBarrel = new G4SubtractionSolid("solidRadiator", solidBarrel_cyl,
                            solidBarrel_box, 0, G4ThreeVector(2*16.8*mm,0.,0.));
    logicBarrel = new G4LogicalVolume(solidBarrel, counterMat, "logicBarrel");
    logicBarrel->SetVisAttributes(counterVisAtt);
    physBarrel = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicBarrel,
                                 "physBarrel", logicWorld, false, 0, true);


    // getting the CAD model (small_detail.stl)
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetReverse(false);

    // CAD model rotation
    rot = new G4RotationMatrix();
    rot->rotateZ(-90*deg);

    // creating the 'small things' at the sides of the window
    solidST = mesh->TessellatedMesh();
    logicST = new G4LogicalVolume(solidST, counterMat, "logicST", 0, 0, 0);
    logicST->SetVisAttributes(counterVisAtt);
    for (int i = 0; i < 2; ++i)
    {
        physST = new G4PVPlacement(rot, G4ThreeVector(-2.8e-10*m,0.,(-21. + i*(2*20. + 1))*mm),
                                    logicST, "physST", logicWorld, false, i, true);
    }

    // ------ end of proportional counter barrel geometry ------ //

    // end caps
    G4double Rmin_cap = 0., Rmax_cap = 18.5 * mm, deltaZ_cap = 0.5 * mm;
    G4double Phimin_cap = 0. , deltaPhi_cap = 360 * degree;
    solidCap = new G4Tubs("solidCap", Rmin_cap, Rmax_cap, deltaZ_cap,
                                           Phimin_cap, deltaPhi_cap);
    logicCap = new G4LogicalVolume(solidCap, counterMat, "logicCap");
    //logicCap->SetVisAttributes(G4VisAttributes::Invisible);
    logicCap->SetVisAttributes(counterVisAtt);
    for (int i = 0; i < 2; ++i)
    {
        physCap = new G4PVPlacement(0,
            G4ThreeVector(0.,0., (75.5 - (2 * i * 75.5)) * mm), logicCap,
                                  "physCap", logicWorld, false, i, true);
    }


    // color for window (transparent blue)
    windowVisAtt = new G4VisAttributes(G4Colour(0.0,0.4,0.8,0.5));
    windowVisAtt->SetForceSolid(true);
    windowVisAtt->SetVisibility(true);

    // window
    solidWindow = new G4Box("solidWindow", 15 * um, 5 * mm, 20 * mm);
    logicWindow = new G4LogicalVolume(solidWindow, windowMat, "logicWindow");
    logicWindow->SetVisAttributes(windowVisAtt);
    //logicWindow->SetVisAttributes(G4VisAttributes::Invisible);
    physWindow = new G4PVPlacement(0, G4ThreeVector(16.8 * mm, 0., 0.),
                      logicWindow,"physWindow", logicWorld, false, 0, true);


    // color for radiator (red)
    radiatorVisAtt = new G4VisAttributes(G4Colour(0.8,0.1,0.1));
    radiatorVisAtt->SetForceSolid(true);
    radiatorVisAtt->SetVisibility(true);

    // radiator
    G4double Rmin_rad = 0., Rmax_rad = 17.5 * mm, deltaZ_rad = 75. * mm;
    G4double Phimin_rad = 0., deltaPhi_rad = 360. * degree;

    solidCylinder = new G4Tubs("solidCylinder", Rmin_rad, Rmax_rad, deltaZ_rad,
                                                     Phimin_rad, deltaPhi_rad);
    solidBox = new G4Box("solidBox", 16.8 * mm, 20 * mm, 21 * mm);
    solidRadiator = new G4SubtractionSolid("solidRadiator", solidCylinder,
                            solidBox, 0, G4ThreeVector(2*16.78*mm,0.,0.));
    logicRadiator = new G4LogicalVolume(solidRadiator, radiatorMat, "logicRadiator");
    logicRadiator->SetVisAttributes(radiatorVisAtt);
    fScoringVolume = logicRadiator;
    //logicRadiator->SetVisAttributes(G4VisAttributes::Invisible);
    physRadiator = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
      logicRadiator,"physRadiator", logicWorld, false, 0, true);


    return physWorld;
}

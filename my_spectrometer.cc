#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QBBC.hh"
#include "G4VModularPhysicsList.hh"

#include "construction.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4RunManager *runManager = new G4RunManager();

    MyDetectorConstruction *myDetectorConstruction = new MyDetectorConstruction();

    G4UIExecutive *ui = 0;
    if (argc == 1)
    {
        G4cout << "Usage: " << G4endl;
        G4cout << "./my_spectrometer small_detail.stl, or" << G4endl;
        G4cout << "./tof_Detector run.mac small_detail.stl" << G4endl;
        return 0;
    }
    else if (argc == 2)
    {
        ui = new G4UIExecutive(argc, argv);
        myDetectorConstruction->SetCADFilename(argv[1]);
    }
    else if (argc == 3)
    {
        myDetectorConstruction->SetCADFilename(argv[2]);
    }

    runManager->SetUserInitialization(myDetectorConstruction);

    G4VModularPhysicsList *physicsList = new QBBC;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new MyActionInitialization());

    runManager->Initialize();

    // random seeds
    G4Random::setTheEngine(new CLHEP::RanecuEngine());
    G4long seed = time(NULL);
    G4Random::setTheSeed(seed);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        // interactive mode
        UImanager->ApplyCommand("/control/execute vis.mac");

        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];

        UImanager->ApplyCommand(command + fileName);
    }


    delete runManager;
    delete visManager;

    return 0;
}

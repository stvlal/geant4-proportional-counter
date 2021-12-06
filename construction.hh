#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Trd.hh"
#include "CADMesh.hh"
#include <string>
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4RunManager.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct();

    void SetCADFilename(std::string name)
    {
        filename = name;
    };

private:
    G4LogicalVolume *fScoringVolume;

    std::string filename;
    G4NistManager *nist;
    G4Material *worldMat;
    G4Material *counterMat;
    G4Material *windowMat;

    G4Element *Xe;
    G4Material *C2H5OH;
    G4Material *radiatorMat;

    G4Box *solidWorld;
    G4LogicalVolume *logicWorld;
    G4VPhysicalVolume *physWorld;

    G4VisAttributes *counterVisAtt;

    G4Tubs *solidBarrel_cyl;
    G4Box *solidBarrel_box;
    G4VSolid *solidBarrel;
    G4LogicalVolume *logicBarrel;
    G4VPhysicalVolume *physBarrel;

    CADMesh *mesh;
    G4RotationMatrix *rot;

    G4VSolid *solidST;
    G4LogicalVolume *logicST;
    G4VPhysicalVolume *physST;

    G4VisAttributes *windowVisAtt;

    G4Box *solidWindow;
    G4LogicalVolume *logicWindow;
    G4VPhysicalVolume *physWindow;

    G4Tubs *solidCap;
    G4LogicalVolume *logicCap;
    G4VPhysicalVolume *physCap;


    G4VisAttributes *radiatorVisAtt;

    G4Tubs *solidCylinder;
    G4Box *solidBox;
    G4VSolid *solidRadiator;
    G4LogicalVolume *logicRadiator;
    G4VPhysicalVolume *physRadiator;

};

#endif

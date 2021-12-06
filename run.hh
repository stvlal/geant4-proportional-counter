#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "g4root.hh"

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run *run);
    virtual void EndOfRunAction(const G4Run*);
};

#endif

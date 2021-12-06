#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);

    G4ThreeVector pos(116.8 * mm,0.,0.);

    G4double cosTheta;
    while (true)
    {
        cosTheta = -1.0 + 2.0*G4UniformRand();
        if (cosTheta <= -0.998752338878)
            break;
    }

    G4double phi = CLHEP::twopi*G4UniformRand();
    G4double sinTheta = sqrt(1 - cosTheta*cosTheta);
    fParticleGun->SetParticleMomentumDirection(
        G4ThreeVector(cosTheta,sinTheta*sin(phi),sinTheta*cos(phi)));

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleEnergy(40.*keV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

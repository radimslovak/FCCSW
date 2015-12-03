#include "SimG4Common/RunManager.h"

// Geant
#include "G4VModularPhysicsList.hh"

namespace sim {
  RunManager::RunManager(): G4RunManager(), m_prevEventTerminated(true), m_msgSvc("MessageSvc","RunManager"), m_log(&(*m_msgSvc),"RunManager") {}

  RunManager::~RunManager() {}

  StatusCode RunManager::start() {
    // as in G4RunManager::BeamOn()
    if(G4RunManager::ConfirmBeamOnCondition()) {
      G4RunManager::ConstructScoringWorlds();
      G4RunManager::RunInitialization();
      return true;
    } else {
      return false;
    }
  }

  bool RunManager::processEvent(G4Event* aEvent) {
    if(!m_prevEventTerminated) {
      return false;
    }
    G4RunManager::currentEvent = aEvent;
    G4RunManager::eventManager->ProcessOneEvent(G4RunManager::currentEvent);
    G4RunManager::AnalyzeEvent(G4RunManager::currentEvent);
    G4RunManager::UpdateScoring();
    m_prevEventTerminated = false;
    return true;
  }

  bool RunManager::retrieveEvent(const G4Event*& aEvent) {
    if(m_prevEventTerminated) {
      m_log<<MSG::ERROR<<"Trying to retrieve an event, but no event has been processed by GEant"<<endmsg;
      return false;
    }
    aEvent = G4RunManager::GetCurrentEvent();
      return true;
  }

  bool RunManager::terminateEvent() {
    G4RunManager::TerminateOneEvent();
    m_prevEventTerminated = true;
    return true;
  }
  void RunManager::finalize() {
    G4RunManager::RunTermination();
  }
}

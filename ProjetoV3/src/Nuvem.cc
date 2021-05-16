#include <string.h>
#include <omnetpp.h>
#include <math.h>

using namespace omnetpp;

class Nuvem: public cSimpleModule {
    protected:
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Nuvem);

void Nuvem::handleMessage(cMessage *msg) {
    cGate *arrivalGate = msg->getArrivalGate();
    int arrivalGateIndex = arrivalGate->getIndex();
    EV << "Enviando para o servidor pela porta: " << arrivalGateIndex << "\n";
    send(msg, "out", arrivalGateIndex);
}

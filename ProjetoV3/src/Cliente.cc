#include <string.h>
#include <omnetpp.h>
#include <math.h>

using namespace omnetpp;

class Cliente : public cSimpleModule {

  private:
    long enviado;
    int i;
    int qtdUser;
    int qtdMsg;

  protected:
    //virtual mensagem *generateMessage();
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarNovaMensagem();
};

Define_Module(Cliente);

void Cliente::initialize() {

    enviado = 0;
    i = 1;
    qtdUser = 40;
    qtdMsg = qtdUser * 6;

    WATCH(enviado);

    if(par("primeiraMsg").boolValue() == true){
        for(int j = 0; j <= (qtdUser - 1); j++){
            enviarNovaMensagem();
        }
    }

    /*if(par("primeiraMsg").boolValue() == true){
        cMessage *msg = new cMessage("Mensagem enviada");
        EV << "Mensagem inicial. Id:"<< getId() << "\n";
        send(msg, "out", 0);
        enviado++;
    }*/
}

void Cliente::handleMessage(cMessage *msg) {
    if(enviado >= qtdMsg){
      delete msg;
      finish();
    }else if(enviado >= (qtdUser - 1)){
      enviarNovaMensagem();
      send(msg, "out", 0);
      enviado++;
    }else{
      EV << "Texto: \"" << msg->getFullName() << "\"" << endl;
      send(msg, "out", 0);
      enviado++;
    }
}

void Cliente::enviarNovaMensagem() {
    SimTime time = simTime() + 0.1;
    //std::string text = "Tarefa " + time.str();
    std::string value = std::to_string(i).c_str();
    std::string text = "Tarefa " + value;
    cMessage *postMsg = new cMessage(text.c_str());
    scheduleAt(time, postMsg);
    //send(postMsg, "out", 0);
    i++;
}

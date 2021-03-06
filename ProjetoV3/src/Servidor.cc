#include <string.h>
#include <omnetpp.h>
#include <math.h>

using namespace omnetpp;

class Servidor: public cSimpleModule {
    private:
        int capacidadeFila;
        double tempoProcessamento;
        cQueue fila;
        cMessage *processando;
        cMessage *msgFimProcesso;
        cHistogram filaStats;
        cOutVector filaVector;
        cHistogram tempoServicoStats;
        cOutVector tempoServicoVector;

        virtual void processar(cMessage *msg);
        virtual void colocarFila(cMessage *msg);

    public:
        int length();
        virtual ~Servidor() override;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};

Define_Module(Servidor);


Servidor::~Servidor() {
    if (processando) {
        cancelAndDelete(processando);
        delete processando;
    }
    cancelAndDelete(msgFimProcesso);
}

void Servidor::initialize() {
    msgFimProcesso = new cMessage("fim");
    capacidadeFila = par("capacidadeFila");
    tempoProcessamento = par("tempoProcessamento");
    filaStats.setName("filaStats");
    filaVector.setName("fila");
    tempoServicoStats.setName("tempoServicoStats");
    tempoServicoVector.setName("tempoServico");
}


void Servidor::handleMessage(cMessage *msg) {
    EV << "Recebeu \"" << msg->getFullName() << "\"."<< endl;
       if (msg==msgFimProcesso) {
           EV << "Fim do processamento de \"" << processando->getFullName() << "\"." << endl;
           delete processando;
           if (fila.isEmpty()) {
               processando = nullptr;
           } else {
               processando = check_and_cast<cMessage *>(fila.pop());
               processar(processando);
           }
       } else if (!processando) {
           processando = msg;
           processar(processando);
       } else {
           colocarFila(msg);
       }
       int tamanhoFila = fila.getLength();
       filaVector.record(tamanhoFila);
       filaStats.collect(tamanhoFila);
}

void Servidor::colocarFila(cMessage *msg) {
    if (capacidadeFila > 0 && fila.getLength() == capacidadeFila) {
        EV << "Descartando \""<< msg->getFullName() << "\". Motivo: fila cheia (#fila: " << capacidadeFila << "." << endl;
        delete msg;
    } else {
        fila.insert(msg);
        EV << "Colocando \"" << msg->getFullName() << "\" na fila (#fila: " << fila.getLength() << ")." << endl;
    }
}

void Servidor::processar(cMessage *msg) {
    //simtime_t tempoServico = const_simtime_t(tempoProcessamento);
    simtime_t tempoServico = exponential(tempoProcessamento);
    EV << "Processando \"" << msg->getFullName() << "\" por " << tempoServico << "s." << endl;
    scheduleAt(simTime()+tempoServico, msgFimProcesso);
    tempoServicoVector.record(tempoServico);
    tempoServicoStats.collect(tempoServico);
}

void Servidor::finish() {
    EV << "Fila, min:    " << filaStats.getMin() << endl;
    EV << "Fila, max:    " << filaStats.getMax() << endl;
    EV << "Fila, mean:   " << filaStats.getMean() << endl;
    EV << "Fila, stddev: " << filaStats.getStddev() << endl;
    filaStats.recordAs("fila");
    EV << "Tempo de Servico, min:    " << tempoServicoStats.getMin() << endl;
    EV << "Tempo de Servico, max:    " << tempoServicoStats.getMax() << endl;
    EV << "Tempo de Servico, mean:   " << tempoServicoStats.getMean() << endl;
    EV << "Tempo de Servico, stddev: " << tempoServicoStats.getStddev() << endl;
    //EV << "Tempo total: " << tempoProcessamento*2;
    tempoServicoStats.recordAs("tempoDeServico");
}


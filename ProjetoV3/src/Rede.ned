package projetov3;

network Rede
{
    parameters:
        int nclients = default(1);
        @display("bgb=231,106");

    submodules:
        cliente: Cliente {
            parameters:
                primeiraMsg = true;
        }

        nuvem: Nuvem;
        servidor: Servidor;
    connections:
            cliente.out++ --> {  delay = 100ms; } --> nuvem.in++;
            nuvem.out++ --> {  delay = 100ms; } --> servidor.in++;
            //servidor.out++ --> {  delay = 100ms; } --> nuvem.in++;
            //nuvem.out++ --> {  delay = 100ms; } --> cliente[i].in++;
        
        //cliente[1].out++ --> {  delay = 100ms; } --> nuvem.in++;
        //cliente[1].in++ <-- {  delay = 100ms; } <-- nuvem.out++;




}

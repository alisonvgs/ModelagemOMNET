package projetov3;

simple Cliente {
    parameters:
        @display("i=device/laptop");
        bool primeiraMsg = default(true);
        //double delay = default(1000);
    gates:
        input in[];
        output out[];
}
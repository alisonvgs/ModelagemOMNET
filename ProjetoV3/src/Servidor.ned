package projetov3;

simple Servidor {
    parameters:
        @display("i=device/mainframe");
        int capacidadeFila = default(0);
        double tempoProcessamento = default(0.41);
    gates:
        input in[];
        output out[];
}

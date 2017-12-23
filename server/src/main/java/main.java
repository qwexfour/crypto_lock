import socket.SocketProcessor;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;

public class main {
    static {
        try {
            System.setErr(new PrintStream(new File("log.txt")));
        } catch (FileNotFoundException e) {
            System.out.println("ERROR");
        }
    }
    public static void main(String args[]) throws Throwable {
        ServerSocket ss = new ServerSocket(8030);
        while (true) {
            Socket s = ss.accept();
            System.err.println("Client accepted");
            new Thread(new SocketProcessor(s)).start();
        }
    }
}

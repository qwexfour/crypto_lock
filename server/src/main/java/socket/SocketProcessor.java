package socket;

import utils.Processor;

import java.io.*;
import java.net.Socket;



public class SocketProcessor implements Runnable{


    private Socket s;
    private InputStream is;
    private OutputStream os;
    private Processor processor;


    public SocketProcessor(Socket s) throws Throwable {
        this.s = s;
        this.is = s.getInputStream();
        this.os = s.getOutputStream();

    }

    public void run() {
        try {
            processor = new Processor(readInputMessages());
            processor.preParsing();
            writeResponse(processor.start());

        } catch (Throwable t) {
                System.err.println("Socket Error" + t.getMessage());
        } finally {
            try {
                s.close();
            } catch (Throwable t) {
                System.err.println("Socket Error:" + t.getMessage());
            }
        }
        System.err.println("Client processing finished");
    }

    public void writeResponse(String response) throws Throwable {
        System.err.println(response);

        os.write(response.getBytes());
        os.flush();
    }

    public String readInputMessages() throws Throwable {
        BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
        String massage = null;
        massage = in.readLine();// System.out.println(massage);

        return massage;
    }
}

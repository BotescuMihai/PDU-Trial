import java.net.*;
import java.io.*;


public class javainetclient {
    public static class msgIntType implements Serializable{
        public int msg;

        public msgIntType(){

        }
    }

    public static class msgHeaderType implements Serializable{
        public int msgSize;
        public int clientID;
        public int opID;
        public String fileName;

        public msgHeaderType(){

        }
    }

    public static class singleIntMsgType implements Serializable{
        public msgHeaderType header;
        public msgIntType i;

        public singleIntMsgType(){
            header = new msgHeaderType();
            i = new msgIntType();
        }
    }

    public static class msgStringType implements Serializable{
        public String msg;

        public msgStringType(){

        }
    }
    static int writeSingleInt (Socket sock, msgHeaderType h, int i) throws Exception{			// Build the message and send it!
        singleIntMsgType s = new singleIntMsgType();
        s.header.clientID = h.clientID;
        s.header.opID = h.opID;
        s.i.msg = i ;
        s.header.msgSize = 100;
        s.header.fileName = h.fileName;
        ObjectOutputStream os = new ObjectOutputStream(sock.getOutputStream());
        ObjectInputStream is = new ObjectInputStream(sock.getInputStream());
        os.writeObject(s);
        return s.header.msgSize;
    }

    static void readSingleInt(Socket sock) throws Exception{
    	singleIntMsgType s ;
        ObjectOutputStream os = new ObjectOutputStream(sock.getOutputStream());
        ObjectInputStream is = new ObjectInputStream(sock.getInputStream());
        String ceva;

      //  while((s = (singleIntMsgType)is.readObject()) != null){
         s = (singleIntMsgType)is.readObject();
            System.out.println(s);
       // }
    }

    public static void main(String[] args) throws Exception{
        client();
    }

    public static void client() throws Exception{
        String hostname = "127.0.0.1";
        int port = 18081;

        try (Socket socket = new Socket(hostname, port)) {
            msgHeaderType h = new msgHeaderType();
            h.clientID = 0;
            h.fileName = "/serv_files/INET/lorem_transfered.txt";
            int ok = writeSingleInt(socket, h, 0);
            readSingleInt(socket);
        } catch (UnknownHostException ex) {

            System.out.println("Server not found: " + ex.getMessage());

        } catch (IOException ex) {
            System.out.println("I/O error: " + ex.getMessage());
        }
    }
}
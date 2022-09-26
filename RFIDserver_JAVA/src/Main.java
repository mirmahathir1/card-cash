import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Main {

    public static void main(String[] args) {

        ClientDatabase clientDatabase=new ClientDatabase();
        clientDatabase.readFromPath();

        ServerSocket serverSocket;
        Socket socket;


        try {
            serverSocket=new ServerSocket(11500);

            while (true){
                //System.out.println("open");
                socket=serverSocket.accept();
                //System.out.println("done");

                ClientHandler clientHandler=new ClientHandler(socket,clientDatabase);
                Thread thread=new Thread(clientHandler);
                thread.start();

                System.out.print("Device connected with ");
            }


        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

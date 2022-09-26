
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientHandler implements Runnable {
    BufferedReader bufferedReader;
    PrintWriter printWriter;
    Socket client;

    ClientDatabase clientDatabase;

    public ClientHandler(Socket socket,ClientDatabase clientDb) {
        clientDatabase=clientDb;
        client=socket;
        //System.out.println("");
        try {
            bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            printWriter = new PrintWriter(socket.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        String agentId;
        String data;
        try {
            agentId=bufferedReader.readLine();
            System.out.println("AgentId: "+agentId);
            data=bufferedReader.readLine();
            System.out.println("Transaction request from RFID: "+data);
            if(data!=null ){
                Client client=clientDatabase.getClientByRFID(data);

                if(client!=null){
                    System.out.println("Name of Client: "+client.name+" , Current Balance: "+client.amount);
                    if(client.password.equalsIgnoreCase("$$$$"))
                        printWriter.write(TRANSACTION_TYPE.REGULAR_TRANSACTION+client.name+" "+client.password+" "+client.amount);
                    else
                        printWriter.write(TRANSACTION_TYPE.TRANSACTION_WITH_PASSWORD+" "+client.name+" "+client.password+" "+client.amount);

                    printWriter.flush();

                    data=bufferedReader.readLine();
                    System.out.println("Transaction amount: "+data);
                    int amount=Integer.parseInt(data);
                    client.amount=client.amount+amount;
                    clientDatabase.writeToPath();

                    clientDatabase.addTransaction("Transfer amount: "+ amount,agentId);

                    System.out.println("Final Balance: "+client.amount);
                    System.out.println();
                }
                else{
                    printWriter.write(TRANSACTION_TYPE.INVALID_RFID+" "+"null"+" "+"null"+" "+"0");
                    printWriter.flush();

                    data=bufferedReader.readLine();
                    System.out.println("Amount: "+data);
                }

            }
            client.close();
        } catch (IOException e) {
            System.out.println("Client does not communicate properly");
        }

    }
}

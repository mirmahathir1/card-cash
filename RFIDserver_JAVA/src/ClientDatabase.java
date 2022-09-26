
import java.io.*;
import java.util.ArrayList;

public class ClientDatabase {
    public ArrayList<Client> clients;

    public ClientDatabase(){
        clients= new ArrayList<>();
    }

    public void addTransaction(String description,String agentNo){
        Transaction transaction=new Transaction(description,agentNo);

        BufferedWriter transactionHistory;
        try {
            transactionHistory=new BufferedWriter(new FileWriter("src/transactionHistory",true));

            transactionHistory.write(transaction.toString());
            transactionHistory.flush();
            transactionHistory.close();

        } catch (Exception e) {
            System.out.println("Transaction file not found");
        }
    }


    public void addClient(Client client)
    {
        clients.add(client);
    }


    public Client getClientByRFID(String rfid)
    {
        for(Client i:clients)
        {
            if(i.rfid.equalsIgnoreCase(rfid)){
                return i;
            }
        }
        return null;
    }

    public void addDefaultClient(){
        addClient(new Client("Mahathir"," 151 129 97 221","$$$$",100));
        addClient(new Client("Ashraful"," 251 06 202 34","$$$$",50));
        addClient(new Client("Enan"," 71 215 110 221","4546",25));
    }

    public void readFromPath()
    {
        readFromPath("src/clientDatabase");
    }
    public void readFromPath(String filePath){
        try {

            FileInputStream fstream = new FileInputStream(filePath);
            BufferedReader br = new BufferedReader(new InputStreamReader(fstream));

            String strLine;

            while ((strLine = br.readLine()) != null) {
                String[] splitted = strLine.split(",");
                clients.add(new Client(splitted[0],splitted[1],splitted[2],Integer.parseInt(splitted[3])));

            }

            fstream.close();
        }catch(Exception e){
            e.printStackTrace();
        }
    }

    public void writeToPath(){
        writeToPath("src/clientDatabase");
    }

    public void writeToPath(String filePath){
        BufferedWriter bw = null;
        try {
            File file = new File(filePath);

            if (!file.exists()) {
                file.createNewFile();
            }

            FileWriter fw = new FileWriter(file);
            bw = new BufferedWriter(fw);
            for(Client i:clients){
                bw.write(i.name+","+i.rfid+","+i.password+","+i.amount+'\n');
            }



        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
        finally
        {
            try{
                if(bw!=null)
                    bw.close();
            }catch(Exception ex){
                System.out.println("Error in closing the BufferedWriter"+ex);
            }
        }
    }
    public void print()
    {
        for(Client i: clients){
            i.print();
        }
    }
}

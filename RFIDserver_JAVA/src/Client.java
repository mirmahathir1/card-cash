import java.io.Serializable;

public class Client {
    public String name="";
    public String rfid="";
    public String password="";
    public int amount=0;

    public Client(String name, String rfid, String password, int amount) {
        this.name = name;
        this.rfid = rfid;
        this.password = password;
        this.amount = amount;
    }
    public void print(){
        System.out.println("Name: "+name+", rfid: "+rfid+", password: "+password+", amount: "+amount);
    }
}

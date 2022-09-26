public class ClientDBTester {
    public static void main(String[] args) {
        ClientDatabase clientDatabase= new ClientDatabase();
        clientDatabase.readFromPath();
        clientDatabase.print();

        clientDatabase.writeToPath();

    }
}

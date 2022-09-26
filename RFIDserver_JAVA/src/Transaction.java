import java.util.Calendar;
import java.util.Date;

public class Transaction {
    Date date;
    String description;
    String agentNo;

    public Transaction(String description, String agentNo) {
        this.description = description;
        this.agentNo = agentNo;
        date= Calendar.getInstance().getTime();
    }

    @Override
    public String toString() {
        return "Transaction: description='"+ description + "', agentNo='" + agentNo+ "', date='" + date+"'\n";
    }
}

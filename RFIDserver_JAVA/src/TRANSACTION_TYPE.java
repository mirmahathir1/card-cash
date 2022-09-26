public enum TRANSACTION_TYPE {
    INVALID_TRANSACTION (0),
    REGULAR_TRANSACTION (1),
    TRANSACTION_WITH_PASSWORD (2),
    SERVER_NOT_CONNECTED (3),
    INVALID_RFID(4);

    int value;

    TRANSACTION_TYPE(int value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return value+"";
    }
}

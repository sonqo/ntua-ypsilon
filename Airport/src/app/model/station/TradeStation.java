package app.model.station;

public class TradeStation extends Station {

    int maxtime = 90;
    String label = "tradeNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

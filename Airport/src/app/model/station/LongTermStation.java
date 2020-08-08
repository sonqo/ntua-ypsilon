package app.model.station;

public class LongTermStation extends Station {

    int maxtime = 600;
    String label = "longNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

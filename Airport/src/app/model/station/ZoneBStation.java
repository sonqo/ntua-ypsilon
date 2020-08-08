package app.model.station;

public class ZoneBStation extends Station {

    int maxtime = 120;
    String label = "zoneBNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

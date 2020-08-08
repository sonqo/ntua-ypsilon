package app.model.station;

public class ZoneAStation extends Station {

    int maxtime = 90;
    String label = "zoneANum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

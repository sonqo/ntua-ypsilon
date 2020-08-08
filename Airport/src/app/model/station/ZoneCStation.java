package app.model.station;

public class ZoneCStation extends Station {

    int maxtime = 180;
    String label = "zoneCNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

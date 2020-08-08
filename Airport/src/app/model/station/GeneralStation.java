package app.model.station;

public class GeneralStation extends Station {

    int maxtime = 240;
    String label = "generalNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }
}

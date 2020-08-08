package app.model.station;

import javafx.scene.control.Label;

public class GateStation extends Station {

    public int maxtime = 45;
    public String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }

}


package app.model.station;

import javafx.scene.control.Label;

public class Station {

    public String id;
    public int gate_type, spot_num, cost, maxtime;

    public void setInfo(int gate_type, int spot_num, int cost, String id){
        this.id = id;
        this.cost = cost;
        this.gate_type = gate_type;
        this.spot_num = spot_num;
    }

    public int getMaxtime(){
        return this.maxtime;
    }

}


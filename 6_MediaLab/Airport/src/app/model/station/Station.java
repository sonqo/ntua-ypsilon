package app.model.station;

public class Station {

    public String id, flight_id = "None";
    public int gate_type, spot_num, cost, maxtime;

    public void setInfo(int gate_type, int cost, String id){
        this.id = id;
        this.cost = cost;
        this.gate_type = gate_type;
        this.spot_num = spot_num;
    }

    public int getMaxtime(){
        return this.maxtime;
    }

    public boolean canServeFlight(int flight_type) {
        return false;
    }

    public boolean canServePlane(int flight_type) {
        return false;
    }

    public boolean allServices(){
        return false;
    }

}


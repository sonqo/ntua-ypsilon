package app;

public class Station {

    String id;
    int gate_id, spot_num, cost;

    public void setInfo(int gate_id, int spot_num, int cost, String id){
        this.id = id;
        this.cost = cost;
        this.gate_id = gate_id;
        this.spot_num = spot_num;
    }

}

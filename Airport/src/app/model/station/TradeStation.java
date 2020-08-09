package app.model.station;

public class TradeStation extends Station {

    int maxtime = 90;
    String label = "tradeNum";
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }

    @Override
    public boolean canServeFlight(int flight_type){
        int[] flights = {2};
        for (int value : flights){
            if (flight_type == value){
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean canServePlane(int plane_type){
        int[] planes = {2, 3};
        for (int value : planes){
            if (plane_type == value){
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean allServices(){
        return true;
    }
}

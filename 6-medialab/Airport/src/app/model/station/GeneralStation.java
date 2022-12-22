package app.model.station;

public class GeneralStation extends Station {

    int maxtime = 240;
    String state = "Available";

    public int getMaxtime(){
        return this.maxtime;
    }

    @Override
    public boolean canServeFlight(int flight_type){
        int[] flights = {1, 2, 3};
        for (int value : flights){
            if (flight_type == value){
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean canServePlane(int plane_type){
        int[] planes = {1, 2, 3};
        for (int value : planes){
            if (plane_type == value){
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean allServices(){
        return false;
    }
}

package app;

public class Flight {

    String city;
    int flight_id, flight_type, plane_type, min2departure;

    public void setInfo(int flight_id, String city, int flight_type, int plane_type, int min2departure){
        this.city = city;
        this.flight_id = flight_id;
        this.plane_type = plane_type;
        this.flight_type = flight_type;
        this.min2departure = min2departure;
    }

}

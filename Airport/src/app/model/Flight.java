package app.model;

import javafx.scene.layout.AnchorPane;

public class Flight {

    public AnchorPane pos;
    public int flight_type, plane_type, min2departure;
    public String city, flight_id, state = "Unserviced";

    public void setInfo(String flight_id, String city, int flight_type, int plane_type, int min2departure){
        this.city = city;
        this.flight_id = flight_id;
        this.plane_type = plane_type;
        this.flight_type = flight_type;
        this.min2departure = min2departure;
    }

}

package app.model;

import app.model.station.Station;
import javafx.scene.layout.AnchorPane;

public class Flight {

    public AnchorPane pos;
    public Station station;
    public String city, flight_id;
    public int flight_type, plane_type, min2departure;

    public void setInfo(String flight_id, String city, int flight_type, int plane_type, int min2departure){
        this.city = city;
        this.flight_id = flight_id;
        this.plane_type = plane_type;
        this.flight_type = flight_type;
        this.min2departure = min2departure;
    }

}

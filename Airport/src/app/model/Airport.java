package app.model;

import app.model.station.Station;

import java.util.List;
import java.util.ArrayList;

public class Airport {

    double fuel = 0.25, cleaning = 0.02, transport = 0.02, loading = 0.05;

    public List<Flight> flight_list = new ArrayList<Flight>();
    public List<Station> station_list = new ArrayList<Station>();

}

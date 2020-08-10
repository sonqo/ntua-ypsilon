package app.model;

import app.model.station.*;
import javafx.scene.layout.AnchorPane;

import java.util.List;
import java.util.ArrayList;

public class Airport {

    double fuel = 0.25, cleaning = 0.02, transport = 0.02, loading = 0.05;

    public List<Station> stationList = new ArrayList<>();

    public List<Flight> flightListQueue = new ArrayList<>();
    public List<Flight> flightListServiced = new ArrayList<>();

    public List<AnchorPane> gateAnchorList;
    public List<GateStation> gateStationList = new ArrayList<>();

    public List<AnchorPane> tradeAnchorList;
    public List<TradeStation> tradeStationList = new ArrayList<>();

    public List<AnchorPane> zoneAAnchorList;
    public List<ZoneAStation> zoneAStationList = new ArrayList<>();

    public List<AnchorPane> zoneBAnchorList;
    public List<ZoneBStation> zoneBStationList = new ArrayList<>();

    public List<AnchorPane> zoneCAnchorList;
    public List<ZoneCStation> zoneCStationList = new ArrayList<>();

    public List<AnchorPane> generalAnchorList;
    public List<GeneralStation> generalStationList = new ArrayList<>();

    public List<AnchorPane> longTermAnchorList;
    public List<LongTermStation> longTermStationList = new ArrayList<>();

}

package app.model;

import app.model.station.*;
import javafx.scene.layout.AnchorPane;

import java.util.List;
import java.util.ArrayList;

public class Airport {

    double fuel = 0.25, cleaning = 0.02, transport = 0.02, loading = 0.05;

    public List<Flight> flightList = new ArrayList<Flight>();

    public List<GateStation> gateStationList = new ArrayList<GateStation>();
    public List<AnchorPane> gateAnchorList;

    public List<TradeStation> tradeStationList = new ArrayList<TradeStation>();
    public List<AnchorPane> tradeAnchorList;

    public List<ZoneAStation> zoneAStationList = new ArrayList<ZoneAStation>();
    public List<AnchorPane> zoneAAnchorList;

    public List<ZoneBStation> zoneBStationList = new ArrayList<ZoneBStation>();
    public List<AnchorPane> zoneBAnchorList;

    public List<ZoneCStation> zoneCStationList = new ArrayList<ZoneCStation>();
    public List<AnchorPane> zoneCAnchorList;

    public List<GeneralStation> generalStationList = new ArrayList<GeneralStation>();
    public List<AnchorPane> generalAnchorList;

    public List<LongTermStation> longTermStationList = new ArrayList<LongTermStation>();
    public List<AnchorPane> longTermAnchorList;
}

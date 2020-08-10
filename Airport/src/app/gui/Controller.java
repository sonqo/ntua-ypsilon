package app.gui;

import app.model.Flight;
import app.model.Airport;
import app.model.station.*;

import java.awt.*;
import java.io.File;
import java.net.URL;
import java.util.List;
import java.util.Arrays;
import javafx.fxml.FXML;
import javafx.stage.Stage;
import java.io.FileReader;
import java.util.ArrayList;
import java.io.IOException;
import javafx.util.Duration;
import java.io.BufferedReader;
import javafx.stage.FileChooser;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.fxml.Initializable;
import javafx.animation.Animation;
import javafx.scene.control.Label;
import java.net.URISyntaxException;
import javafx.scene.control.MenuBar;
import javafx.scene.layout.AnchorPane;

public class Controller implements Initializable {

    public MenuBar menuBar;
    public Label timerLabel, statusMessage;

    @FXML
    private List<AnchorPane> globalGateAnchorList;
    @FXML
    private List<AnchorPane> globalTradeAnchorList;
    @FXML
    private List<AnchorPane> globalZoneAAnchorList;
    @FXML
    private List<AnchorPane> globalZoneBAnchorList;
    @FXML
    private List<AnchorPane> globalZoneCAnchorList;
    @FXML
    private List<AnchorPane> globalGeneralAnchorList;
    @FXML
    private List<AnchorPane> globalLongTermAnchorList;

    private final List<AnchorPane> validGateAnchorList = new ArrayList<>();
    private final List<AnchorPane> validTradeAnchorList = new ArrayList<>();
    private final List<AnchorPane> validZoneAAnchorList = new ArrayList<>();
    private final List<AnchorPane> validZoneBAnchorList = new ArrayList<>();
    private final List<AnchorPane> validZoneCAnchorList = new ArrayList<>();
    private final List<AnchorPane> validGeneralAnchorList = new ArrayList<>();
    private final List<AnchorPane> validLongTermAnchorList = new ArrayList<>();

    int hours = 0;
    int minutes = 0;
    String defaultSetup = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/setup_SCENARIO-01.txt";
    String defaultAirport = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/airport_SCENARIO-01.txt";

    private final Airport airport = new Airport();

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private void setupState() throws IOException{

        // Reading application state - setting initial GUI
        BufferedReader reader = new BufferedReader(new FileReader(defaultAirport)); // Reading Airport_Scenario
        String line = reader.readLine();
        while (line != null) {
            List<String> rtmp = Arrays.asList(line.split(","));
            if (Integer.parseInt(rtmp.get(0)) == 1) {
                GateStation stat = new GateStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.gateStationList.add(stat);
                for (int i=0; i<airport.gateStationList.get(0).spot_num; i++){
                    validGateAnchorList.add(globalGateAnchorList.remove(0));
                }
                airport.gateAnchorList = validGateAnchorList;
                for (int i=0; i<airport.gateAnchorList.size(); i++){
                    airport.gateAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 2){
                TradeStation stat = new TradeStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.tradeStationList.add(stat);
                for (int i=0; i<airport.tradeStationList.get(0).spot_num; i++){
                    validTradeAnchorList.add(globalTradeAnchorList.remove(0));
                }
                airport.tradeAnchorList = validTradeAnchorList;
                for (int i=0; i<airport.tradeAnchorList.size(); i++){
                    airport.tradeAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 3){
                ZoneAStation stat = new ZoneAStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.zoneAStationList.add(stat);
                for (int i=0; i<airport.zoneAStationList.get(0).spot_num; i++){
                    validZoneAAnchorList.add(globalZoneAAnchorList.remove(0));
                }
                airport.zoneAAnchorList = validZoneAAnchorList;
                for (int i=0; i<airport.zoneAAnchorList.size(); i++){
                    airport.zoneAAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 4){
                ZoneBStation stat = new ZoneBStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.zoneBStationList.add(stat);
                for (int i=0; i<airport.zoneBStationList.get(0).spot_num; i++){
                    validZoneBAnchorList.add(globalZoneBAnchorList.remove(0));
                }
                airport.zoneBAnchorList = validZoneBAnchorList;
                for (int i=0; i<airport.zoneBAnchorList.size(); i++){
                    airport.zoneBAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 5){
                ZoneCStation stat = new ZoneCStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.zoneCStationList.add(stat);
                for (int i=0; i<airport.zoneCStationList.get(0).spot_num; i++){
                    validZoneCAnchorList.add(globalZoneCAnchorList.remove(0));
                }
                airport.zoneCAnchorList = validZoneCAnchorList;
                for (int i=0; i<airport.zoneCAnchorList.size(); i++){
                    airport.zoneCAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 6){
                GeneralStation stat = new GeneralStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.generalStationList.add(stat);
                for (int i=0; i<airport.generalStationList.get(0).spot_num; i++){
                    validGeneralAnchorList.add(globalGeneralAnchorList.remove(0));
                }
                airport.generalAnchorList = validGeneralAnchorList;
                for (int i=0; i<airport.generalAnchorList.size(); i++){
                    airport.generalAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            else if (Integer.parseInt(rtmp.get(0)) == 7){
                LongTermStation stat = new LongTermStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.longTermStationList.add(stat);
                for (int i=0; i<airport.longTermStationList.get(0).spot_num; i++){
                    validLongTermAnchorList.add(globalLongTermAnchorList.remove(0));
                }
                airport.longTermAnchorList = validLongTermAnchorList;
                for (int i=0; i<airport.longTermAnchorList.size(); i++){
                    airport.longTermAnchorList.get(i).setStyle("-fx-background-color: seagreen; -fx-border-color: white");
                }
            }
            line = reader.readLine();
        }
        reader.close();

        reader = new BufferedReader(new FileReader(defaultSetup)); // Reading Setup_Scenario
        line = reader.readLine();
        while (line != null){
            List<String> rtmp = Arrays.asList(line.split(","));
            Flight flt = new Flight(); // for each file line, create a Flight instance
            flt.setInfo(rtmp.get(0), rtmp.get(1), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)), Integer.parseInt(rtmp.get(4)));
            airport.flightListQueue.add(flt);
            line = reader.readLine();
        }
        reader.close();

        // Serving initial flights
        for (int i=0; i<airport.flightListQueue.size(); i++){
            if (!airport.gateStationList.isEmpty()){
                if ((airport.gateStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.gateStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.gateAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.gateAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.gateStationList.remove(0);
                }
            }
            else if (!airport.tradeStationList.isEmpty()){
                if ((airport.tradeStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.tradeStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.tradeAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.tradeAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.tradeStationList.remove(0);
                }
            }
            else if (!airport.zoneAStationList.isEmpty()){
                if ((airport.zoneAStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.zoneAStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.zoneAAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.zoneAAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.zoneAStationList.remove(0);
                }
            }
            else if (!airport.zoneBStationList.isEmpty()){
                if ((airport.zoneBStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.zoneBStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.zoneBAnchorList.get(0).setStyle("-fx-background-color: palevioletred; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.zoneBAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.zoneBStationList.remove(0);
                }
            }
            else if (!airport.zoneCStationList.isEmpty()){
                if ((airport.zoneCStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.zoneCStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.zoneCAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.zoneCAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.zoneCStationList.remove(0);
                }
            }
            else if (!airport.generalStationList.isEmpty()){
                if ((airport.generalStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.generalStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.generalAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.generalAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.generalStationList.remove(0);
                }
            }
            else if (!airport.longTermStationList.isEmpty()){
                if ((airport.longTermStationList.get(0).canServeFlight(airport.flightListQueue.get(i).flight_type))
                        && (airport.longTermStationList.get(0).canServePlane(airport.flightListQueue.get(i).plane_type))) {
                    airport.flightListServiced.add(airport.flightListQueue.get(i));
                    airport.longTermAnchorList.get(0).setStyle("-fx-background-color: crimson; -fx-border-color: white");
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).pos = airport.longTermAnchorList.remove(0);
                    airport.flightListServiced.get(airport.flightListServiced.size()-1).station = airport.longTermStationList.remove(0);
                }
            }
        }
        airport.flightListQueue.removeAll(airport.flightListServiced);

        String msg;
        if (!airport.flightListQueue.isEmpty()){
            msg = "Status: Holding flights ";
            for (int i=0; i<airport.flightListQueue.size(); i++){
                msg += airport.flightListQueue.get(i).flight_id;
                msg += " ";
            }
            statusMessage.textProperty().set(msg);
        }

        statusMessage.textProperty().set("Status: Airport state initialized, initial flights serviced"); // setting status message

    }

    private void scheduleDeparture(){
        for (int i=0; i<airport.flightListServiced.size(); i++){
            Flight curr = airport.flightListServiced.get(i);
            Timeline timeline = new Timeline(new KeyFrame(
                    Duration.millis(curr.min2departure*1000),
                    ae -> departFlights(curr)));
            timeline.play();
        }
    }

    private void departFlights(Flight flight){
        if (flight.station instanceof GateStation){
            airport.gateStationList.add((GateStation) flight.station);
            airport.gateAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof TradeStation){
            airport.tradeStationList.add((TradeStation) flight.station);
            airport.tradeAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof ZoneAStation){
            airport.zoneAStationList.add((ZoneAStation) flight.station);
            airport.zoneAAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof ZoneBStation){
            airport.zoneBStationList.add((ZoneBStation) flight.station);
            airport.zoneBAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof ZoneCStation){
            airport.zoneCStationList.add((ZoneCStation) flight.station);
            airport.zoneCAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof GeneralStation){
            airport.generalStationList.add((GeneralStation) flight.station);
            airport.generalAnchorList.add(flight.pos);
        }
        else if (flight.station instanceof LongTermStation){
            airport.longTermStationList.add((LongTermStation) flight.station);
            airport.longTermAnchorList.add(flight.pos);
        }
        flight.pos.setStyle("-fx-background-color: seagreen; -fx-border-color: white");
        airport.flightListServiced.remove(flight);
    }

    private void runTimer(String name){
        minutes ++;
        if (minutes == 60){
            minutes = 0;
            hours++;
        }
        if (hours == 24){
            minutes = 0;
            hours = 0;
        }
        timerLabel.textProperty().set(name + String.format("%02d:%02d", hours, minutes));
    }

    public void startApp(ActionEvent actionEvent) throws IOException { // Start button
        hours = 0;
        minutes = 0;
        String defaultTimer = timerLabel.textProperty().get(); // setup timer
        timerLabel.textProperty().set(defaultTimer + "00:00");
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.millis(5000),
                ae -> runTimer(defaultTimer)));
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        setupState(); // read setup files, setup airport state
        scheduleDeparture();
    }

    public void loadApp(ActionEvent actionEvent) { // Load button
        FileChooser fc = new FileChooser();
        fc.setTitle("Select Setup-Scenario");
        fc.setInitialDirectory(new File("C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab"));
        fc.getExtensionFilters().add(new FileChooser.ExtensionFilter("TXT Files", "*.txt"));
        String f = fc.showOpenDialog(null).toString();
        defaultSetup = f;
        fc.setTitle("Select Airport-Scenario");
        fc.getExtensionFilters().add(new FileChooser.ExtensionFilter("TXT Files", "*.txt"));
        f = fc.showOpenDialog(null).toString();
        defaultAirport = f;
    }

    public void aboutApp() throws IOException, URISyntaxException {
        Desktop.getDesktop().browse(new URL("https://github.com/Sonqo/Ntua_Lambda/tree/master/6_MediaLab/Airport").toURI());
    }

    public void exitApp(ActionEvent actionEvent) { // Exit button
        Stage stage = (Stage) menuBar.getScene().getWindow();
        stage.close();
    }

}

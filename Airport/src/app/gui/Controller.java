package app.gui;

import app.model.Flight;
import app.model.Airport;
import app.model.station.*;

import java.io.File;
import java.net.URL;
import java.util.List;
import java.util.Arrays;
import javafx.stage.Stage;
import java.io.FileReader;
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
import javafx.scene.control.MenuBar;

public class Controller implements Initializable {

//    Station stat = null;
    public MenuBar menuBar;
    public Label timerLabel, gateNum, tradeNum, zoneANum, zoneBNum, zoneCNum, generalNum, longNum;

    int hours = 0;
    int minutes = 0;
    String defaultSetup = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/setup_SCENARIO-01.txt";
    String defaultAirport = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/airport_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private void setupState() throws IOException{

        Airport airport = new Airport();

        // Reading application state
        BufferedReader reader = new BufferedReader(new FileReader(defaultAirport)); // Reading Airport_Scenario
        String line = reader.readLine();
        while (line != null) {
            List<String> rtmp = Arrays.asList(line.split(","));
            if (Integer.parseInt(rtmp.get(0)) == 1) {
                GateStation stat = new GateStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 2){
                TradeStation stat = new TradeStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 3){
                ZoneAStation stat = new ZoneAStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 4){
                ZoneBStation stat = new ZoneBStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 5){
                ZoneCStation stat = new ZoneCStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 6){
                GeneralStation stat = new GeneralStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
            }
            else if (Integer.parseInt(rtmp.get(0)) == 7){
                LongTermStation stat = new LongTermStation();
                stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
                airport.station_list.add(stat);
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
            airport.flight_list.add(flt);
            line = reader.readLine();
        }
        reader.close();

        // Setting up GUI
        for (int i=0; i<airport.station_list.size(); i++){
            if (airport.station_list.get(i).gate_type == 1){
                gateNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 2){
                tradeNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 3){
                zoneANum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 4){
                zoneBNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 5){
                zoneCNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 6){
                generalNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
            else if (airport.station_list.get(i).gate_type == 7){
                longNum.textProperty().set(String.valueOf(airport.station_list.get(i).spot_num));
            }
        }
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

        setupState(); // setup files

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

    public void exitApp(ActionEvent actionEvent) { // Exit button
        Stage stage = (Stage) menuBar.getScene().getWindow();
        stage.close();
    }

}

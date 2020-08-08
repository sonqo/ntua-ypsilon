package app.gui;

import app.model.Flight;
import app.model.Airport;
import app.model.Station;

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

    public MenuBar menuBar;
    public Label timerLabel;

    int hours = 0;
    int minutes = 0;

    String defaultSetup = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/setup_SCENARIO-01.txt";
    String defaultAirport = "C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/airport_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

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

        // Timer handling
        String defaultTimer = timerLabel.textProperty().get();
        timerLabel.textProperty().set(defaultTimer + "00:00");
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.millis(5000),
                ae -> runTimer(defaultTimer)));
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        // Setting application state
        Airport airport = new Airport(); // create Airport instance

        BufferedReader reader = new BufferedReader(new FileReader(defaultAirport));
        String line = reader.readLine();
        while (line != null){
            List<String> rtmp = Arrays.asList(line.split(","));
            Station stat = new Station(); // for each file line, create a Station instance
            stat.setInfo(Integer.parseInt(rtmp.get(0)), Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), rtmp.get(3));
            airport.station_list.add(stat); // append Station class to Airport
            line = reader.readLine();

        }
        reader.close();

        // Reading Setup_Scenario
        reader = new BufferedReader(new FileReader(defaultSetup));
        line = reader.readLine();
        while (line != null){
            List<String> rtmp = Arrays.asList(line.split(","));
            Flight flt = new Flight(); // for each file line, create a Flight instance
            flt.setInfo(rtmp.get(0), rtmp.get(1), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)), Integer.parseInt(rtmp.get(4)));
            airport.flight_list.add(flt); // append Flight class to Airport
            line = reader.readLine();
        }
        reader.close();
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

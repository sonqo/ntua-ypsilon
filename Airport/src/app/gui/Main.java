package app.gui;

import app.model.Flight;
import app.model.Airport;
import app.model.Station;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.Parent;
import javafx.fxml.FXMLLoader;
import javafx.scene.image.Image;
import javafx.application.Application;

import java.util.List;
import java.util.Arrays;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("MediaLab Airport");
        primaryStage.getIcons().add(new Image("file:/C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/images/stackoverflow_v1.png"));
        primaryStage.setScene(new Scene(root, 1024, 624));
        primaryStage.show();
    }
    
    public static void main(String[] args) throws IOException {

        launch(args);

        Airport airport = new Airport(); // create Airport instance

        // Reading Airport_Scenario
        BufferedReader reader = new BufferedReader(new FileReader("C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/airport_SCENARIO-01.txt"));
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
        reader = new BufferedReader(new FileReader("C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/medialab/setup_SCENARIO-01.txt"));
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
}

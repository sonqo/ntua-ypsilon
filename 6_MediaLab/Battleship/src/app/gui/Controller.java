package app.gui;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.MenuItem;

import app.model.ship.*;

public class Controller implements Initializable {

    List<Ship> enemy = new ArrayList<>();
    List<Ship> player = new ArrayList<>();

    public MenuItem startGame;
    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    public List readInput(String path) throws IOException{
        List<Ship> ship_list = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(path));
        String line = reader.readLine();
        while (line != null) {
            List<String> rtmp = Arrays.asList(line.split(","));
            if (Integer.parseInt(rtmp.get(0)) == 1) {
                Carrier curr = new Carrier();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_list.add(curr);
            } else if (Integer.parseInt(rtmp.get(0)) == 2) {
                Battleship curr = new Battleship();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_list.add(curr);
            } else if (Integer.parseInt(rtmp.get(0)) == 3) {
                Cruiser curr = new Cruiser();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_list.add(curr);
            } else if (Integer.parseInt(rtmp.get(0)) == 4) {
                Submarine curr = new Submarine();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_list.add(curr);
            } else if (Integer.parseInt(rtmp.get(0)) == 5) {
                Destroyer curr = new Destroyer();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_list.add(curr);
            }
            line = reader.readLine();
        }
        return ship_list;
    }

    public void startApp(ActionEvent actionEvent) throws IOException, InterruptedException {

        enemy = readInput(defaultEnemy); // reading enemy's scenario
        player = readInput(defaultPlayer); // reading player's scenario

        

    }

}

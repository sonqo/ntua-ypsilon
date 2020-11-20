package app.gui;

import app.model.ship.*;

import java.net.URL;
import java.util.List;
import javafx.fxml.FXML;
import java.util.Arrays;
import java.io.FileReader;
import java.util.ArrayList;
import java.io.IOException;
import java.io.BufferedReader;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.AnchorPane;

public class Controller implements Initializable {

    private List<Ship> enemy = new ArrayList<>();
    private List<Ship> player = new ArrayList<>();

    @FXML
    private List<AnchorPane> playerBoard;

    public MenuItem startGame;
    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private List readInput(String path) throws IOException{
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

        for (int i=0; i<playerBoard.size(); i++){
            playerBoard.get(i).setStyle("-fx-background-color: royalblue; -fx-border-color: white");
        }

    }

}

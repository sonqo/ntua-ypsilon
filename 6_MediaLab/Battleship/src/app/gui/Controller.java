package app.gui;

import app.model.ship.*;
import app.model.player.Player;

import java.util.*;
import java.net.URL;
import javafx.fxml.FXML;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.AnchorPane;

public class Controller implements Initializable {

    public Player enemy = new Player();
    public Player player = new Player();

    @FXML
    public List<AnchorPane> enemyBoard;
    @FXML
    public List<AnchorPane> playerBoard;

    public MenuItem startGame;
    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private Ship[] readInput(String path) throws IOException{
        Ship[] ship_array = new Ship[6];
        BufferedReader reader = new BufferedReader(new FileReader(path));
        String line = reader.readLine();
        while (line != null) {
            List<String> rtmp = Arrays.asList(line.split(","));
            if (Integer.parseInt(rtmp.get(0)) == 1) {
                Carrier curr = new Carrier();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_array[Integer.parseInt(rtmp.get(0))] = curr;
            } else if (Integer.parseInt(rtmp.get(0)) == 2) {
                Battleship curr = new Battleship();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_array[Integer.parseInt(rtmp.get(0))] = curr;
            } else if (Integer.parseInt(rtmp.get(0)) == 3) {
                Cruiser curr = new Cruiser();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_array[Integer.parseInt(rtmp.get(0))] = curr;
            } else if (Integer.parseInt(rtmp.get(0)) == 4) {
                Submarine curr = new Submarine();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_array[Integer.parseInt(rtmp.get(0))] = curr;
            } else if (Integer.parseInt(rtmp.get(0)) == 5) {
                Destroyer curr = new Destroyer();
                curr.setStats(Integer.parseInt(rtmp.get(1)), Integer.parseInt(rtmp.get(2)), Integer.parseInt(rtmp.get(3)));
                ship_array[Integer.parseInt(rtmp.get(0))] = curr;
            }
            line = reader.readLine();
        }
        return ship_array;
    }

    public void startApp(ActionEvent actionEvent) throws IOException, InterruptedException {

        enemy.ship_array = readInput(defaultEnemy); // reading enemy's scenario
        player.ship_array = readInput(defaultPlayer); // reading player's scenario

        for (int i=0; i<playerBoard.size(); i++){ // painting see board
            enemyBoard.get(i).setStyle("-fx-background-color: #87BDD8; -fx-border-color: white");
            playerBoard.get(i).setStyle("-fx-background-color: #CFE0E8; -fx-border-color: white");
        }

        for (int i=1; i<6; i++) { // register player's ships position, create player's flag-board
            enemy.ship_array[i].fillAnchorList(enemyBoard, enemy, 0);
            player.ship_array[i].fillAnchorList(playerBoard, player, 1);
        }

    }

}

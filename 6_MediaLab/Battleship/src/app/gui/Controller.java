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
import javafx.scene.control.Alert;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.AnchorPane;
import javafx.scene.control.TextInputDialog;

public class Controller implements Initializable {

    @FXML
    public List<AnchorPane> enemyBoard;
    @FXML
    public List<AnchorPane> playerBoard;

    public MenuItem startGame;
    public Label playersPoints, enemysPoints, playersShips, enemysShips, playersShots, enemysShots;

    public Player enemy = new Player();
    public Player player = new Player();
    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private void checkInput(String path, Player user) throws IOException {

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

    public void handleLabels() {
        enemysPoints.textProperty().set("Enemy's Points: " + enemy.getPoints());
        enemysShips.textProperty().set("Enemy's Functional Ships: " + enemy.getShip_count());
        enemysShots.textProperty().set("Enemy's Successful Shots: " + enemy.getShot_count());

        playersPoints.textProperty().set("Player's Points: " + player.getPoints());
        playersShips.textProperty().set("Player's Functional Ships: " + player.getShip_count());
        playersShots.textProperty().set("Player's Successful Shots: " + player.getShot_count());

    }

    public void startApp(ActionEvent actionEvent) throws IOException {

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

        handleLabels();

    }

    public void loadApp(){
        TextInputDialog dialog = new TextInputDialog();
        dialog.setHeaderText(null);
        dialog.setContentText("ID: ");
        dialog.setTitle("Load Scenario");
        Optional<String> res = dialog.showAndWait();
        if (res.isPresent()) {
            try {
                FileReader reader1 = new FileReader("medialab/enemy_SCENARIO-" + res.get() + ".txt");
                FileReader reader2 = new FileReader("medialab/enemy_SCENARIO-" + res.get() + ".txt");
                defaultEnemy = "medialab/enemy_SCENARIO-" + res.get() + ".txt";
                defaultPlayer = "medialab/player_SCENARIO-" + res.get() + ".txt";
                startApp(null);
            } catch (Exception e) {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setHeaderText(null);
                alert.setTitle("Load Scenario");
                alert.setContentText("Scenario ID not found!");
                alert.showAndWait();
            }
        }
    }

    public void exitApp() {
        System.exit(0);
    }

}

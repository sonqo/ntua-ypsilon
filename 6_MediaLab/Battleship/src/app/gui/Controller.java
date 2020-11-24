package app.gui;

import app.model.Shot;
import app.model.ship.*;
import app.model.player.Player;

import java.awt.*;
import java.util.*;
import java.net.URL;
import java.util.List;

import javafx.beans.property.BooleanProperty;
import javafx.fxml.FXML;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import java.net.URISyntaxException;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import javafx.scene.control.TextInputDialog;

public class Controller implements Initializable {

    @FXML
    public List<AnchorPane> enemyBoard;
    @FXML
    public List<AnchorPane> playerBoard;

    public Button fireButton;
    public MenuItem startGame;
    public TextField xCoord, yCoord;
    public Label playersPoints, enemysPoints, playersShips, enemysShips, playersShots, enemysShots;

    public Player enemy;
    public Player player;

    private int x, y;

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
        enemysShips.textProperty().set("Enemy's Functional Ships: " + enemy.getFunctional_ships());
        enemysShots.textProperty().set("Enemy's Successful Shots: " + enemy.getSuccessful_shots());

        playersPoints.textProperty().set("Player's Points: " + player.getPoints());
        playersShips.textProperty().set("Player's Functional Ships: " + player.getFunctional_ships());
        playersShots.textProperty().set("Player's Successful Shots: " + player.getSuccessful_shots());
    }

    public void handleSubmit() {

        x = Integer.parseInt(xCoord.textProperty().get());
        y = Integer.parseInt(yCoord.textProperty().get());

        Shot curr = new Shot(x, y, player.getTurn());

        if (enemy.board[x][y] == 0) {

            curr.ship = 0;
            curr.status = "Miss";
            enemyBoard.get(10*(x-1) + (y-1)).setStyle("-fx-background-color:#3b444b; -fx-border-color: white");

        } else if (enemy.board[x][y] > 0) {


            curr.status = "Hit";
            curr.ship = enemy.board[x][y];

            if (enemy.ship_array[enemy.board[x][y]].getState().equals("Intact")) {
                enemy.ship_array[enemy.board[x][y]].setSize();
                enemy.ship_array[enemy.board[x][y]].setDamaged();
                player.setSuccessful_shots();
                player.setPoints(enemy.ship_array[enemy.board[x][y]].getHit_points());

            } else if (enemy.ship_array[enemy.board[x][y]].getState().equals("Damaged")) {
                if (enemy.ship_array[enemy.board[x][y]].getSize() > 1) {
                    enemy.ship_array[enemy.board[x][y]].setSize();
                    player.setPoints(enemy.ship_array[enemy.board[x][y]].getHit_points());

                } else {
                    enemy.setFunctional_ships();
                    enemy.ship_array[enemy.board[x][y]].setSunken();
                    player.setPoints(enemy.ship_array[enemy.board[x][y]].getDestroy_bonus());
                }
                player.setSuccessful_shots();
            }

            enemy.board[x][y] = -1;
            enemyBoard.get(10*(x-1) + (y-1)).setStyle("-fx-background-color:crimson; -fx-border-color: white");

        } else {
            curr.ship = 0;
            curr.status = "Miss";
        }
        player.shots.add(curr);

        xCoord.clear();
        yCoord.clear();

        player.turn += 1;

        handleLabels();

        if (enemy.getFunctional_ships() == 4) {
            gameOver();
        }

    }

    public void startApp(ActionEvent actionEvent) throws IOException {

        enemy = new Player();
        player = new Player();

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

    public void gameOver() {
        xCoord.setEditable(false);
        yCoord.setEditable(false);
        fireButton.setDisable(true);
    }

    public void enemyShipsHandler() {
        Alert dialog = new Alert(Alert.AlertType.INFORMATION);
        String curr = "Carrier:\t\t\t\t" + enemy.ship_array[1].getState() + "\nBattleship:\t\t\t" + enemy.ship_array[2].getState() +
                "\nCruiser:\t\t\t\t" + enemy.ship_array[3].getState() + "\nSubmarine:\t\t\t" + enemy.ship_array[4].getState() + "\nDestroyer:\t\t\t" + enemy.ship_array[5].getState();
        dialog.setHeaderText(null);
        dialog.setContentText(curr);
        dialog.setTitle("Enemy's Ships Info");
        dialog.showAndWait();
    }

    public void playerShotsHandler() {
        Shot curr;
        String string = "";
        for (int i=0; i<player.shots.size(); i++) {
            if (i == 5) {
                break;
            }
            curr = player.shots.get(player.shots.size()-i-1);
            string += "Turn: " + curr.getTurn() + "\t\t(" + curr.getX() + "," + curr.getY() + ")\t\t\t"  + curr.getStatus() + "\t\t\t" + curr.getShip();
            string += "\n";
        }
        Alert dialog = new Alert(Alert.AlertType.INFORMATION);
        dialog.setHeaderText(null);
        dialog.setContentText(string);
        dialog.setTitle("Player's Last 5 Shots Info");
        dialog.showAndWait();
    }

    public void enemyShotsHandler() {
        Shot curr;
        String string = "";
        for (int i=0; i<enemy.shots.size(); i++) {
            if (i == 5) {
                break;
            }
            curr = enemy.shots.get(enemy.shots.size()-i-1);
            string += "Turn: " + curr.getTurn() + "\t\t(" + curr.getX() + "," + curr.getY() + ")\t\t\t"  + curr.getStatus() + "\t\t\t" + curr.getShip();
            string += "\n";
        }
        Alert dialog = new Alert(Alert.AlertType.INFORMATION);
        dialog.setHeaderText(null);
        dialog.setContentText(string);
        dialog.setTitle("Enemy's Last 5 Shots Info");
        dialog.showAndWait();
    }

    public void aboutApp() throws IOException, URISyntaxException {
        Desktop.getDesktop().browse(new URL("https://github.com/Sonqo/Ntua_Ypsilon/tree/master/6_MediaLab/Battleship").toURI());
    }

    public void exitApp() {
        System.exit(0);
    }

}

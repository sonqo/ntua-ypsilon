package app.gui;

import app.model.ship.*;
import app.model.Player;
import app.model.exception.InvalidCountException;

import java.awt.*;
import java.util.*;
import java.net.URL;
import java.util.List;
import javafx.fxml.FXML;
import javafx.stage.Stage;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;
import java.text.DecimalFormat;
import javafx.scene.image.Image;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.Label;
import javafx.scene.control.Button;
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
    public Label playersPoints, enemysPoints, playersShips, enemysShips, playersShots, enemysShots, turnLabel;

    public Player player, enemy;

    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    private static DecimalFormat df = new DecimalFormat("##.##");

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private Ship[] readInput(String path) throws IOException{
        int[] acc = new int [6];
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
            acc[Integer.parseInt(rtmp.get(0))]++;
            try {
                if (acc[Integer.parseInt(rtmp.get(0))] == 2) {
                    throw new InvalidCountException();
                }
            } catch (InvalidCountException e) {
                e.handleException();
            }
            line = reader.readLine();
        }
        return ship_array;
    }

    public void handleLabels() {

        turnLabel.textProperty().set("Game Turn: " + player.getTurn());

        double percentage_enemy;
        double percentage_player;
        if (player.getShot_count() == 0) {
            percentage_player = 0.0;
        } else {
            percentage_player = ((double)player.getSuccessful_shots()/player.getShot_count())*100;
        }
        if (enemy.getShot_count() == 0) {
            percentage_enemy = 0.0;
        } else {
            percentage_enemy = ((double)enemy.getSuccessful_shots()/enemy.getShot_count())*100;
        }

        enemysPoints.textProperty().set("Enemy's Points: " + enemy.getPoints());
        enemysShips.textProperty().set("Enemy's Functional Ships: " + enemy.getFunctional_ships());
        enemysShots.textProperty().set("Enemy's Successful Shots: " + df.format(percentage_enemy) + "%");

        playersPoints.textProperty().set("Player's Points: " + player.getPoints());
        playersShips.textProperty().set("Player's Functional Ships: " + player.getFunctional_ships());
        playersShots.textProperty().set("Player's Successful Shots: " + df.format(percentage_player) + "%");
    }

    public void handleSubmit() {

        int x = Integer.parseInt(xCoord.textProperty().get());
        int y = Integer.parseInt(yCoord.textProperty().get());

        player.turnHandler(enemy, enemyBoard, x, y);

        handleLabels();

        xCoord.clear();
        yCoord.clear();

        if (enemy.getFunctional_ships() == 0) {
            gameOver(1);
        } else if ((player.getTurn() == 41) && (player.getTurn() == enemy.getTurn())){
            enemy.turn = 0;
            gameOver(3);
        } else {
            enemysTurn();
        }
    }

    public void enemysTurn() {

        int[] coords = enemy.prophetFunction();

        enemy.turnHandler(player, playerBoard, coords[0], coords[1]);

        handleLabels();

        if (player.getFunctional_ships() == 0) {
            gameOver(2);
        } else if ((enemy.getTurn() == 41) && (enemy.getTurn() == player.getTurn())) {
            player.turn = 0;
            gameOver(3);
        }
    }

    public void gameOver(int state) {

        xCoord.setEditable(false);
        yCoord.setEditable(false);
        fireButton.setDisable(true);

        Alert dialog = new Alert(Alert.AlertType.WARNING);
        dialog.setHeaderText(null);
        dialog.setTitle("Game Over");
        Stage stage = (Stage) dialog.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));

        if (state == 1) {
            dialog.setContentText("Player wins!");
        } else if (state == 2) {
            dialog.setContentText("Enemy wins");
        } else {
            if (player.getPoints() > enemy.getPoints()) {
                dialog.setContentText("Limit of turns reached, player wins!");
            } else if (player.getPoints() < enemy.getPoints()) {
                dialog.setContentText("Limit of turns reached, enemy wins!");
            } else {
                dialog.setContentText("Limit of turns reached, tie!");
            }
        }
        dialog.showAndWait();
    }

    public void initializeApp(ActionEvent actionEvent) throws IOException { // Menu -> Application -> Start Game

        enemy = new Player("Enemy");
        player = new Player("Player");

        xCoord.setEditable(true);
        yCoord.setEditable(true);
        fireButton.setDisable(false);

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

        Random rand = new Random();
        int t = rand.nextInt(2);

        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setHeaderText(null);
        alert.setTitle("Start Game");
        Stage stage = (Stage) alert.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));

        if (t == 0) { //player's turn
            alert.setContentText("Player will go first.");
            alert.showAndWait();
        } else { // enemy's turn
            alert.setContentText("Enemy will go first.");
            alert.showAndWait();
            enemysTurn();
        }
    }

    public void loadApp(){ // Menu -> Application -> Load Scenarios
        TextInputDialog dialog = new TextInputDialog();
        dialog.setHeaderText(null);
        dialog.setContentText("ID: ");
        dialog.setTitle("Load Scenario");
        Stage stage = (Stage) dialog.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));
        Optional<String> res = dialog.showAndWait();
        if (res.isPresent()) {
            try {
                FileReader reader1 = new FileReader("medialab/enemy_SCENARIO-" + res.get() + ".txt");
                FileReader reader2 = new FileReader("medialab/enemy_SCENARIO-" + res.get() + ".txt");
                defaultEnemy = "medialab/enemy_SCENARIO-" + res.get() + ".txt";
                defaultPlayer = "medialab/player_SCENARIO-" + res.get() + ".txt";
                initializeApp(null);
            } catch (Exception e) {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setHeaderText(null);
                alert.setTitle("Load Scenario");
                alert.setContentText("Scenario ID not found!");
                stage = (Stage) alert.getDialogPane().getScene().getWindow();
                stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));
                alert.showAndWait();
            }
        }
    }

    public void exitApp() { // Menu -> Application -> Quit Game
        System.exit(0);
    }

    public void enemyShipsInfoHandler() { // Menu -> Details -> Enemy Ships
        enemy.shipsInfoHandler();
    }

    public void playerShotsInfoHandler() { // Menu -> Details -> Player Shots
        player.shotsInfoHandler();
    }

    public void enemyShotsInfoHandler() { // Menu -> Details -> Enemy Shots
        enemy.shotsInfoHandler();
    }

    public void aboutApp() throws IOException, URISyntaxException { // Menu -> About
        Desktop.getDesktop().browse(new URL("https://github.com/Sonqo/Ntua_Ypsilon/tree/master/6_MediaLab/Battleship").toURI());
    }

}

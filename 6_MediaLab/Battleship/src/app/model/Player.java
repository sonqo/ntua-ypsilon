package app.model;

import app.model.ship.Ship;

import java.util.List;
import java.util.Random;
import javafx.stage.Stage;
import java.util.LinkedList;
import javafx.scene.image.Image;
import javafx.scene.control.Alert;
import javafx.scene.layout.AnchorPane;

public class Player {

    public Ship[] ship_array = new Ship[6];

    public LinkedList<Shot> shots = new LinkedList();
    public LinkedList<Coords> prophet = new LinkedList<>();

    public String name;
    public int[][] board = new int[11][11];
    public int[][] tries = new int[11][11];
    public int turn, points, functional_ships, successful_shots, shot_count;

    public Player(String name) {
        turn = 1;
        points = 0;
        shot_count = 0;
        successful_shots = 0;
        functional_ships = 5;
        this.name = name;
    }

    public int getPoints() {
        return points;
    }

    public void setPoints(int points) {
        this.points += points;
    }

    public String getName() {
        return name;
    }

    public int getFunctional_ships() {
        return functional_ships;
    }

    public void setFunctional_ships() {
        this.functional_ships--;
    }

    public int getSuccessful_shots() {
        return successful_shots;
    }

    public void setSuccessful_shots() {
        this.successful_shots++;
    }

    public int getTurn() {
        return turn;
    }

    public int[] prophetFunction() {

        int x, y;
        int[] coords = new int[2];

        if (prophet.isEmpty()) {
            Random rand = new Random();
            x = rand.nextInt(10) + 1;
            y = rand.nextInt(10) + 1;
            while (tries[x][y] != 0) {
                x = rand.nextInt(10) + 1;
                y = rand.nextInt(10) + 1;
            }
        } else {
            Coords curr = prophet.removeLast();
            x = curr.x;
            y = curr.y;
        }

        coords[0] = x;
        coords[1] = y;
        tries[x][y] = 1;

        return coords;
    }

    public void turnHandler(Player opponent, List<AnchorPane> opponentBoard, int x, int y) {

        Shot curr = new Shot(x, y, getTurn()); // create a current shot

        if (opponent.board[x][y] == 0) { // missed shot
            curr.ship = 0;
            curr.status = "Miss";
            opponentBoard.get(10 * (x - 1) + (y - 1)).setStyle("-fx-background-color:#3b444b; -fx-border-color: white");
        } else if (opponent.board[x][y] > 0) { // successful shot
            curr.status = "Hit";
            curr.ship = opponent.board[x][y];

            if (getName().equals("Enemy")) { // potential successful shots for the "AI"
                int startPosX = (x - 1 < 0) ? x : x-1;
                int startPosY = (y - 1 < 0) ? y : y-1;
                int endPosX =   (x + 1 > 10) ? x : x+1;
                int endPosY =   (y + 1 > 10) ? y : y+1;
                for (int i=startPosX; i<=endPosX; i++) {
                    for (int j=startPosY; j<=endPosY; j++) {
                        if ((i == x) || (j == y)){
                            if (tries[i][j] != 1) {
                                Coords c = new Coords(i, j);
                                prophet.add(c);
                            }
                        }
                    }
                }
            }

            if (opponent.ship_array[opponent.board[x][y]].getState().equals("Intact")) {
                opponent.ship_array[opponent.board[x][y]].setSize();
                opponent.ship_array[opponent.board[x][y]].setDamaged();
                setSuccessful_shots();
                setPoints(opponent.ship_array[opponent.board[x][y]].getHit_points());
            } else if (opponent.ship_array[opponent.board[x][y]].getState().equals("Damaged")) {
                if (opponent.ship_array[opponent.board[x][y]].getSize() > 1) {
                    opponent.ship_array[opponent.board[x][y]].setSize();
                    setPoints(opponent.ship_array[opponent.board[x][y]].getHit_points());
                } else {
                    if (getName().equals("Enemy")) {
                        while (!prophet.isEmpty()) {
                            prophet.pop();
                        }
                    }
                    opponent.setFunctional_ships();
                    opponent.ship_array[opponent.board[x][y]].setSunken();
                    setPoints(opponent.ship_array[opponent.board[x][y]].getHit_points());
                    setPoints(opponent.ship_array[opponent.board[x][y]].getDestroy_bonus());
                }
                setSuccessful_shots();
            }
            opponent.board[x][y] = -1;
            opponentBoard.get(10*(x -1) + (y -1)).setStyle("-fx-background-color:crimson; -fx-border-color: white");
        } else {
            curr.ship = 0;
            curr.status = "Miss";
        }

        shots.add(curr); // save current shot
        turn += 1;
    }


    public void shipsInfoHandler() {
        Alert dialog = new Alert(Alert.AlertType.INFORMATION);
        String curr = "Carrier:\t\t\t\t" + ship_array[1].getState() + "\nBattleship:\t\t\t" + ship_array[2].getState() + "\nCruiser:\t\t\t\t" + ship_array[3].getState() +
                "\nSubmarine:\t\t\t" + ship_array[4].getState() + "\nDestroyer:\t\t\t" + ship_array[5].getState();
        dialog.setHeaderText(null);
        dialog.setContentText(curr);
        dialog.setTitle(getName() + "'s Ships Info");
        Stage stage = (Stage) dialog.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));
        dialog.showAndWait();
    }

    public void shotsInfoHandler() {
        Shot curr;
        StringBuilder string = new StringBuilder();
        for (int i=0; i<shots.size(); i++) {
            if (i == 5) {
                break;
            }
            curr = shots.get(shots.size()-i-1);
            string.append(String.format("Turn: %02d %10s (%02d,%02d) %10s %4s %20s\n", curr.getTurn(), "", curr.getX(), curr.getY(), "", curr.getStatus(), curr.getShip()));
        }
        Alert dialog = new Alert(Alert.AlertType.INFORMATION);
        dialog.setHeaderText(null);
        dialog.setContentText(string.toString());
        dialog.setTitle(getName() + "'s Last 5 Shots Info");
        Stage stage = (Stage) dialog.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));
        dialog.showAndWait();
    }

}

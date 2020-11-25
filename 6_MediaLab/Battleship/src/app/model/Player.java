package app.model;

import app.model.ship.Ship;

import java.util.List;
import java.util.Random;
import javafx.stage.Stage;
import java.util.LinkedList;
import javafx.scene.image.Image;
import javafx.scene.control.Alert;
import javafx.scene.layout.AnchorPane;

/** @author Efstratios "Sonqo" Karipiadis
 *
 * Player class, modeling each user of the Battleship Application.
 *
 * - Each player has unique name, points, functional ships, successful shots and turn.
 * - A ship_array is used to save all unique ship classes for each player, with IDs ranging from 1-5, containing
 * information on their coordinates, hit points, destroy bonus points, status and undamaged areas.
 * - A LinkedList shots accumulates all shots made by the player, each one of them containing respective information on
 * coordinates, status and ship target.
 * - The LinkedList prophet is used only for the 'AI' player, keeping track of potential target positions by appending
 * all neighboring coordinates of a successful shot. The LinkedList prophet is emptied as soon as the 'AI' player sinks
 * an enemy ship, increasing the winning potential for the 'AI' player.
 * - Two dimensional array board keep track of the player's ship positioning, specifying each different ship with it's
 * respective ID.
 * - Two dimensional array tries is used only for the 'AI' player, preventing him from making shots in already tried
 * coordinates.
*/
public class Player {

    public Ship[] ship_array = new Ship[6];

    public LinkedList<Shot> shots = new LinkedList();
    public LinkedList<Coords> prophet = new LinkedList<>();

    public String name;
    public int[][] board = new int[11][11];
    public int[][] tries = new int[11][11];
    public int turn, points, functional_ships, successful_shots;

    /**
     * Class constructor specifying the name of the player.
    */
    public Player(String name) {
        turn = 1;
        points = 0;
        successful_shots = 0;
        functional_ships = 5;
        this.name = name;
    }

    /**
     * Return total points of the player.
    */
    public int getPoints() {
        return points;
    }

    /**
     * Setter method to increase player's score by the points provided.
     * @param points points to be added to player's score
    */
    public void setPoints(int points) {
        this.points += points;
    }

    /**
     * Return the name of the player.
    */
    public String getName() {
        return name;
    }

    /**
     * Return the number of functional ships for the player.
    */
    public int getFunctional_ships() {
        return functional_ships;
    }

    /**
     * Decrease the number of functional ships for the player, after sunken status.
    */
    public void setFunctional_ships() {
        this.functional_ships--;
    }

    /**
     * Return the number of successful shots for the player.
    */
    public int getSuccessful_shots() {
        return successful_shots;
    }

    /**
     * Increase the number of successful shots for the player, after hit status.
    */
    public void setSuccessful_shots() {
        this.successful_shots++;
    }

    /**
     * Return the current turn for the player.
    */
    public int getTurn() {
        return turn;
    }

    /**
     * Method used only by the 'AI' player. If the prophet list isn't empty, coordinates of potential target is
     * returned. If the prophet list is empty, random coordinates are generated as soon as the 'AI' hasn't tried them.
     * @return coords array, containing x-coordinate on its first position and y-coordinate on its second position
    */
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

    /**
     * Check the opponent's board for successful shot. Handles ship damage status as well as functional ship count and
     * player's score for the opponent and player, respectively. If the 'AI' player is playing, potential targets are
     * generated and saved in the prophet LinkedList. Increases player's turn by 1. Updates GUI respectively.
     * @param opponent of the player calling the function
     * @param opponentBoard of the player calling the function. Immediate access to the GUI board
     * @param x coordinate selected by the player calling the function
     * @param y coordinate selected by the player calling the function
    */
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


    /**
     * Generates pop-up window providing information on ship status for all the ships of the player calling the
     * function. Ship status include Intact, Damaged or Sunken.
    */
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

    /**
     * Generated pop-up window proving information on the last 5 shots made by the player calling the function. These
     * include turn count, coordinates tried, hit or miss status, as well as ship name, in case of a successful shot.
    */
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

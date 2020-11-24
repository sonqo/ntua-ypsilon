package app.model.player;

import app.model.Shot;
import app.model.ship.Ship;

import java.util.LinkedList;

public class Player {

    public Ship[] ship_array = new Ship[6];

    public LinkedList<Shot> shots = new LinkedList();

    public int[][] board = new int[11][11];
    public int turn, points, functional_ships, successful_shots, ship_count, shot_count;

    public Player() {

        turn = 1;
        points = 0;
        shot_count = 0;
        ship_count = 5;
        successful_shots = 0;
        functional_ships = 5;

        for (int i=0; i<10; i++) {
            for (int j=0; j<10; j++) {
                board[i][j] = 0;
            }
        }

    }

    public int getPoints() {
        return points;
    }

    public void setPoints(int points) {
        this.points += points;
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

    public int getShip_count() {
        return ship_count;
    }

    public int getShot_count() {
        return shot_count;
    }

    public LinkedList getShots() {
        return shots;
    }

    public int getTurn() {
        return turn;
    }
}

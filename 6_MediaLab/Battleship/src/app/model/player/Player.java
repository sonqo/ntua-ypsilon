package app.model.player;

import app.model.ship.Ship;

public class Player {

    public Ship[] ship_array = new Ship[6];

    public int[][] board = new int[11][11];
    public int points, functional_ships, successful_shots;

    public Player() {

        points = 0;
        successful_shots = 0;
        functional_ships = 5;

        for (int i=0; i<10; i++) {
            for (int j=0; j<10; j++) {
                board[i][j] = 0;
            }
        }

    }

}

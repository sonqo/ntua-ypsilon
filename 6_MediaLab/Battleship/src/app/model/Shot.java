package app.model;

public class Shot {

    public int x, y, turn, ship;
    public String status;

    public Shot(int x, int y, int turn) {
        this.x = x;
        this.y = y;
        this.turn = turn;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getTurn() {
        return turn;
    }

    public String getStatus() {
        return status;
    }

    public String getShip() {
        if (ship == 1) {
            return  "Carrier";
        } else if (ship == 2) {
            return "Battleship";
        } else if (ship == 3) {
            return "Cruiser";
        } else if (ship == 4) {
            return "Submarine";
        } else if (ship ==5) {
            return "Destroyer";
        } else {
            return "";
        }
    }

}

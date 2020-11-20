package app.model.ship;

public class Battleship extends Ship{

    public int size = 4;
    public int hit_points = 250;
    public int destroy_bonus = 500;

    @Override
    public int getSize() {
        return size;
    }

}

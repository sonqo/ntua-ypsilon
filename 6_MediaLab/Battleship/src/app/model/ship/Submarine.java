package app.model.ship;

public class Submarine extends Ship{

    public int size = 3;
    public int hit_points = 100;

    @Override
    public int getSize() {
        return size;
    }

}

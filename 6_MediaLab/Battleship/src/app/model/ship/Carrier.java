package app.model.ship;

public class Carrier extends Ship{

    public int size = 5;
    public int hit_points = 350;
    public int destroy_bonus = 100;

    @Override
    public int getSize() {
        return size;
    }

}

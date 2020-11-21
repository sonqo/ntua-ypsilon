package app.model.ship;

public class Cruiser extends Ship{

    public int size = 3;
    public int hit_points = 100;
    public int destroy_bonus = 250;

    public String color="#FF7B25";

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public String getColor() {
        return color;
    }

}

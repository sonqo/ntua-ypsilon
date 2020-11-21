package app.model.ship;

public class Carrier extends Ship{

    public int size = 5;
    public int hit_points = 350;
    public int destroy_bonus = 100;

    public String color="#034F84";

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public String getColor() {
        return color;
    }

}

package app.model.ship;

public class Destroyer extends Ship{

    public int size = 2;
    public int hit_points = 50;

    public String color="#82B74B";

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public String getColor() {
        return color;
    }

}

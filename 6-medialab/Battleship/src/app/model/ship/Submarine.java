package app.model.ship;

public class Submarine extends Ship{

    public int id = 4;
    public int size = 3;
    public int hit_points = 100;

    public String color="#FEB236";

    @Override
    public int getId() {
        return id;
    }

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public void setSize() {
        this.size = getSize()-1;
    }

    @Override
    public String getState() {
        return state;
    }

    @Override
    public String getColor() {
        return color;
    }

    @Override
    public int getHit_points() {
        return hit_points;
    }
}

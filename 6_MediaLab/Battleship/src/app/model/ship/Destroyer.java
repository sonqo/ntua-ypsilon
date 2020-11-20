package app.model.ship;

public class Destroyer extends Ship{

    public int size = 2;
    public int hit_points = 50;

    @Override
    public int getSize() {
        return size;
    }

}

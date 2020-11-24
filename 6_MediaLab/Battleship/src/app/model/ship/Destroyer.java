package app.model.ship;

public class Destroyer extends Ship{

    public int id = 5;
    public int size = 2;
    public int hit_points = 50;

    public String color="#82B74B";

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
    public void setDamaged() {
        this.state = "Damaged";
    }

    @Override
    public void setSunken() {
        this.state = "Sunken";
    }

    @Override
    public String getColor() {
        return color;
    }

    @Override
    public int getHit_points() {
        return hit_points;
    }

    @Override
    public int getDestroy_bonus() {
        return super.getDestroy_bonus();
    }
}

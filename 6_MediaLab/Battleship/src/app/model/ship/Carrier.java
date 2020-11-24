package app.model.ship;

public class Carrier extends Ship{

    public int id = 1;
    public int size = 5;
    public int hit_points = 350;
    public int destroy_bonus = 100;

    public String color="#034F84";

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
        return destroy_bonus;
    }

}

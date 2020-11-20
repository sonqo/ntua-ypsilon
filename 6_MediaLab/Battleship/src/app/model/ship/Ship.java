package app.model.ship;

public class Ship {

    public int start, finish, orientation;
    public int size, hit_points, destroy_points;

    public void setStats(int start, int finish, int orientation){
        this.start = start;
        this.finish = finish;
        this.orientation = orientation;
    }

    public int getSize(){
        return size;
    }

}

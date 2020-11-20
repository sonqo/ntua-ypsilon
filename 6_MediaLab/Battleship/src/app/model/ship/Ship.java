package app.model.ship;

import java.util.LinkedList;
import java.util.List;
import javafx.scene.layout.AnchorPane;

public class Ship {

    public int start, finish, orientation;
    public int size, hit_points, destroy_points;

    public LinkedList<AnchorPane> anchorList = new LinkedList();

    public void setStats(int start, int finish, int orientation){
        this.start = start;
        this.finish = finish;
        this.orientation = orientation;
    }

    public int getStart() {
        return start;
    }

    public int getFinish() {
        return finish;
    }

    public int getOrientation(){
        return orientation;
    }

    public int getSize(){
        return size;
    }

    public int getHit_points() {
        return hit_points;
    }

    public int getDestroy_points() {
        return destroy_points;
    }

    public LinkedList<AnchorPane> getAnchorList(){
        return anchorList;
    }

    public void fillAnchorList(List<AnchorPane> list){
        LinkedList<AnchorPane> curr = getAnchorList();
        if (getOrientation() == 1) {
            for (int i=0; i<getSize(); i++) {
                curr.add(list.get(10*getStart()+getFinish()+i));
            }

        } else {
            for (int i=0; i<getSize()*10; i+=10) {
                curr.add(list.get(10*getStart()+getFinish()+i));
            }
        }
        for (int i=0; i<getSize(); i++) {
            curr.get(i).setStyle("-fx-background-color: darkgrey; -fx-border-color: white");
        }
    }
}

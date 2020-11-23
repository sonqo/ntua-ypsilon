package app.model.ship;
import app.model.player.Player;

import java.util.List;
import java.util.LinkedList;
import javafx.scene.layout.AnchorPane;

public class Ship {

    public String color;

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

    public String getColor() {
        return color;
    }

    public LinkedList<AnchorPane> getAnchorList(){
        return anchorList;
    }

    public void fillAnchorList(List<AnchorPane> list, Player user, int flag){
        LinkedList<AnchorPane> curr = getAnchorList();
        if (getOrientation() == 1) {
            for (int i=0; i<getSize(); i++) {
                curr.add(list.get(10*getStart()+getFinish()+i));
                user.board[getStart()+1][getFinish()+i+1] = 1;
            }

        } else {
            for (int i=0; i<getSize(); i++) {
                curr.add(list.get(10*getStart()+getFinish()+10*i));
                user.board[getStart()+i+1][getFinish()+1] = 1;
            }
        }
        if (flag == 1) {
            for (int i = 0; i < getSize(); i++) {
                curr.get(i).setStyle("-fx-background-color: " + getColor() + "; -fx-border-color: white");
            }
        }
//            list.get(10*(6-1) + (6-1)).setStyle("-fx-background-color:red");
    }
}

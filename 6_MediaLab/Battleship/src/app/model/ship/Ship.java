package app.model.ship;
import app.model.player.Player;
import app.model.exception.OverlapException;
import app.model.exception.OversizeException;

import java.util.List;
import java.util.LinkedList;
import javafx.scene.layout.AnchorPane;

public class Ship {

    public String state, color;

    public int id, start, finish, orientation;
    public int size, hit_points, destroy_bonus;

    public LinkedList<AnchorPane> anchorList = new LinkedList();

    public void setStats(int start, int finish, int orientation){
        this.start = start;
        this.finish = finish;
        this.state = "Intact";
        this.orientation = orientation;
    }

    public int getId() {
        return id;
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

    public void setSize() {
        this.size = getSize()-1;
    }

    public int getHit_points() {
        return hit_points;
    }

    public int getDestroy_bonus() {
        return destroy_bonus;
    }

    public String getState() {
        return state;
    }

    public void setDamaged() {
        this.state = "Damaged";
    }

    public void setSunken() {
        this.state = "Sunken";
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
                try {
                    if ((getStart()+1 < 0) || (getStart()+1 > 9) || (getFinish()+i+1 < 0) || (getFinish()+i+1 > 9)) {
                        throw new OversizeException();
                    } else {
                        try {
                            if (user.board[getStart()+1][getFinish()+i+1] != 0) {
                                throw new OverlapException();
                            } else {
                                curr.add(list.get(10*getStart()+getFinish()+i));
                                user.board[getStart()+1][getFinish()+i+1] = getId();
                            }
                        } catch (OverlapException e) {
                            e.handleException();
                        }

                    }
                } catch (OversizeException e) {
                    e.handleException();
                }
            }
        } else {
            for (int i=0; i<getSize(); i++) {
                try {
                    if ((getStart()+i+1 < 0) || (getStart()+i+1 > 9) || (getFinish()+1 < 0) || (getFinish()+1 > 9)) {
                        throw new OversizeException();
                    } else {
                        try {
                            if (user.board[getStart()+i+1][getFinish()+1] != 0) {
                                throw new OverlapException();
                            } else {
                                curr.add(list.get(10*getStart()+getFinish()+10*i));
                                user.board[getStart()+i+1][getFinish()+1] = getId();
                            }
                        } catch (OverlapException e) {
                            e.handleException();
                        }
                    }
                } catch (OversizeException e) {
                    e.handleException();
                }
            }
        }
        if (flag == 1) {
            for (int i = 0; i < getSize(); i++) {
                curr.get(i).setStyle("-fx-background-color: " + getColor() + "; -fx-border-color: white");
            }
        }
    }
}

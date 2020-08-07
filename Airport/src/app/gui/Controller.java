package app.gui;

import java.net.URL;
import javafx.util.Duration;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.fxml.Initializable;
import javafx.animation.Animation;
import javafx.scene.control.Label;
import javafx.scene.control.Button;

public class Controller implements Initializable {

    public Button startButton;
    public Label timerLabel;

    int hours = 0;
    int minutes = 0;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    private void runTimer(String name){
        minutes ++;
        if (minutes == 60){
            minutes = 0;
            hours++;
        }
        timerLabel.textProperty().set(name + String.format("%02d:%02d", hours, minutes));
    }

    public void startApp(ActionEvent actionEvent) { // Start button

        // Timer handling
        String defaultTimer = timerLabel.textProperty().get();
        timerLabel.textProperty().set(defaultTimer + "00:00");
        Timeline timeline = new Timeline(new KeyFrame(
                Duration.millis(5000),
                ae -> runTimer(defaultTimer)));
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

    }
}

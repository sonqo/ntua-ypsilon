package app.gui;

import java.net.URL;
import java.io.IOException;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.Initializable;

public class Controller implements Initializable {

    String defaultEnemy = "medialab/enemy_SCENARIO-01.txt";
    String defaultPlayer = "medialab/player_SCENARIO-01.txt";

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }

    public void startApp(ActionEvent actionEvent) throws IOException, InterruptedException {
        // Setting up player's scenario

        // Setting up enemy's scenario
    }

}

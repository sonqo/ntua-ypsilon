package app.gui;

import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.Parent;
import javafx.fxml.FXMLLoader;
import javafx.application.Application;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("MediaLab Battleship");
        primaryStage.setScene(new Scene(root, 1300, 900));
        primaryStage.show();
    }
    
    public static void main(String[] args){
        launch(args);
    }
}

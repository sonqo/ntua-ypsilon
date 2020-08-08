package app.gui;

import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.Parent;
import javafx.fxml.FXMLLoader;
import javafx.scene.image.Image;
import javafx.application.Application;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("MediaLab Airport");
        primaryStage.getIcons().add(new Image("file:/C:/Users/Sonqo/Desktop/Ntua_Lambda/6_MediaLab/Airport/images/stackoverflow_v1.png"));
        primaryStage.setScene(new Scene(root, 1024, 624));
        primaryStage.show();
    }
    
    public static void main(String[] args){
        launch(args);
    }
}

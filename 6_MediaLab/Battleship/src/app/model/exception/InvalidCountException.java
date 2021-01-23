package app.model.exception;

import javafx.stage.Stage;
import javafx.scene.image.Image;
import javafx.scene.control.Alert;

public class InvalidCountException extends Exception {

    public String toString() {
        return ("Too many ships of the same type found!\nThe application will now close.");
    }

    public void handleException() {
        Alert dialog = new Alert(Alert.AlertType.ERROR);
        dialog.setHeaderText(null);
        dialog.setContentText(toString());
        dialog.setTitle("InvalidCountException");
        Stage stage = (Stage) dialog.getDialogPane().getScene().getWindow();
        stage.getIcons().add(new Image("file:images/battleship_v2.jpg"));
        dialog.showAndWait();
        System.exit(-1);
    }

}

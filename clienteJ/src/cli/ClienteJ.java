package cli;

import java.io.*;
import java.net.*;

public class ClienteJ {

    private static final String SERVER_IP = "127.0.0.1";
    private static final int PORT = 8080;

    public static void main(String[] args) {
        try (Socket socket = new Socket(SERVER_IP, PORT);
             BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

        		System.out.println("Menú:");
                System.out.println("1. Generar nombre de usuario");
                System.out.println("2. Generar contraseña");
                System.out.println("3. Salir");
                System.out.print("Elija una opción: ");
            while (true) {
                
                int choice = Integer.parseInt(userInput.readLine());

                if (choice == 3) {
                    break;
                }else if(choice == 1) {
                	System.out.print("Ingrese la longitud (entre 5 y 15): ");
                    int length = Integer.parseInt(userInput.readLine());

                    // Enviar el tipo de solicitud
                    out.println(choice);

                    // Enviar la longitud
                    out.println(length);

                    // Recibir y mostrar la respuesta del servidor
                    String response = in.readLine();
                    System.out.println("Respuesta del servidor: " + response);
                } else if(choice == 2) {
                	System.out.print("Ingrese la longitud (entre 8 y 50): ");
                    int length = Integer.parseInt(userInput.readLine());

                    // Enviar el tipo de solicitud
                    out.println(choice);

                    // Enviar la longitud
                    out.println(length);

                    // Recibir y mostrar la respuesta del servidor
                    String response = in.readLine();
                    System.out.println("Respuesta del servidor: " + response);
                } else {
                    System.out.println("Opción no válida. Intente de nuevo.");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

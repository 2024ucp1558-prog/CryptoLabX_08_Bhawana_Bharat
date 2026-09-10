import java.sql.*;
import java.util.Scanner;

public class ECommerceApp {
    private static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        System.out.println("=== E-Commerce Application ===");
        while (true) {
            System.out.println("\n1. Login (SQL Injection Vulnerability)");
            System.out.println("2. Browse Products");
            System.out.println("3. Checkout & Cart (Price Manipulation Vulnerability)");
            System.out.println("4. Order History (IDOR Vulnerability)");
            System.out.println("5. Exit");
            System.out.print("Select an option: ");

            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1:
                    login();
                    break;
                case 2:
                    browseProducts();
                    break;
                case 3:
                    checkout();
                    break;
                case 4:
                    viewOrderHistory();
                    break;
                case 5:
                    System.out.println("Exiting application.");
                    return;
                default:
                    System.out.println("Invalid option.");
            }
        }
    }


    public static void login() {
        System.out.print("Enter username: ");
        String username = scanner.nextLine();
        System.out.print("Enter password: ");
        String password = scanner.nextLine();

  
        String query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
        System.out.println("[DEBUG Executing Query]: " + query);

        if (username.contains("' OR '1'='1") || username.contains("' OR 1=1")) {
            System.out.println("Login successful! Welcome, Admin (Bypassed via SQL Injection).");
        } else {
            System.out.println("Login attempted using standard logic.");
        }
    }

  
    public static void browseProducts() {
        System.out.println("\n--- Available Products ---");
        System.out.println("1. Laptop - $1000");
        System.out.println("2. Smartphone - $500");
        System.out.println("3. Headphones - $100");
    }

    public static void checkout() {
        System.out.print("Enter item price ($): ");
        double price = scanner.nextDouble();
        System.out.print("Enter quantity: ");
        int quantity = scanner.nextInt();

       
        double total = price * quantity;
        System.out.println("Total Amount Charged: $" + total);
        if (total <= 0) {
            System.out.println("ALERT: Order processed for free or negative balance due to Price Manipulation!");
        } else {
            System.out.println("Order placed successfully!");
        }
    }

    // Vulnerability 3: Insecure Direct Object Reference (IDOR)
    // Allows viewing orders of any user by simply changing the numeric order ID without authorization checks
    public static void viewOrderHistory() {
        System.out.print("Enter Order ID to view details: ");
        int orderId = scanner.nextInt();

        // Missing authorization check: display data directly based on unvalidated ID
        System.out.println("Fetching details for Order ID: " + orderId);
        System.out.println("Order Details: User ID " + (orderId + 100) + " purchased Items worth $150.00");
        System.out.println("VULNERABILITY DETECTED: Any authenticated user can view Order ID " + orderId + " without authorization.");
    }
}

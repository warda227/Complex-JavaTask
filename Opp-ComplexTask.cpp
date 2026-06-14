// ==========================================
// 1. CUSTOM EXCEPTIONS
// ==========================================
class PayloadExceededException extends Exception {
    public PayloadExceededException(String message) {
        super(message);
    }
}

class LowPowerException extends Exception {
    public LowPowerException(String message) {
        super(message);
    }
}

// ==========================================
// 2. INTERFACE
// ==========================================
interface Trackable {
    void updateGPSLocation(double latitude, double longitude);
    String getStatusReport();
}

// ==========================================
// 3. ABSTRACT CLASS (Base Class)
// ==========================================
abstract class DeliveryVehicle {
    private String vehicleId;
    private double batteryLevel; // Percentage (0.0 to 100.0)
    private boolean isAvailable;

    public DeliveryVehicle(String vehicleId, double batteryLevel, boolean isAvailable) {
        this.vehicleId = vehicleId;
        this.batteryLevel = batteryLevel;
        this.isAvailable = isAvailable;
    }

    // Abstract Method to be implemented by children
    public abstract double calculateMaxPayload();

    public void chargeBattery() {
        this.batteryLevel = 100.0;
        System.out.println("Vehicle " + vehicleId + " fully charged to 100%.");
    }

    // Getters and Setters
    public String getVehicleId() { return vehicleId; }
    public double getBatteryLevel() { return batteryLevel; }
    public void setBatteryLevel(double batteryLevel) { this.batteryLevel = batteryLevel; }
    public boolean isAvailable() { return isAvailable; }
    public void setAvailable(boolean available) { this.isAvailable = available; }
}

// ==========================================
// 4. CHILD CLASSES (Specialized Vehicles)
// ==========================================
class Drone extends DeliveryVehicle implements Trackable {
    private int rotorCount;
    private double flightRangeKM;
    private double lat, lon;

    public Drone(String vehicleId, double batteryLevel, boolean isAvailable, int rotorCount, double flightRangeKM) {
        super(vehicleId, batteryLevel, isAvailable);
        this.rotorCount = rotorCount;
        this.flightRangeKM = flightRangeKM;
    }

    @Override
    public double calculateMaxPayload() {
        return this.rotorCount * 2.5; // Custom payload math
    }

    @Override
    public void updateGPSLocation(double latitude, double longitude) {
        this.lat = latitude;
        this.lon = longitude;
    }

    @Override
    public String getStatusReport() {
        return "[Drone " + getVehicleId() + "] Battery: " + getBatteryLevel() + "%, Available: " + isAvailable() + 
               ", Max Payload: " + calculateMaxPayload() + "kg, GPS: (" + lat + ", " + lon + ")";
    }
}

class GroundRobot extends DeliveryVehicle implements Trackable {
    private double wheelTreadWidth;
    private String navigationType;
    private double lat, lon;

    public GroundRobot(String vehicleId, double batteryLevel, boolean isAvailable, double wheelTreadWidth, String navigationType) {
        super(vehicleId, batteryLevel, isAvailable);
        this.wheelTreadWidth = wheelTreadWidth;
        this.navigationType = navigationType;
    }

    @Override
    public double calculateMaxPayload() {
        return this.wheelTreadWidth * 50.0; // Custom payload math
    }

    @Override
    public void updateGPSLocation(double latitude, double longitude) {
        this.lat = latitude;
        this.lon = longitude;
    }

    @Override
    public String getStatusReport() {
        return "[GroundRobot " + getVehicleId() + "] Battery: " + getBatteryLevel() + "%, Available: " + isAvailable() + 
               ", Max Payload: " + calculateMaxPayload() + "kg, Nav: " + navigationType + ", GPS: (" + lat + ", " + lon + ")";
    }
}

// ==========================================
// 5. COMPOSITION CLASSES (Product & Order)
// ==========================================
class Product {
    private String name;
    private double weight; // in kg
    private double price;

    public Product(String name, double weight, double price) {
        this.name = name;
        this.weight = weight;
        this.price = price;
    }

    public String getName() { return name; }
    public double getWeight() { return weight; }
}

class Order {
    private String orderId;
    private String destination;
    private Product product; // Composition
    private String status;

    public Order(String orderId, String destination, Product product) {
        this.orderId = orderId;
        this.destination = destination;
        this.product = product;
        this.status = "Pending";
    }

    public String getOrderId() { return orderId; }
    public Product getProduct() { return product; }
    public void setStatus(String status) { this.status = status; }
    public String getStatus() { return status; }
}

// ==========================================
// 6. BUSINESS LOGIC (Warehouse Manager)
// ==========================================
class WarehouseManager {
    private DeliveryVehicle[] fleet;

    public WarehouseManager(DeliveryVehicle[] fleet) {
        this.fleet = fleet;
    }

    public void dispatchOrder(Order order) throws PayloadExceededException, LowPowerException {
        System.out.println("\n>> Processing Dispatch for Order: " + order.getOrderId() + " (" + order.getProduct().getName() + ", " + order.getProduct().getWeight() + "kg)");
        
        boolean foundCapableVehicle = false;
        boolean foundWithEnoughPower = false;

        for (DeliveryVehicle vehicle : fleet) {
            // Check weight capability
            if (vehicle.calculateMaxPayload() >= order.getProduct().getWeight()) {
                foundCapableVehicle = true;
                
                // Check power capability
                if (vehicle.getBatteryLevel() >= 20.0) {
                    foundWithEnoughPower = true;
                    
                    // Check availability
                    if (vehicle.isAvailable()) {
                        // Allocation logic
                        vehicle.setAvailable(false);
                        vehicle.setBatteryLevel(vehicle.getBatteryLevel() - 15.0); // deployment battery cost
                        order.setStatus("Dispatched");
                        
                        // If it's trackable, update its mock GPS location
                        if (vehicle instanceof Trackable) {
                            ((Trackable) vehicle).updateGPSLocation(33.6844, 73.0479);
                        }

                        System.out.println("SUCCESS: Allocated to Vehicle [" + vehicle.getVehicleId() + "]. Order Status updated to: " + order.getStatus());
                        return; // Exit method on successful dispatch
                    }
                }
            }
        }

        // Exception hierarchy handling if execution reaches here
        if (!foundCapableVehicle) {
            throw new PayloadExceededException("CRITICAL ERROR: No vehicle in the entire fleet can carry " + order.getProduct().getWeight() + "kg.");
        } else if (!foundWithEnoughPower) {
            throw new LowPowerException("OPERATIONAL ERROR: Capable vehicles are available but all have critical battery (< 20%). Dispatch aborted.");
        } else {
            System.out.println("DELAY: All qualified vehicles are currently busy. Order remains Pending.");
        }
    }

    public void printFleetReport() {
        System.out.println("\n========== WAREHOUSE FLEET STATUS REPORT ==========");
        for (DeliveryVehicle vehicle : fleet) {
            if (vehicle instanceof Trackable) {
                System.out.println(((Trackable) vehicle).getStatusReport());
            }
        }
        System.out.println("===================================================\n");
    }
}

// ==========================================
// 7. SIMULATION (Main Class)
// ==========================================
public class Main {
    public static void main(String[] args) {
        // Initialize fleet with varying specifications
        DeliveryVehicle[] fleet = new DeliveryVehicle[4];
        fleet[0] = new Drone("DRONE-01", 85.0, true, 4, 15.0);          // Max Payload: 10kg
        fleet[1] = new Drone("DRONE-02", 15.0, true, 6, 20.0);          // Max Payload: 15kg, Low Battery!
        fleet[2] = new GroundRobot("ROBOT-01", 90.0, true, 0.8, "LIDAR"); // Max Payload: 40kg
        fleet[3] = new GroundRobot("ROBOT-02", 70.0, true, 1.5, "Sonar"); // Max Payload: 75kg

        WarehouseManager manager = new WarehouseManager(fleet);

        // Create 3 diverse test products & orders
        Order order1 = new Order("ORD-101", "Zone A", new Product("Flagship Smartphone", 0.2, 1200)); 
        Order order2 = new Order("ORD-102", "Zone B", new Product("Heavy Microwave", 12.0, 350));    
        Order order3 = new Order("ORD-103", "Zone C", new Product("Industrial Power Generator", 500.0, 4500)); 

        // Simulate operations with safe handling of complex business exceptions
        try {
            manager.dispatchOrder(order1); // Should seamlessly use DRONE-01
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

        try {
            manager.dispatchOrder(order2); // Drone-02 is dead battery, Drone-01 is now busy, should fallback to ROBOT-01
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

        try {
            manager.dispatchOrder(order3); // Will trigger PayloadExceededException (Too heavy for any asset)
        } catch (PayloadExceededException | LowPowerException e) {
            System.err.println(e.getMessage()); // Printed in error stream for emphasis
        }

        // Print final status dashboard
        manager.printFleetReport();
    }
}

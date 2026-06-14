# Complex-JavaTask
## Scenario: Automated Smart Warehouse & Drone Delivery System
A tech-logistics company wants to automate its smart fulfillment center. The warehouse stores various products, and instead of humans, Autonomous Ground Robots (AGRs) and Aerial Delivery Drones manage the inventory and handle dispatches. You need to design the backend architecture for this system in Java.
* 1. Core Architecture & OOP Requirements
A. Core Abstraction & Inheritance (Abstract Class)
Create an abstract class named DeliveryVehicle with the following:
Attributes: String vehicleId, double batteryLevel (percentage), and boolean isAvailable.
Constructor: To initialize the vehicle details.
Abstract Method: abstract double calculateMaxPayload() — each vehicle type handles weight limits differently.
Concrete Method: void chargeBattery() — sets batteryLevel to 100%.
B. Specialized Vehicles (Child Classes)
Drone (Extends DeliveryVehicle):
Additional attributes: int rotorCount, double flightRangeKM.
Implementation of calculateMaxPayload(): Return rotorCount * 2.5 (in kg).
GroundRobot (Extends DeliveryVehicle):
Additional attributes: double wheelTreadWidth, String navigationType (e.g., "LIDAR").
Implementation of calculateMaxPayload(): Return wheelTreadWidth * 50 (in kg).
C. Interfaces for Polymorphic Behavior
Create an interface named Trackable:
Method: void updateGPSLocation(double latitude, double longitude);
Method: String getStatusReport();
Both Drone and GroundRobot must implement the Trackable interface, providing their own custom tracking messages.
D. Package & Order Management (The Composition)
Create a Product class (name, weight, price) and an Order class.
The Order class should contain a unique orderId, a destination, and a Product object (Composition).
It should also have a status attribute ("Pending", "Dispatched", "Delivered").
* 2. Business Logic & Complex Operations
Create a WarehouseManager class that manages an array or collection of DeliveryVehicle objects and processes orders using the following complex logic:
Polymorphic Vehicle Allocation: When an Order needs to be dispatched, the system must loop through all available vehicles (DeliveryVehicle[]) and find a vehicle that meets all of these conditions:
The vehicle isAvailable == true.
The vehicle's batteryLevel is greater than or equal to 20%.
The vehicle's calculateMaxPayload() is greater than or equal to the total weight of the product in the order.
Order Processing: If a matching vehicle is found:
Change the vehicle's availability to false.
Reduce its battery level by 15% (simulating deployment).
Update the order status to "Dispatched".
Custom Exception Handling (Crucial):
If no capable vehicle is found because of weight constraints, throw a custom exception: PayloadExceededException.
If vehicles exist but all have low battery, throw a custom exception: LowPowerException.
* 3. Simulation Task (Main Class)
In your Main method, simulate the entire warehouse operations:
Create an array of mixed vehicles (2 Drones, 2 GroundRobots) with different battery levels and specifications.
Create 3 different orders (e.g., a lightweight Smartphone, a medium-weight Microwave, and an extremely heavy Industrial Generator).
Attempt to dispatch each order using a try-catch block to gracefully handle the custom exceptions when a delivery fails.
Print a detailed dashboard showing the status reports of all trackable vehicles at the end.
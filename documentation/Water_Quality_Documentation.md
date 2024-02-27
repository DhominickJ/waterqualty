## Water Quality Documentation
==============================================================

# Features of Application:
    - Automatic WiFi Connection Interface
    - Interactive UI System featuring Sensor Information
    - Real-time Connection Interface for the Model Type via WebSocket
    - Locally Hosted Server Interface
    - Automatic Database Schematics Modelling
    - Confirmed Working with Different Devices such as Website, Mobile, PC, etc.

## Important Notes to Consider: 

Server : NodeMCU 

LED Lights: 

RED : Powering ON

Blue: WiFi

Green: Ready State

Important Variables to Consider:
    
    - Node MCU : Server *ipAddress

        const char *wifiSSID = "WiFi_Name Here";
        const char *wifiPass = "password_here";

        const char *serverAddress = "192.168.1.16"; // IP address of the server (PC to connect to)

    - Server Variables:
        // Used for specifying the localhost of the network the NodeMCU will connect to.

        const ip = "192.168.1.16";

    - Server Database Make File:

        // Using a make file system processes are automated with the bat file. Further tuning required for UI.

        make db // Running the Database 

        Run the Server.bat 

        Change the directory of the project before use


    - Server Browser: 
        
        Manual Running:
            node . // Use on the directory of the program

            nodemon . // For automation purposes

        Automated Script:
            Run.bat // Automated processes for given parameters are never changed. 


        // Using the make file of the program, we can run the browser that the sensors can connect to. 

        Issues:
            - Serial Displays only work 
            - No interaction with the UI and Variables associated
            - Further work needed for porting the Water Related Sensors
        

February 15 - 18, 2024:

    - Node MCU
        
        - Cloned AirQuality with a few modifications on the system-implementation

        * Changed D1 to GPIO Consecutive Pins Respectively 

        * Removed redundant sensors unrelated to water quality

        * Restructured code for communication by changing the parameters of the function to only fit specific sensors ---> Planning to use pointers for structure rather than using parameters int eh future Agile Implementation.

    Server

    Progress:

        - UI was developed with sample data
        - No responses in the UI and the data
        - Database was successfully integrated with the program
        - Non-zero modifier was verified working - Rei
        - Changed parameters in calculating the sensor data to avoid zero-modifier function from triggering causing loss of connection to database.


February 19, 2024:

    Wiring Layout Documentation:
        D15 - GPIO 15 - DHT11 Sensor
        D2 - GPIO 2 - RED - POWER_LED
        D4 - GPIO 4 - GREEN - READY_LED
        D5 - GPIO 5 - BLUE - WIFI_LED
        D18 - GPIO 18 - TRIGGER_PIN

Implemented Features:
    Custom Parameter Reset Button - Imported on Demand Calls Structure.

    - Changed the connection structure of the Wifi Structure for a more convenient format.
    - Created a new function to check constantly the state of wifi.
    - Automation bug fixes.
    - Restart on call function (on button press)

(Project Pictures Sent on GC)

    


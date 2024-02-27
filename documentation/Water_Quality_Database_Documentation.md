### Water Quality Database Documentation

    Project Stack: MEHCN - MongoDB Express HTML CSS Node.js
    Project Title: Air Quality - Web App

    Data Routes: Arduino -> Server -> Database Stack via Mongoose -> MongoDB -> MongoDB Compass (Access)

    Database Connection Types: interfacing through the mongod connection module:
    Database User Interface: Use Mongo Database Compass Community Edition to ensure automatic schematic modelling for easy access types.

# Installation:

    Installation: 

        1. Open terminal: 
            winget install --id MongoDB.Server
            winget install --id MongoDB.Compass.Community

        2. Install the path variables towards the database modules to use the command mongod 

        Explanation: Mongod means mongod daemon -> a status type that consist of code structure to be used by the mongodb service. In short, to run the command, a server command is needed such that: 

            mongod --port port_number --dbpath database_path // Make sure that the folder exist before running the bat files.

        3. Run the server.bat files

        4. Run the run.bat files after running the server.bat file to avoid the errors.


## Process Types Explanation

    Directory: server/ 
        - Contains the codes for the server files. It includes the models and the connection to the servers. 
    
    File: server/models.js
        - Contains the connection information towards the server model.

    File: server/schema.js 
        - Contains the schematic models for the server files which is interfaced and exported (Special Thanks to Rei for recommendation) to be easily used for modification and access upon server request. 

    Interfacing: 
        - Using MongoDB Compass
            Use the connection link specified below to access the database entries and the graphs using the latest sensor information and all of the data gathered by the sensor captured and sent by the server.

        Connect to URI on MongodDB Compass:
            mongodb://localhost:6969/sensor_data


## Possible Errors:
    
    Error 100: Existence of Mongodb.lock on /database (Took me 3 days to figure out):
        - Stop the service, and delete the .lock file in the ./database and run the module again.
    
    Error 400: Folder doesn't exist
        - Calling the database directory in a terminal with the location that doesn't exist. Explained on the Installation Tab above.

    Any possible error types, reach out to me or Rei: 
        
        - github.com/Reiebenezer
        - github.com/DhominickJ


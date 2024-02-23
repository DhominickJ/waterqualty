const path = require("path");
const express = require("express");
const { SensorData, saveSensorData } = require("./server/models");



const app = express();
const appWs = require("express-ws")(app);


app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "client"));
app.use(express.static("client"));

const PORT = process.env.PORT || 1337;
// const ip = '192.168.43.119';
// const ip = '192.168.3.156';
const ip ="192.168.43.114";


const SENSORS = new Set();
const USERS = new Set();

app.ws("/esp", (ws) => {
    SENSORS.add(ws);

    ws.send("Server response message. Connection to server verified");
    ws.timeout = 0;

    ws.id = -1;

    ws.data = {
        temperature: 0,
        humidity: 0,
    };

    ws.averageData = {
        temperature: 0,
        humidity: 0,
    };
    ws.tempData = {
        temperature: 0,
        humidity: 0,
    };


    ws.gasConcentration = 0;
    ws.averageCounter = 1;

    ws.on("message", (msg) => {
        ws.timeout = 0;

        if (msg === "ping") {
            ws.timeout = 0;
        } else {
            const parsed_msg = JSON.parse(msg);

            switch (parsed_msg["type"]) {
                case "id":
                    ws.id = parsed_msg["id"];

                    console.log(`New sensor ${ws.id} connected to server`);
                    break;

                case "data":
                    ws.gasConcentration =
                        +parsed_msg["gas_concentration"] || ws.gasConcentration;

                    ws.data.temperature =
                        +parsed_msg["temperature"] || ws.data.temperature;
                    
                    ws.data.humidity =
                        +parsed_msg["humidity"] || ws.data.humidity;
                    
                    // ws.data.ozone = 
                    //     +parsed_msg["ozone"] || ws.data.ozone;
                    
                    // ws.data.carbonMonoxide =
                    //     +parsed_msg["carbon_monoxide"] || ws.data.carbonMonoxide;

                    // Insert new row in database
                    // console.log(ws.data);

                    break;

                default:
                    break;
            }
        }
    });

    ws.on("close", () => {
        SENSORS.delete(ws);
        console.log(`Sensor ${ws.id} disconnected from server`);
    });
});

app.ws("/browser", (ws) => {
    console.log("New browser connected");
    USERS.add(ws);

    ws.on("message", (msg) => {
        console.log(`Browser has sent message: ${msg}`);

        if (msg === "disconnect-all") {
            console.log("Disconnecting all sensors...");
            SENSORS.forEach((sensor) => {
                sensor.send("disconnect");
            });
        }
    });

    ws.on("close", () => {
        console.log(`Browser disconnected`);
        USERS.delete(ws);
    });
});

app.get("/", (req, res) => {
    res.render("index", { ip: `ws://${ip}:${PORT}` });
});

app.get("/db", (req, res) => {
    res.sendFile(path.join(__dirname, "client/db.html"));
});

// For averaging_values for database_entries
var average_temp = 0;
var average_humid = 0;
var average_gasCon = 0;

var average_counter = 0;

app.listen(PORT, () => {
    console.log(`node: SERVER STARTED at port ${PORT}`);
});

setInterval(() => {
    const sensor_data = [];
    // const sensor_entries = [];


    SENSORS.forEach((sensor) => {
        if (sensor.timeout > 10) {
            sensor.send("disconnect");

            SENSORS.delete(sensor);
            console.log(`Removed unresponsive sensor ${sensor.id}`);
        } else {
            let hasZero = false;
            for (key in sensor.data) {
                if (sensor.data[key] === 0) {
                    hasZero = true;
                    break;
                }
            }

            
            console.log(sensor.averageCounter, sensor.data);
            sensor_data.push({...sensor.data, id: sensor.id, timeout: sensor.timeout});

            if (!hasZero) {
                for (key in sensor.data) {

                    if (key !== "id" || key !== "timeout") {
                        // Check if sensor.data[key] is within 10% of sensor.averageData[key]
                        sensor.tempData[key] = sensor.averageData[key];
                        var threshold = sensor.averageData[key] - sensor.data[key];
                        if(Math.abs(threshold) > 0.1 * sensor.averageData[key]) {
                            sensor.tempData[key] += sensor.data[key]; // add new data point to running total
                            sensor.averageData[key] = sensor.tempData[key] / sensor.averageCounter; // calculate average
                            // console.log(sensor.tempData);
                        }
                    }
                    
                    delete sensor.averageData["id"];
                    delete sensor.averageData["timeout"];
                    delete sensor.averageData['ozone'];
                    delete sensor.averageData['carbonMonoxide'];

                }

                // console.log(sensor.data);
                sensor.averageCounter++;

                if (sensor.averageCounter > 60) {
                    // console.log(sensor.averageData);
                    
                    // Create a sample test data for the database
                    const sensorData = new SensorData({
                        sensorId: sensor.id,
                        temperature: sensor.averageData.temperature,
                        humidity: sensor.averageData.humidity,
                        // ozone: sensor.averageData.ozone,
                        // carbonMonoxide: sensor.averageData.carbonMonoxide,
                    });
                    
                    
                    // Save the sample data to the database
                    saveSensorData(sensorData);


                    sensor.averageCounter = 1;

                    sensor.averageData.temperature = 0;
                    sensor.averageData.humidity = 0;
                    // sensor.averageData.ozone = 0;
                    // sensor.averageData.carbonMonoxide = 0;

                    // Reseting Temp Data

                    sensor.tempData.temperature = 0;
                    sensor.averageData.humidity = 0;
                    // sensor.averageData.ozone = 0;
                    // sensor.averageData.carbonMonoxide = 0;
                }
            }

            average_temp += sensor.temperature;
            average_humid += sensor.humidity;
            // average_gasCon += sensor.gasConcentration;
            average_counter++;

            sensor.timeout++;       
        }    
    });

    USERS.forEach((user) => {
        user.send(JSON.stringify(sensor_data));
    });
}, 1000);

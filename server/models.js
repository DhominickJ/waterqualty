const mongoose = require("mongoose");
const sensorDataSchema = require("./schemas");

// Connect to MongoDB
mongoose.connect('mongodb://localhost:6969/water_quality_sensor');

// Create a model for sensor data
const SensorData = mongoose.model("SensorData", sensorDataSchema);

// To be modified to work for the saving the data
async function saveSensorData(sensorData) {
    try {
        const data = await sensorData.save();
        console.log("Sensor data saved successfully!");
    } catch (err) {
        console.error(err);
    }
}

module.exports = {
    SensorData,
    saveSensorData
}